#include "NateCode.h"
#include "NateParser.h"
#include "NateFunctions.h"
#include "Identifier.h"

#include <algorithm>

NateCode::NateCode(std::ostream& aOut, NateParser* aParser)
  : mOut(&aOut),
	  mParser(aParser)
{
}

std::string NateCode::in(int extra)
{
	return std::string(std::max(0, mIndent + extra), '\t');
}

void NateCode::printLineNr(const Location& aLocation)
{

	if (aLocation.beginLine != mPrevLine || 
			aLocation.filename != mPrevFile)
	{
		*mOut << "#line " << aLocation.beginLine;
		if (mPrevFile != aLocation.filename)
		{
			*mOut << " \"" << aLocation.filename << "\"";
		}
			
		*mOut << std::endl;

		mPrevLine = aLocation.beginLine;
		mPrevFile = aLocation.filename;
	}
}

void NateCode::codeStats(const std::list<Stat::SPtr>& aStats)
{
  for (auto& stat : aStats)
  {
		stat->accept(this);
	}
}

void NateCode::codeNested(const TreeNodePtr& aStat)
{
  for (auto& stat : aStat->nested)
  {
		//code(stat);
	}
}

void NateCode::codeCompound(const Stat& aStat)
{
	*mOut << in() << "{" << end();
	++mIndent;
	codeStats(aStat.getCompound());
	--mIndent;
	*mOut << in() << "}" << end();
}

void NateCode::visit(const StatProgram& aStat)
{
	*mOut << in() << "#undef NOMINMAX" << end();
	*mOut << in() << "#define NOMINMAX" << end();
	*mOut << in() << "#include <windows.h>" << end();

	printLineNr(aStat.getLocation());

	*mOut << in() << "int main(int argc, char** argv)\n" << in() << "{" << end();
	++mIndent;
	*mOut << in() << "output = std::shared_ptr<std::ostream>(&std::cout, [](void*) {});" << end();
	*mOut << in() << "error = std::shared_ptr<std::ostream>(&std::cerr, [](void*) {});" << end();
	*mOut << in() << "input = std::shared_ptr<std::istream>(&std::cin, [](void*) {});" << end();
	*mOut << in() << "SetConsoleOutputCP(65001);" << end();
	//*mOut << "std::locale::global(std::locale(\"en_US.UTF8\"));" << end();
	
	codeStats(aStat.getCompound());
	--mIndent;
	*mOut << in() << "}" << end();
}

void NateCode::visit(const StatDeclareLocal& aStat)
{
	if (!aStat.getIdentifier()->is(Identifier::ObjectImpl))
	{
		printLineNr(aStat.getLocation());

		*mOut << in();

		if (aStat.getIdentifier()->is(Identifier::Const))
		{
			*mOut << "const ";		
		}

		*mOut << aStat.getIdentifier()->type()->codeType() << " " << aStat.getIdentifier()->codeName();

		*mOut << " = " << codeExpr(aStat.getExpr());
	
		*mOut << ";" << end();
	}
}

void NateCode::visit(const StatAssign& aStat)
{
	std::string endPars;

	printLineNr(aStat.getLocation());

	for (auto const& expr : aStat.getIdentifiers())
	{				
		std::string code = codeExpr(expr);
		size_t size = code.size();
		if (size > 6 && code.substr(size - 6, 6) == "_get()")
		{
			code[size - 5] = 's';
			code[size - 1] = '\0';
			endPars += ")";
			*mOut << in() << code;
		}
		else
		{
			*mOut << in() << code << " = ";
		}
	}

	*mOut << aStat.getExpr().code() << endPars << ";" << end();
}

void NateCode::visit(const StatIfThen& aStat)
{
	printLineNr(aStat.getLocation());

	*mOut << in() << "if (" << codeExpr(aStat.getExpr()) << ")" << end();
	
	codeCompound(aStat);
}

void NateCode::visit(const StatIfThen::ElseIf& aStat)
{
	printLineNr(aStat.getLocation());

	*mOut << in() << "else if (" << codeExpr(aStat.getExpr()) << ")" << end();
	
	codeCompound(aStat);
}

void NateCode::visit(const StatIfThen::Else& aStat)
{
	printLineNr(aStat.getLocation());

	*mOut << in() << "else" << end();
	
	codeCompound(aStat);
}

void NateCode::visit(const StatIfIs& aStat)
{
	printLineNr(aStat.getLocation());
	
	bool firstIf = true;
	bool usesIf = false;
	bool usesSwitch = false;
	Stat::SPtr elsePart;
	
	*mOut << in() << "auto const " << aStat.getId()->codeName() << " = " << codeExpr(aStat.getExpr()) << ";" << end();
	
  for (auto& part : aStat.getCompound())
  {
		const StatIfIs::IsList* isList = dynamic_cast<const StatIfIs::IsList*>(part.get());
    if (isList != nullptr)
    {
			if (!isList->getCompound().empty())
			{
				if (isNestedNonConstIntScalar(*isList))
				{
					usesIf = true;
				}
				else if (isNestedConstIntScalar(*isList))
				{
					usesSwitch = true;
				}
			}
		}
		else
		{
	    elsePart = part;
		}
	}

	if (usesSwitch)
	{
		codeIfIsSwitch(aStat);
		if (elsePart)
		{
			*mOut << in() << "default:" << end();			

			if (usesIf)
			{
				*mOut << in() << "{" << end();
				++mIndent;
				codeIfIsIfs(aStat, elsePart);
				--mIndent;
			  *mOut << in() << "}" << end();
			}
			else
			{
				codeCompound(*elsePart);
			}
		}
		
		*mOut << in() << "break;" << end();
		*mOut << in() << "}" << end();
	}
	else
	{
		codeIfIsIfs(aStat, elsePart);
	}
}

void NateCode::visit(const StatLoop& aStat)
{
	printLineNr(aStat.getLocation());
	*mOut << in() << "while (true)" << end();
	codeCompound(aStat);
}

void NateCode::visit(const StatLoop::While& aStat)
{
	printLineNr(aStat.getLocation());
	*mOut << in() << "if (!(" << codeExpr(aStat.getExpr()) << ")) break;" << end();
}

void NateCode::visit(const StatLoop::ForStep& aStat)
{
	printLineNr(aStat.getLocation());

	*mOut << in() << "for (" << aStat.getId()->type()->codeType() << " " 
			 << aStat.getId()->codeName() << "= " << codeExpr(aStat.getStart()) << ";"  
			 << aStat.getId()->codeName() << (aStat.getDownTo() ? " >= " : "<=") << codeExpr(aStat.getEnd()) << "; "
			 << aStat.getId()->codeName() << (aStat.getDownTo() ? " -= " : "+=") << codeExpr(aStat.getStep()) << ")" << end();
	codeCompound(aStat);
}

void NateCode::visit(const StatLoop::ForRange& aStat)
{
	printLineNr(aStat.getLocation());

	TypePtr rangeType = aStat.getRange().type();
	auto range = mParser->uniqueName();
	auto iter = mParser->uniqueName();
	auto next = mParser->uniqueName();

	std::string ref = aStat.getRange().is(Expr::Output) ? "&" : "";
	std::string increment;

	if (rangeType->isOfType("text"))
	{
		increment = iter + "=" + next;
		*mOut << in() << "const std::string" << ref << " " << range << " = " << codeExpr(aStat.getRange()) << ";" << end(); 
		*mOut << in() << "auto " << next << " = " << range << ".cbegin();" << end();
	}
	else
	{
		increment = "++" + iter;
		*mOut << in() << "auto const" << ref << " " << range << " = " << codeExpr(aStat.getRange()) << ";" << end(); 
	}

	*mOut << in() << "for (auto " << iter << " = " << range << ".cbegin(); "
			 << iter << " != " << range << ".cend(); "
			 << increment << ")" << end();

	*mOut << in() << "{" << end();
	++mIndent;
	if (rangeType->isOfType("text"))
	{
		*mOut << in() << "uint32_t " << aStat.getId()->codeName() << " = utf8::next(" + next + "," + range + ".cend());" << end();
	}
	else
	{
		*mOut << in() << "auto const& " << aStat.getId()->codeName() << " = *" << iter << ";" << end();
	}
	
	codeStats(aStat.getCompound());
	--mIndent;
	*mOut << in() << "}" << end();
}

void NateCode::visit(const StatCode& aStat)
{
	printLineNr(aStat.getLocation());
	*mOut << in() << aStat.getCode() << end();
}

void NateCode::visit(const StatExpr& aStat)
{
	*mOut << " = " << codeExpr(aStat.getExpr());
}

void NateCode::visit(const StatOutput& aStat)
{
	codeOutputStart(aStat, "*output");
}

void NateCode::visit(const StatOutput::Comma& aStat)
{
	codeOutput("\" \"");
}

void NateCode::visit(const StatOutput::Concat& aStat)
{
	// nothing
}

void NateCode::visit(const StatOutput::End& aStat)
{
	if (aStat.getEndOfLine())
	{
		codeOutputNew();
		codeOutput("std::endl");
	}
	else
	{
		codeOutput("");
	}

	*mOut << end();
	mStartOutput = true;
	mFirstOutput = true;
}

void NateCode::visit(const StatOutput::Value& aStat)
{
	const Expr& expr = aStat.getExpr();

	if (expr.type() && expr.type()->is(Type::Boolean))
	{
		codeOutput("std::boolalpha ");
	}
	
	if (expr.is(Expr::Literal))
	{
		codeOutput(codeExpr(expr));
	}
	else
	{
		if (expr.type() && expr.type()->name() == "int-8")
		{
			codeOutput("static_cast<int>(" + codeExpr(expr) + ")");
		}
		else
		{
			Expr outExpr(Expr("stream-out"), expr);
			Expr resExpr = mParser->evaluate(outExpr);
			if (!resExpr.isEmpty())
			{
				codeOutput("(" + codeExpr(resExpr) + ")");
			}
			else
			{
				codeOutput("(" + codeExpr(expr) + ")");
			}
		}
	}
}

void NateCode::visit(const StatWrite& aStat)
{
	printLineNr(aStat.getLocation());

	if (aStat.getCreateIt())
	{
    codeDeclIdentifier(false, aStat.getWriter(), true, aStat.getLocation());
	}
	else if (!aStat.getOutput().isEmpty())
	{
		*mOut << in() << aStat.getWriter()->codeName() << " = " << codeExpr(aStat.getOutput()) << ";" << end();
	}

	codeOutputStart(aStat, "*" + aStat.getWriter()->codeName());
}

void NateCode::visit(const StatData& aStat)
{
	printLineNr(aStat.getLocation());
	std::string name = aStat.getId()->codeName() + "_temp";
	*mOut << in() << "std::ostringstream " << name << ";" << end();
	codeOutputStart(aStat, name, true);
	*mOut << in() << "const " << aStat.getId()->type()->codeType() << " " << aStat.getId()->codeName() << "= " << name << ".str();" << end();

}

void NateCode::codeOutputStart(const StatOutput& aStat, const std::string& aOutput, bool aDataOutput)
{
	printLineNr(aStat.getLocation());
	mStream = aOutput;
	mFirstOutput = true;
	mStartOutput = true;
	mDataOutput = aDataOutput;
	mCachedOutput.clear();

  for (auto& part : aStat.getCompound())
  {
		part->accept(this);
  }
}

char NateCode::end()
{
	++mPrevLine;
	return '\n';
}

void NateCode::codeRead(const TreeNodePtr& aNode)
{
	printLineNr(aNode->location);
	if (aNode->bool1)
	{
    //codeDeclIdentifier(aNode, false, aNode->id, true, aNode->location);
	}
	else if (!aNode->expr.isEmpty())
	{
		*mOut << in() << "nate__reader = " << codeExpr(/*aNode, */aNode->expr) << ";" << end();
	}

	codeInput("*nate__reader", aNode);
}

void NateCode::codeOutputNew()
{
	if (mFirstOutput)
	{
		if (mStartOutput)
		{
			*mOut << in();
	    mStartOutput = false;
		}

		*mOut << mStream;
		mFirstOutput = false;
	}
}

void NateCode::codeOutput(const std::string& aString)
{
	if ((!mCachedOutput.empty()) && aString[0] != '"')
	{
		codeOutputNew();
		*mOut << " << \"" << mCachedOutput << "\"";
		mCachedOutput.clear();
		if (!aString.empty())
		{
			codeOutputNew();
			*mOut << " << " << aString << (mDataOutput ? ";" : "; ");
			mFirstOutput = true;
		}
		else
		{
			*mOut << (mDataOutput ? ";" : "; ");
		}
	}
	else if (!mCachedOutput.empty())
	{
		mCachedOutput += unquote(aString);
	}
	else if (aString[0] == '"')
	{
		mCachedOutput = unquote(aString);
	}
	else
	{
		if (!aString.empty())
		{
			codeOutputNew();
			*mOut << " << " << aString << (mDataOutput ? ";" : "; ");
			mFirstOutput = true;
		}
	}
}

void NateCode::codeInput(const std::string& aStream, const TreeNodePtr& aNode)
{
	printLineNr(aNode->location);
	ByteCode lastSeperator = ByteCode::None;

	for (auto iter = aNode->nested.cbegin(); iter != aNode->nested.cend(); ++iter)
	{
		auto const &part = *iter;
		InputType curInputType = aNode->inputType;
	  auto next = std::next(iter);
		std::string skipSpaces = lastSeperator == ByteCode::SepComma ? " >> std::ws" : "";

    switch (part->code)
    {
    case ByteCode::Expr:
			if ((*next)->code != ByteCode::End || !part->expr.type()->is(Type::Text)) {
					curInputType = InputType::Normal;			
			}

			if (curInputType == InputType::Line)
			{
				*mOut << "std::getline(" << aStream << skipSpaces << ", " << codeExpr(/*aNode, */part->expr) << ");" << end();
			}
			else if (curInputType == InputType::All)
			{
			}
			else
			{
				if (part->expr.type()->is(Type::Boolean))
				{
					*mOut << aStream << skipSpaces << " >> std::boolalpha  >> " << codeExpr(/*aNode, */part->expr) << " >> std::noboolalpha;" << end();
				}
				else
				{
					*mOut << aStream << skipSpaces << " >> " << codeExpr(/*aNode, */part->expr) << ";" << end();
				}
			}
      break;
    case ByteCode::SepComma:
    case ByteCode::SepConcat:
			lastSeperator = part->code;
      break;
    case ByteCode::End:
			if (part->bool1 && curInputType == InputType::Normal)
			{
				*mOut << "(" << aStream << ").ignore(std::numeric_limits<std::streamsize>::max(), '\\n');" << end();
			}
      break;
		default:
			std::cerr << "Bad Input bytecode " << (int)part->code << std::endl;
			break;
    }
  }
}

void NateCode::codeLocalVar(const TreeNodePtr& aNode, bool inImplObject)
{
	//if (!aNode->id->is(Identifier::ObjectImpl) || inImplObject)
	//{
 //   codeDeclIdentifier(false, aNode->id, aNode->bool1, aNode->location);
	//}
}

void NateCode::codeDeclIdentifier(bool aExtern,
																	const IdentifierPtr& aIdentifier,
																	bool initializeVariables,
																	const Location& aLocation)
{
	printLineNr(aLocation);

	*mOut << in();

	if (aExtern)
	{
	  *mOut << "extern ";	
	}

	if (aIdentifier->is(Identifier::Const))
	{
		*mOut << "const ";		
	}

	*mOut << aIdentifier->type()->codeType() << " " << aIdentifier->codeName();

	if (initializeVariables)
	{
		*mOut << " = " << codeExpr(aIdentifier->initValue());
	}
	
	*mOut << ";" << end();
}

std::string NateCode::codeExpr(/*const TreeNodePtr& aNode, */const Expr& aValue)
{
	std::string result = aValue.code();
	if (aValue.is(Expr::Identifier) && !aValue.is(Expr::Property))
	{
		IdentifierPtr id = aValue.id();
		if (id)
		{
			if (id->isObjectMe())
			{
				result = "std::dynamic_pointer_cast<" + toCodeName(id->type()->name()) + ">(shared_from_this())";
			}
			else
			{
				IIdentifiersHolderPtr holder = id->identifiersHolder().lock();
				if (holder)
				{
					if (holder->scopeFlag() == IIdentifiersHolder::ScopeFlag::ObjectImpl)
					{
					/*	if (aNode->defyne && !aNode->defyne->is(Define::Undeclared) && 
								!aNode->defyne->object()->is(Type::ObjectImpl))
						{
							result = "_impl->" + result;
						}*/
					}
				}
			}
		}
	}

	return result;
}

bool NateCode::isConstIntScalar(const Expr& aExpr)
{
  return (aExpr.is(Expr::ConstExpr) && aExpr.type()->is(Type::Scalar) &&
		      !aExpr.type()->is(Type::Real));
}

bool NateCode::isNestedConstIntScalar(const Stat& aStat)
{
	return std::any_of(aStat.getCompound().begin(), aStat.getCompound().end(),
										[&](const Stat::SPtr& part) { const StatIfIs::Is* is = dynamic_cast<const StatIfIs::Is*>(part.get());
	                                          return is == nullptr || isConstIntScalar(is->getExpr()); });
}

bool NateCode::isNestedNonConstIntScalar(const Stat& aStat)
{
	return std::any_of(aStat.getCompound().begin(), aStat.getCompound().end(),
										[&](const Stat::SPtr& part) { const StatIfIs::Is* is = dynamic_cast<const StatIfIs::Is*>(part.get());
	                                          return is != nullptr && !isConstIntScalar(is->getExpr()); });
}


void NateCode::codeCaseIsListIf(const StatIfIs::IsList& aStat, const StatIfIs& aIfIsStat, bool firstIf)
{
	bool firstCond = true;

	printLineNr(aStat.getLocation());
	*mOut << in() << (firstIf ? "if " : "else if ") << "(";

	for (const Stat::SPtr& part : aStat.getCompound())
	{
		const StatIfIs::Is* is = dynamic_cast<const StatIfIs::Is*>(part.get());
		if (is != nullptr)
		{
			if (!firstCond)
			{
				*mOut << end();
				printLineNr(aStat.getLocation());
				*mOut << end() << in(4) << " || ";
			}

			*mOut << "(" << aIfIsStat.getId()->codeName() << " == " << codeExpr(is->getExpr()) << ")";
			firstCond = false;
		}
	}

	*mOut << ")" << end() << in() << "{" << end();
	++mIndent;
	for (const Stat::SPtr& part : aStat.getCompound())
	{
		const StatIfIs::Is* is = dynamic_cast<const StatIfIs::Is*>(part.get());
		if (is == nullptr)
		{
			part->accept(this);
		}
	}

	--mIndent;
	*mOut << in() << "}" << end();
}

void NateCode::codeIfIsIfs(const StatIfIs& aStat, const Stat::SPtr& aElsePart)
{
	bool firstIf = true;

  for (auto& part : aStat.getCompound())
  {
		const StatIfIs::IsList* isList = dynamic_cast<const StatIfIs::IsList*>(part.get());

	  if (isList != nullptr && isNestedNonConstIntScalar(*isList) && !isList->getCompound().empty())
    {
      codeCaseIsListIf(*isList, aStat, firstIf);
		  firstIf = false;
    }
  }

	if (aElsePart)
	{
		*mOut << in() << "else" << end();
	  codeCompound(*aElsePart);
	}
}

void NateCode::codeIfIsSwitch(const StatIfIs& aStat)
{
	printLineNr(aStat.getLocation());
	*mOut << in() << "switch (" + aStat.getId()->codeName() << ")" << end();
	*mOut << in() << "{" << end();
	
	for (auto& part : aStat.getCompound())
  {
		const StatIfIs::IsList* isList = dynamic_cast<const StatIfIs::IsList*>(part.get());
		if (isList != nullptr)
    {
      codeCaseIsListSwitch(*isList);
    }
  }
}

void NateCode::codeCaseIsListSwitch(const Stat& aStat)
{
	if (isNestedConstIntScalar(aStat) && !aStat.getCompound().empty())
	{
		for (auto& part : aStat.getCompound())
		{
			const StatIfIs::Is* is = dynamic_cast<const StatIfIs::Is*>(part.get());
			if (is != nullptr && isConstIntScalar(is->getExpr()))
			{
				codeCaseIsSwitch(*is);
			}
		}

		*mOut << in() << "{" << end();
		++mIndent;
		for (auto& part : aStat.getCompound())
		{
			const StatIfIs::Is* is = dynamic_cast<const StatIfIs::Is*>(part.get());
			if (is == nullptr || isConstIntScalar(is->getExpr()))
			{
				part->accept(this);
			}
		}
		
		*mOut << in() << "break;" << end();
		--mIndent;
		*mOut << in() << "}" << end();
	}
}

void NateCode::codeCaseIsSwitch(const StatIfIs::Is& aStat)
{
	printLineNr(aStat.getLocation());
	*mOut << in() << "case " << codeExpr(aStat.getExpr()) << ":" << end();
}

void NateCode::codeCodeInclude(const TreeNodePtr& aNode)
{
	printLineNr(aNode->location);
	*mOut << in() << aNode->string << end();
}

void NateCode::codeRecord(const TreeNodePtr& aNode)
{
	printLineNr(aNode->location);
	RecordPtr record = std::dynamic_pointer_cast<Record>(aNode->type);
	
	*mOut << in() << "struct " << record->codeType() << end() << in() << "{" << end();
	++mIndent;
	*mOut << in() << record->codeType() << "()" << end();
	bool first = true;

	for (auto& id : record->identifiers().get())
	{
		if (first)
		{
			*mOut << in(1) << ": ";
			first = false;
		}
		else
		{
			*mOut << in(1) << ", ";
		}

		*mOut << id->codeName() << "(";

		if (!id->initValue().is(Expr::Default))
		{
			*mOut << codeExpr(/*aNode, */id->initValue());
		}
		*mOut << ")" << end();
	}

	*mOut << in() << "{}" << end();
	codeNested(aNode);
	--mIndent;
	*mOut << in() << "};" << end() << end();
}

void NateCode::codeDefine(const TreeNodePtr& aNode)
{
	printLineNr(aNode->location);
	DefinePtr defyne = aNode->defyne;
	
	if (defyne->isObjectMethod())
	{				
		bool defineDecl = aNode->bool1;
		if (defineDecl)
		{
			*mOut << in(-1) << createCodeDecl(defyne, toCodeName(aNode->object->name())) << end() <<
				       in(-1) << "{" << end();
		}
		else
		{				
			*mOut << in() << (defyne->isStatic() ? "static " : "") <<
					              createCodeDecl(defyne) << end() << 
				       in() << "{" << end();
		}

		++mIndent;
		codeNested(aNode);
		--mIndent;
		*mOut << in() << "}" << end() << end();
	}
	else
	{
		bool isDecl = aNode->bool1;
		createCodeDecl(defyne, "");
		if (isDecl)
		{
			*mOut << ";" << end();
		}
		else
		{
			*mOut << end() << in() << "{" << end();
			++mIndent;
			codeNested(aNode);
			--mIndent;
			*mOut << in() << "}" << end() << end();
		}
	}
}


std::string NateCode::createCodeDecl(const DefinePtr& aDefine, const std::string& aObjectName)
{
	std::ostringstream out;

	if (aDefine->is(Define::None))
	{
		out << "void ";
	}
	else
	{
		out << aDefine->type()->codeType();
		if (aDefine->is(Method::Ref))
		{
			out << "&";
		}
		out << " ";
	}

	if (!aObjectName.empty())
	{
		out << aObjectName << "::";
	}

	out << aDefine->pattern();
	out << createCodeDeclArgs(aDefine, aDefine->args());
	
	if (aDefine->is(Define::ConstMethod) && !aDefine->is(Method::Ref))
	{
		out << " const";
	}

	return out.str();
}

std::string NateCode::createCodeDeclArgs(const DefinePtr& aDefine, const std::vector<Arg>& aArgs)
{
	std::ostringstream out;

	bool first = true;
	
	out << "(";

	for (auto const& arg : aDefine->args())
	{
		if (arg.isIdentifier() && !arg.identifier()->isObjectMe())
		{
			if (!first)
			{
				out << ", ";
			}

			first = false;

			if (arg.identifier()->type()->is(Type::NeedsRef) && !arg.is(Arg::Out))
			{
				out << "const ";
			}

			out << arg.identifier()->type()->codeType();

			if (arg.identifier()->type()->is(Type::NeedsRef) || arg.is(Arg::Out))
			{
				out << "&";
			}

			out << " " << arg.identifier()->codeName();
		}
	}

	out << ")";

	return out.str();
}

void NateCode::codeReturn(const TreeNodePtr& aNode)
{
	printLineNr(aNode->location);
	*mOut << in() << "return " << codeExpr(/*aNode, */aNode->expr) << ";" << end();
}

void NateCode::codeDeclObject(const TreeNodePtr& aNode)
{
	printLineNr(aNode->location);
	ObjectPtr object = aNode->object;
	codeObjectBases(object);

	*mOut << end();
	*mOut << in() << "{" << end();
	auto name = toCodeName(object->name());
	
	*mOut << in() << "public:" << end();
	
	++mIndent;
	if (object->isRole())
	{
		*mOut << in() << "virtual ~" << name << "() = default;" << end();
	}
	else
	{
		*mOut << in() << "virtual ~" << name << "();" << end();
		*mOut << in()  << name << "();" << end();
		*mOut << in(-1) << "private:" << end();
		*mOut << in() << "class __impl;"  << end();
		*mOut << in() << "__impl* _impl;"  << end();
		*mOut << in() << "friend class __impl;"  << end();
	  *mOut << in() << "public:" << end();
	}

	codeNested(aNode);
	codeDeclProperties(object);

	--mIndent;
	*mOut << in() << "};" << end() << end();
}

void NateCode::codeObjectBases(const ObjectPtr& aObject)
{
	*mOut << in() << "class " << toCodeName(aObject->name());

	if (aObject->getBases().empty() && !aObject->isRole())
	{
		*mOut << ": public std::enable_shared_from_this<" << toCodeName(aObject->name()) << ">";
	}
	else
	{
		bool first = true;
		for (auto const& base : aObject->getBases())
		{
			*mOut << (first ? ": public " : ", public ") << 
						   (base->isRole() ? "virtual " : "") <<
				       toCodeName(base->name());
			first = false;
		}
	}
}

void NateCode::codeDeclObjectDefine(const TreeNodePtr& aNode)
{
	printLineNr(aNode->location);

	DefinePtr defyne = aNode->defyne;
	const char* startKeys = defyne->is(Method::Final) || defyne->is(Method::Overriden)
												  ? "" : "virtual ";
	const char* endKeys = defyne->is(Method::Overriden)
												? " override" : "";
	const char* abstract = aNode->object->isRole()
												 ? " = 0" : "";
	*mOut << in() << (defyne->isStatic() ? "static " : startKeys) << 
									  createCodeDecl(defyne) << endKeys << abstract << ";" << end();
}

void NateCode::codeImplObject(const TreeNodePtr& aNode)
{
	printLineNr(aNode->location);
	ObjectPtr object = aNode->object;

	if (object->is(Type::ObjectImpl))
	{
		codeObjectBases(object);
		*mOut << in() << end();
		*mOut << in() << "{" << end();
		*mOut << in() << "public:" << end();
		++mIndent;
		codeNested(aNode);
		*mOut << in(-1) << "private:" << end();
		codeImplObjectVariables(aNode);

		--mIndent;
		*mOut << in() << "};" << end() << end();
	}
	else
	{
		auto name = toCodeName(object->name());
		*mOut << in() << "class " << name << "::__impl" << end();
		*mOut << in() << "{" << end();
		*mOut << in() << "private:" << end();
		*mOut << in(1) << "friend class " << name << ";" << end();
		*mOut << in(1) << name << "* me;" << end();
		++mIndent;
		codeImplObjectVariables(aNode);
		*mOut << in(-1) << "public:" << end();
		*mOut << in() << "__impl(" << name << "* aMe) : me(aMe) {}" << end();
		codeImplObjectNested(aNode, true);
		--mIndent;
		*mOut << in() << "};" << end() << end();

		*mOut << in() << name << "::" << name << "()" << end();
		*mOut << in(1) << ": _impl(new __impl(this)) {}" << end();
		*mOut << in() << name << "::~" << name << "() { delete _impl; }" << end();
		codeImplObjectNested(aNode, false);
	}
	
	codeDefaultProperties(object);
}

void NateCode::codeImplObjectVariables(const TreeNodePtr& aNode)
{
  for (auto& part : aNode->nested)
  {
    switch (part->code)
    {
    case ByteCode::LocalVar:
      codeLocalVar(part, true);
      break;
		default:
			break;
    }
  }
}

void NateCode::codeImplObjectNested(const TreeNodePtr& aNode, bool inImpl)
{
  for (auto& part : aNode->nested)
  {
    switch (part->code)
    {
    case ByteCode::Define:
		{
			DefinePtr defyne = part->defyne;
	
			if (defyne->isObjectMethod() && inImpl != part->bool1)
			{				
				codeDefine(part);
			}
      break;
		}
    case ByteCode::Prop:
			if (!inImpl)
			{
				codeProp(part);
			}
      break;
		default:
			break;
    }
  }
}

void NateCode::codeProp(const TreeNodePtr& aNode)
{
	printLineNr(aNode->location);
	
	bool addObjectName = !aNode->object->is(Type::ObjectImpl);
	*mOut << in() << codePropHeader(aNode->object, addObjectName, aNode->id, 
																	aNode->bool1  ? Object::PropType::Get : Object::PropType::Set) << end() 
			  << in() << "{" << end() ;

	++mIndent;
	codeNested(aNode);
	--mIndent;
	*mOut << in() << "}" << end() << end();
}

std::string NateCode::codePropHeader(const ObjectPtr& aObject,
																	   bool aAddObjectName,
																		 const IdentifierPtr& aId, 
																		 Object::PropType aPropType)
{
	std::stringstream buf;
	
	auto scopeName = aId->type()->typeScopeName();
	std::string propType = (aId->type()->is(Type::NeedsRef))
											    ? scopeName + aId->type()->codeType()
			                    : aId->type()->codeType();
	std::string refType = (aId->type()->is(Type::NeedsRef))
											    ? "&"
			                    : "";
	std::string objectPrefix = aAddObjectName
													   ? toCodeName(aObject->name()) + "::"
														 : "";

	if (aPropType == Object::PropType::Get)
	{
	  buf << propType << " " << objectPrefix << aId->codeName();
		buf << "_get() const";
	}
	else
	{
	  buf << "const " <<  propType << refType << " " << objectPrefix << aId->codeName();
		buf << "_set(const " << propType << refType << " value)";
	}

	return buf.str();
}

void NateCode::codeDefaultProperties(const ObjectPtr& aObject)
{
	for (const auto& propMethod : aObject->propertyMethods())
	{
		codeDefaultPropertyImpl(aObject, propMethod.first);
	}
}

void NateCode::codeDefaultPropertyImpl(const ObjectPtr& aObject,
																	     const IdentifierPtr& propId)
{
	bool addObjectName = !aObject->is(Type::ObjectImpl);
	if (aObject->getPropState(propId, Object::PropType::Get).state == Object::PropState::State::Declared)
	{
		*mOut << in() << codePropHeader(aObject, addObjectName, propId, Object::PropType::Get) 
				  << " { return " << propId->codeName() << "; }" << end();
	}

	if (aObject->getPropState(propId, Object::PropType::Set).state == Object::PropState::State::Declared)
	{
		*mOut << in() << codePropHeader(aObject, addObjectName, propId, Object::PropType::Set) 
				  << " { return " << propId->codeName() << " = value; }" << end();
	}
}

void NateCode::codeDeclProperties(const ObjectPtr& aObject)
{
	for (auto& propMethod : aObject->propertyMethods())
	{
		Location location(propMethod.second.location, propMethod.second.filename);
		codeDeclareProperty(aObject, propMethod.first, location);
	}
}

void NateCode::codeDeclareProperty(const ObjectPtr& aObject,
																	 const IdentifierPtr& aId,
																	 const Location& aLocation)
{
	TypePtr idType = aId->type();
	std::string propType = (idType->is(Type::NeedsRef))
											    ? idType->codeType() + "&"
			                    : idType->codeType();
	std::string declType = idType->codeType();
				
	if (!aObject->isRole())
	{
		*mOut << in(-1) << "private:" << end();
		*mOut << in() << declType << " " << aId->codeName() << " = {};" << end();
	}
		
	const char* permisKey = "public";
	*mOut << in(-1) << permisKey << ":" << end();

	bool overriden = aObject->basesIsPropDeclared(aId, Object::PropType::Get);

	const char* startKeys = aId->is(Identifier::Final) || overriden
			                    ? "" : "virtual ";
	const char* abstractKey = aObject->isRole()
			                      ? " = 0" : "";
	const char* endKeys = overriden
			                  ? " override" : "";
	*mOut << in() << startKeys << codePropHeader(aObject, false, aId, Object::PropType::Get) << endKeys << abstractKey << ";" << end();

	if (!aId->is(Identifier::ReadOnly))
	{
		overriden = aObject->basesIsPropDeclared(aId, Object::PropType::Set);
	  startKeys = aId->is(Identifier::Final) || overriden
			          ? "" : "virtual ";
		endKeys = overriden
			        ? " override" : "";
		*mOut << in() << startKeys << codePropHeader(aObject, false, aId, Object::PropType::Set) << endKeys << abstractKey << ";" << end();
	}
}

void NateCode::codeExprStat(const TreeNodePtr& aNode)
{
	printLineNr(aNode->location);
	*mOut << in() << codeExpr(/*aNode, */aNode->expr) << ";" << end();
}