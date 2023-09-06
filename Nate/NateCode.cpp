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

std::string NateCode::codeDesc(const TreeNodePtr& aNode)
{
	std::string result;
	switch (aNode->code)
	{
	case ByteCode::Code: result = "Code"; mIndent = -1; break;
	case ByteCode::Program: result = "Program"; break;
	case ByteCode::Expr: result = "Expr " + aNode->expr.code(); break;
	case ByteCode::Block: result = "Block"; break;
	case ByteCode::StdOutput: result = "StdOutput"; break;
	case ByteCode::StdError: result = "Error"; break;
	case ByteCode::StdInput: result = "Input"; break;
	case ByteCode::Read: result = "Read"; break;
	case ByteCode::Write: result = "Write"; break;
	case ByteCode::Data: result = "Data"; break;
	case ByteCode::SepComma: result = "OutputSepComma"; break;
	case ByteCode::SepConcat: result = "OutputSepConcat"; break;
	case ByteCode::LocalVar: result = "LocalVar " + aNode->id->name() + " " + aNode->id->type()->codeType() + "=" + aNode->id->initValue().code(); break;
	case ByteCode::Assign: result = "Assign"; break;
	case ByteCode::IfThen: result = "IfThen"; break;
	case ByteCode::If: result = "If"; break;
	case ByteCode::Else: result = "Else"; break;
	case ByteCode::ElseIf: result = "ElseIf"; break;
	case ByteCode::IfIs: result = "IfIs"; break;
	case ByteCode::CaseIsList: result = "CaseIsList"; break;
	case ByteCode::CaseIs: result = "CaseIs"; break;
	case ByteCode::ElseIs: result = "ElseIs"; break;
	case ByteCode::LoopStart: result = "LoopStart"; break;
	case ByteCode::LoopStartForStep: result = "LoopStartForStep"; break;
	case ByteCode::LoopStartForRange: result = "LoopStartForRange"; break;
	case ByteCode::While: result = "While"; break;
	case ByteCode::Scope: result = "Scope"; break;
	case ByteCode::CodeInclude: result = "CodeInclude"; break;
	case ByteCode::Record: result = "Record"; break;
	case ByteCode::Define: result = "Define"; break;
	case ByteCode::Return: result = "Return"; break;
	case ByteCode::DeclObject: result = "DeclObject"; break;
	case ByteCode::DeclObjectDefine: result = "DeclObjectDefine"; break;
	case ByteCode::ImplObject: result = "ImplObject"; break;
	case ByteCode::Prop: result = "Prop"; break;
	case ByteCode::ExprStat: result = "ExprStat " + aNode->expr.code(); break;
	case ByteCode::End: result = aNode->bool1 ? "End" : ""; break;
	default: result = "****"; break;
	}

	return result;
}

void NateCode::codeTreeDesc(const TreeNodePtr& aStat, std::ostream& out)
{
	std::string line = codeDesc(aStat);
	if (!line.empty())
	{
		out << in() << line << std::endl;
	}

	++mIndent;
  for (auto& stat : aStat->nested)
  {
		codeTreeDesc(stat, out);
	}
	--mIndent;
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
void NateCode::visit(const StatProgram& aStat)
{
	*mOut << in() << "#define NOMINMAX" << end();
	*mOut << in() << "#include <windows.h>" << end();

	printLineNr(aStat.getLocation());

	*mOut << in() << "int main(int argc, char** argv)\n" << in() << "{" << end();
	*mOut << in(1) << "output = std::shared_ptr<std::ostream>(&std::cout, [](void*) {});" << end();
	*mOut << in(1) << "error = std::shared_ptr<std::ostream>(&std::cerr, [](void*) {});" << end();
	*mOut << in(1) << "input = std::shared_ptr<std::istream>(&std::cin, [](void*) {});" << end();
	*mOut << in(1) << "SetConsoleOutputCP(65001);" << end();
	//*mOut << "std::locale::global(std::locale(\"en_US.UTF8\"));" << end();

	++mIndent;
	codeStats(aStat.getCompound());
	*mOut << in(-1) << "}" << end();
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

		*mOut << " = " << codeExpr(aStat.getValue());
	
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

	*mOut << aStat.getValue().code() << endPars << ";" << end();
}

void NateCode::visit(const StatExpr& aStat)
{
		*mOut << " = " << codeExpr(aStat.getExpr());
}

void NateCode::visit(const StatOutput& aStat)
{
	printLineNr(aStat.getLocation());
	mStream = "*output";
	mFirstOutput = true;
	mStartOutput = true;
	mDataOutput = false;
	mCachedOutput.clear();

  for (auto& part : aStat.getCompound())
  {
		part->accept(this);
  }
}

void NateCode::visit(const StatOutputComma& aStat)
{
	codeOutput("\" \"");
}

void NateCode::visit(const StatOutputConcat& aStat)
{
	// nothing
}

void NateCode::visit(const StatOutputEnd& aStat)
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

void NateCode::visit(const StatOutputExpr& aStat)
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

//void NateCode::code(const Stat::SPtr& aStat)
//{
//  switch (aStat->code)
//  {
//  case ByteCode::StdError:
//    codeOutput("*error", aStat);
//    break;
//  case ByteCode::StdInput:
//    codeInput("*input", aStat);
//    break;
//  case ByteCode::Data:
//    codeData(aStat);
//    break;
//  case ByteCode::Write:
//    codeWrite(aStat);
//    break;
//  case ByteCode::Read:
//    codeRead(aStat);
//    break;
//  case ByteCode::IfThen:
//    codeIfThen(aStat);
//    break;
//  case ByteCode::IfIs:
//    codeIfIs(aStat);
//    break;
//  case ByteCode::LoopStart:
//    codeStartLoop(aStat);
//    break;
//  case ByteCode::LoopStartForStep:
//    codeStartLoopForStep(aStat);
//    break;
//  case ByteCode::LoopStartForRange:
//    codeStartLoopForRange(aStat);
//    break;
//  case ByteCode::While:
//    codeWhile(aStat);
//    break;
//  case ByteCode::Scope:
//    codeScope(aStat);
//    break;
//  case ByteCode::CodeInclude:
//    codeCodeInclude(aStat);
//    break;
//  case ByteCode::Record:
//    codeRecord(aStat);
//    break;
//  case ByteCode::Define:
//		codeDefine(aStat);
//    break;
//  case ByteCode::Return:
//    codeReturn(aStat);
//    break;
//  case ByteCode::DeclObject:
//    codeDeclObject(aStat);
//    break;
//  case ByteCode::DeclObjectDefine:
//    codeDeclObjectDefine(aStat);
//    break;
//  case ByteCode::ImplObject:
//    codeImplObject(aStat);
//    break;
//  case ByteCode::Prop:
//    codeProp(aStat);
//    break;
//  case ByteCode::ExprStat:
//    codeExprStat(aStat);
//    break;
//	default:
//		std::cerr << "Bad bytecode " << (int)aStat->code << std::endl;
//		break;
//  }
//}

char NateCode::end()
{
	++mPrevLine;
	return '\n';
}

void NateCode::codeData(const TreeNodePtr& aNode)
{
	printLineNr(aNode->location);
	mDataOutput = true;
	std::string name = aNode->id->codeName() + "_temp";
	*mOut << in() << "std::ostringstream " << name << ";" << end();
	//codeOutput(name, aNode);
	*mOut << in() << "const " << aNode->id->type()->codeType() << " " << aNode->id->codeName() << "= " << name << ".str();" << end();

}

void NateCode::codeWrite(const TreeNodePtr& aNode)
{
	printLineNr(aNode->location);
	mDataOutput = false;
	if (aNode->bool1)
	{
    //codeDeclIdentifier(aNode, false, aNode->id, true, aNode->location);
	}
	else if (!aNode->expr.isEmpty())
	{
		*mOut << in() << "nate__writer = " << codeExpr(/*aNode, */aNode->expr) << ";" << end();
	}

	//codeOutput("*nate__writer", aNode);
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
			*mOut << " << " << aString << (mDataOutput ? "" : "; ");
			mFirstOutput = true;
		}
		else
		{
			*mOut << (mDataOutput ? "" : "; ");
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
			*mOut << " << " << aString << (mDataOutput ? "" : "; ");
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
	if (!aNode->id->is(Identifier::ObjectImpl) || inImplObject)
	{
    codeDeclIdentifier(aNode, false, aNode->id, aNode->bool1, aNode->location);
	}
}

void NateCode::codeDeclIdentifier(const TreeNodePtr& aNode, 
																	bool aExtern,
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
		*mOut << " = " << codeExpr(/*aNode, */aIdentifier->initValue());
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

void NateCode::codeIfThen(const TreeNodePtr& aNode)
{
  for (auto& part : aNode->nested)
  {
    switch (part->code)
    {
    case ByteCode::If:
      codeIf(part);
      break;
    case ByteCode::ElseIf:
      codeElseIf(part);
      break;
    case ByteCode::Else:
      codeElse(part);
      break;
		default:
			std::cerr << "Bad Output bytecode " << (int)part->code << std::endl;
			break;
    }
	}

	--mIndent;
	printLineNr(aNode->location);
	*mOut << in() << "}" << end();
}

void NateCode::codeIf(const TreeNodePtr& aNode)
{
	printLineNr(aNode->location);

	*mOut << in() << "if (" << codeExpr(/*aNode, */aNode->expr) << ")" << end() << in() << "{" << end();
	++mIndent;
	codeNested(aNode);
}

void NateCode::codeElseIf(const TreeNodePtr& aNode)
{
	--mIndent;
	*mOut << in() << "}" << end();
	printLineNr(aNode->location);
	*mOut << in() << "else if (" << codeExpr(/*aNode, */aNode->expr) << ")" << end() << in() << "{" << end();
	++mIndent;
	codeNested(aNode);
}

void NateCode::codeElse(const TreeNodePtr& aNode)
{
	--mIndent;
	*mOut << in() << "}" << end();
	printLineNr(aNode->location);
	*mOut << in() << "else" << end() << in() << "{" << end();
	++mIndent;
	codeNested(aNode);
}

bool NateCode::isConstIntScalar(const Expr& aExpr)
{
  return (aExpr.is(Expr::ConstExpr) && aExpr.type()->is(Type::Scalar) &&
		      !aExpr.type()->is(Type::Real));
}

bool NateCode::isNestedConstIntScalar(const TreeNodePtr& aNode)
{
	return std::all_of(aNode->nested.begin(), aNode->nested.end(),
										[&](const TreeNodePtr& part) { return part->code != ByteCode::CaseIs || isConstIntScalar(part->expr); });
}

bool NateCode::isNestedNonConstIntScalar(const TreeNodePtr& aNode)
{
	return std::any_of(aNode->nested.begin(), aNode->nested.end(),
										[&](const TreeNodePtr& part) { return part->code == ByteCode::CaseIs && !isConstIntScalar(part->expr); });
}

void NateCode::codeIfIs(const TreeNodePtr& aNode)
{
	bool firstIf = true;
	bool usesIf = false;
	bool usesSwitch = false;
	TreeNodePtr elsePart;
	
	printLineNr(aNode->location);
	*mOut << in() << "auto const " << aNode->id->codeName() << " = " << codeExpr(/*aNode, */aNode->expr) << ";" << end();

  for (auto& part : aNode->nested)
  {
    if (part->code == ByteCode::CaseIsList)
    {
      bool needsIf = codeCaseIsListIf(part, aNode, firstIf);
			usesIf = needsIf || usesIf;
      bool needsSwitch = isNestedConstIntScalar(part) && !part->nested.empty();
			usesSwitch = needsSwitch || usesSwitch;
    }
		else
		{
			elsePart = part;
		}
  }

	if (usesIf)
	{
		if (usesSwitch || elsePart)
		{
			*mOut << in() << "else" << end();
			*mOut << in() << "{" << end();
			if (usesSwitch)
			{
				++mIndent;
				codeSwitch(aNode, elsePart);
				--mIndent;
			}
			else
			{
				codeElseIs(elsePart);
			}

			*mOut << in() << "}" << end();
		}
	}
	else if (usesSwitch)
	{
		codeSwitch(aNode, elsePart);
	}
}

bool NateCode::codeCaseIsListIf(const TreeNodePtr& aNode, const TreeNodePtr& aIfIsNode, bool& firstIf)
{
	bool firstCond = true;
	bool result = isNestedNonConstIntScalar(aNode);

	if (result && !aNode->nested.empty())
	{
	  printLineNr(aNode->location);
		*mOut << in() << (firstIf ? "if " : "else if ") << "(";
		for (auto& part : aNode->nested)
		{
			if (part->code == ByteCode::CaseIs)
			{
				codeCaseIsIf(part, aIfIsNode, firstCond);
				firstCond = false;
			}
		}

		*mOut << ")" << end() << in() << "{" << end();
		++mIndent;
		for (auto& part : aNode->nested)
		{
			if (part->code != ByteCode::CaseIs)
			{
				//code(part);
			}
		}

		--mIndent;
		*mOut << in() << "}" << end();

		firstIf = false;
	}

	return result;
}

void NateCode::codeCaseIsIf(const TreeNodePtr& aNode, const TreeNodePtr& aIfIsNode, bool firstCond)
{
	if (!firstCond)
	{
		*mOut << end();
	  printLineNr(aNode->location);
		*mOut << end() << in(4) << " || ";
	}

	*mOut << "(" << aIfIsNode->id->codeName() << " == " << codeExpr(/*aNode, */aNode->expr) << ")";
}

void NateCode::codeSwitch(const TreeNodePtr& aNode, const TreeNodePtr& aElsePart)
{
	printLineNr(aNode->location);
	*mOut << in() << "switch (" + aNode->id->codeName() << ")" << end();
	*mOut << in() << "{" << end();
	
  for (auto& part : aNode->nested)
  {
    if (part->code == ByteCode::CaseIsList)
    {
      codeCaseIsListSwitch(part);
    }
  }
	
	*mOut << in() << "default:" << end();
	*mOut << in() << "{" << end();
	codeElseIs(aElsePart);
	*mOut << in(1) << "break;" << end();
	*mOut << in() << "}" << end();
	*mOut << in() << "}" << end();
}

void NateCode::codeCaseIsListSwitch(const TreeNodePtr& aNode)
{
	if (isNestedConstIntScalar(aNode) && !aNode->nested.empty())
	{
		for (auto& part : aNode->nested)
		{
			if (part->code == ByteCode::CaseIs && isConstIntScalar(part->expr))
			{
				codeCaseIsSwitch(part);
			}
		}

		*mOut << in() << "{" << end();
		++mIndent;
		for (auto& part : aNode->nested)
		{
			if (part->code != ByteCode::CaseIs)
			{
				//code(part);
			}
		}

		--mIndent;
		*mOut << in(1) << "break;" << end();
		*mOut << in() << "}" << end();
	}
}

void NateCode::codeCaseIsSwitch(const TreeNodePtr& aNode)
{
	printLineNr(aNode->location);
	*mOut << in() << "case " << codeExpr(/*aNode, */aNode->expr) << ":" << end();
}

void NateCode::codeElseIs(const TreeNodePtr& aNode)
{
	++mIndent;
	codeNested(aNode);
	--mIndent;
}

void NateCode::codeStartLoop(const TreeNodePtr& aNode)
{
	printLineNr(aNode->location);
	*mOut << in() << "while (true)" << end() << in() << "{" << end();
	++mIndent;
	codeNested(aNode);
	--mIndent;
	*mOut << in() << "}" << end();
}

void NateCode::codeStartLoopForStep(const TreeNodePtr& aNode)
{
	printLineNr(aNode->location);

	*mOut << in() << "for (" << aNode->id->type()->codeType() << " " 
			 << aNode->id->codeName() << "= " << codeExpr(/*aNode, */aNode->expr) << ";"  
			 << aNode->id->name() << (aNode->bool1 ? " >= " : "<=") << codeExpr(/*aNode, */aNode->expr2) << "; "
			 << aNode->id->name() << (aNode->bool1 ? " -= " : "+=") << codeExpr(/*aNode, */aNode->expr3) << ")" << end()
		   << in() << "{" << end();
	++mIndent;
	codeNested(aNode);
	--mIndent;
	*mOut << in() << "}" << end();
}

void NateCode::codeStartLoopForRange(const TreeNodePtr& aNode)
{
	TypePtr rangeType = aNode->expr.type();
	auto range = mParser->uniqueName();
	auto iter = mParser->uniqueName();
	auto next = mParser->uniqueName();

	std::string ref = aNode->expr.is(Expr::Output) ? "&" : "";
	std::string increment;

	if (rangeType->isOfType("text"))
	{
		increment = iter + "=" + next;
		*mOut << in() << "const std::string" << ref << " " << range << " = " << codeExpr(/*aNode, */aNode->expr) << ";" << end(); 
		*mOut << in() << "auto " << next << " = " << range << ".cbegin();" << end();
	}
	else
	{
		increment = "++" + iter;
		*mOut << in() << "auto const" << ref << " " << range << " = " << codeExpr(/*aNode, */aNode->expr) << ";" << end(); 
	}

	*mOut << in() << "for (auto " << iter << " = " << range << ".cbegin(); "
			 << iter << " != " << range << ".cend(); "
			 << increment << ")" << end() << in() << "{" << end();

	++mIndent;

	if (rangeType->isOfType("text"))
	{
		*mOut << in() << "uint32_t " << aNode->id->codeName() << " = utf8::next(" + next + "," + range + ".cend());" << end();
	}
	else
	{
		*mOut << in() << "auto const& " << aNode->id->codeName() << " = *" << iter << ";" << end();
	}

	codeNested(aNode);
	--mIndent;
	*mOut << in() << "}" << end();
}

void NateCode::codeWhile(const TreeNodePtr& aNode)
{
	printLineNr(aNode->location);
	*mOut << in() << "if (!(" << codeExpr(/*aNode, */aNode->expr) << ")) break;" << end();
}

void NateCode::codeScope(const TreeNodePtr& aNode)
{
	printLineNr(aNode->location);
	*mOut << in() << "{" << end();
	++mIndent;
	codeNested(aNode);
	--mIndent;
	*mOut << in() << "}" << end();
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