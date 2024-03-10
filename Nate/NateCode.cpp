#include "NateCode.h"
#include "NateParser.h"
#include "NateFunctions.h"
#include "Identifier.h"
#include "Property.h"

#include <algorithm>

namespace nate
{
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

void NateCode::codeCompound(const Stat& aStat)
{
	*mOut << in() << "{" << end();
	++mIndent;
	codeStats(aStat.getCompound());
	--mIndent;
	*mOut << in() << "}" << end();
}

void NateCode::codeBlock(const Stat& aStat)
{
	*mOut << in() << "{" << end();
	++mIndent;
	aStat.accept(this);
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
	//if (!aStat.getIdentifier()->is(Identifier::ObjectImpl))
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
				codeBlock(*elsePart);
			}

			*mOut << in() << "break;" << end();
		}
		
		*mOut << in() << "}" << end();
	}
	else
	{
		codeIfIsIfs(aStat, elsePart);
	}
}

void NateCode::visit(const StatIfIs::Else& aStat)
{
	codeStats(aStat.getCompound());
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
	printLineNr(aStat.getLocation());
	*mOut << in() << codeExpr(aStat.getExpr()) << ";" << end();
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
		codeOutput("std::endl");
	}
	else
	{
		codeOutput("");
	}
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

void NateCode::codeOutputStart(const StatOutput& aStat, const std::string& aOutput, bool aDataOutput)
{
	printLineNr(aStat.getLocation());
	mStream = aOutput;
	mDataOutput = aDataOutput;
	mCachedOutput.clear();
	
	*mOut << in() << mStream;

  for (auto& part : aStat.getCompound())
  {
		part->accept(this);
  }

	*mOut << ";" << end();
}

char NateCode::end()
{
	++mPrevLine;
	return '\n';
}

void NateCode::codeOutput(const std::string& aString)
{
	if ((!mCachedOutput.empty()) && aString[0] != '"')
	{
		*mOut << " << \"" << mCachedOutput << "\"";
		mCachedOutput.clear();
		if (!aString.empty())
		{
			*mOut << " << " << aString;
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
			*mOut << " << " << aString;
		}
	}
}

void NateCode::visit(const StatInput& aStat)
{
	codeInputStart(aStat, "*input");
}

void NateCode::visit(const StatInput::Comma& aStat)
{
	mLastInputComma = true;
}

void NateCode::visit(const StatInput::Concat& aStat)
{
	mLastInputComma = false;
}

void NateCode::visit(const StatInput::End& aStat)
{
	if (aStat.getEndOfLine() && mCurInputType == InputType::Normal)
	{
		*mOut << in() << "(" << mStream << ").ignore(std::numeric_limits<std::streamsize>::max(), '\\n');" << end();
	}
}

void NateCode::visit(const StatInput::Value& aStat)
{
	InputType inputType = mCurInputType;
	std::string skipSpaces = mLastInputComma ? " >> std::ws" : "";

	if (!mNextInputEnd || !aStat.getExpr().type()->is(Type::Text)) {
			inputType = InputType::Normal;			
	}

	if (inputType == InputType::Line)
	{
		*mOut << in() << "std::getline(" << mStream << skipSpaces << ", " << codeExpr(aStat.getExpr()) << ");" << end();
	}
	else if (inputType == InputType::All)
	{
	}
	else
	{
		if (aStat.getExpr().type()->is(Type::Boolean))
		{
			*mOut << in() << mStream << skipSpaces << " >> std::boolalpha  >> " << codeExpr(aStat.getExpr()) << " >> std::noboolalpha;" << end();
		}
		else
		{
			*mOut << in() << mStream << skipSpaces << " >> " << codeExpr(aStat.getExpr()) << ";" << end();
		}
	}
}

void NateCode::visit(const StatRead& aStat)
{
	if (aStat.getCreateIt())
	{
    codeDeclIdentifier(false, aStat.getReader(), true, aStat.getLocation());
	}
	else if (!aStat.getInput().isEmpty())
	{
		*mOut << in() << aStat.getReader()->codeName() << " = " << codeExpr(aStat.getInput()) << ";" << end();
	}

	codeInputStart(aStat, "*" + aStat.getReader()->codeName(), aStat.getInputType());
}

void NateCode::visit(const StatError& aStat)
{
	codeOutputStart(aStat, "*error");
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

void NateCode::visit(const StatRecord& aStat)
{
	printLineNr(aStat.getLocation());
	
	*mOut << in() << "struct " << aStat.getRecord()->codeType() << end() << in() << "{" << end();
	++mIndent;
	*mOut << in() << aStat.getRecord()->codeType() << "()" << end();
	bool first = true;

	for (auto& id : aStat.getRecord()->identifiers().get())
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
			*mOut << codeExpr(id->initValue());
		}

		*mOut << ")" << end();
	}

	*mOut << in() << "{}" << end();
	for (auto& id : aStat.getRecord()->identifiers().get())
	{
    codeDeclIdentifier(false, id, false, aStat.getLocation());
	}

	--mIndent;
	*mOut << in() << "};" << end() << end();
}

void NateCode::visit(const StatDefine& aStat)
{
	codeDefine(aStat);
}

void NateCode::codeDefine(const StatDefine& aStat, const ObjectPtr& aObject)
{
	printLineNr(aStat.getLocation());

	DefinePtr defyne = aStat.getDefine();
	ObjectPtr object = aObject ? aObject : defyne->object();
	
	if (defyne->isObjectMethod())
	{		
		if (aStat.isImpOnly())
		{
			if (aStat.isDecl())
			{
				*mOut << in() << (defyne->isStatic() ? "static " : "") <<
													createCodeDecl(defyne) << ";" << end();
			}
			else
			{
				*mOut << in() << (defyne->isStatic() ? "static " : "") <<
													createCodeDecl(defyne) << end();
				codeCompound(aStat);
			}
		}
		else
		{
			if (aStat.isDecl())
			{
				const char* startKeys = defyne->is(Method::Final) || defyne->is(Method::Overriden)
																? "" : "virtual ";
				const char* endKeys = defyne->is(Method::Overriden)
															? " override" : "";
				const char* abstract = object->isRole()
																? " = 0" : "";
				*mOut << in() << (defyne->isStatic() ? "static " : startKeys) << 
													createCodeDecl(defyne) << endKeys << abstract << ";" << end();
			}
			else
			{
				*mOut << in() << (defyne->isStatic() && aStat.isImpOnly() ? "static " : "") <<
													createCodeDecl(defyne, toCodeName(object->name())) << end();

				codeCompound(aStat);
			}
		}
	}
	else
	{
		if (aStat.isDecl())
		{
			*mOut << in() << createCodeDecl(defyne) << ";" << end();
		}
		else
		{
			*mOut << in() << createCodeDecl(defyne) << end();
			codeCompound(aStat);
		}
	}
}

void NateCode::visit(const StatReturn& aStat)
{
	printLineNr(aStat.getLocation());
	*mOut << in() << "return " << codeExpr(aStat.getExpr()) << ";" << end();
}

void NateCode::visit(const StatScope& aStat)
{
	printLineNr(aStat.getLocation());
	codeCompound(aStat);
}

void NateCode::codeInputStart(const StatInput& aStat, const std::string& aStream, InputType inputType)
{
	printLineNr(aStat.getLocation());
	mLastInputComma = false;
	mCurInputType = inputType;
	mStream = aStream;
	
	for (auto iter = aStat.getCompound().begin(); iter != aStat.getCompound().end(); ++iter)
  {
		auto next = std::next(iter);
		mNextInputEnd = (next != aStat.getCompound().end() && dynamic_cast<StatInput::End*>(next->get()) != nullptr);
		(*iter)->accept(this);
  }
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

std::string NateCode::codeExpr(const Expr& aValue)
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
		codeBlock(*aElsePart);
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
		
		--mIndent;
		*mOut << in() << "}" << end();
		*mOut << in() << "break;" << end();
	}
}

void NateCode::codeCaseIsSwitch(const StatIfIs::Is& aStat)
{
	printLineNr(aStat.getLocation());
	*mOut << in() << "case " << codeExpr(aStat.getExpr()) << ":" << end();
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

void NateCode::visit(const StatProperty& aStat)
{
	printLineNr(aStat.getLocation());
	
	const ObjectPtr object = aStat.getObject();

	for (auto& part : aStat.getCompound())
	{
		StatDefine* defyne = dynamic_cast<StatDefine*>(part.get());
		Property* property = dynamic_cast<Property*>(defyne->getDefine().get());

		*mOut << in() << codePropHeader(object, true, aStat.getId(), property->getPropType()) << end();
		codeCompound(*defyne);
	}
}

std::string NateCode::codePropHeader(const ObjectPtr& aObject,
																	   bool aAddObjectName,
																		 const IdentifierPtr& aId, 
																		 Property::PropType aPropType)
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

	if (aPropType == Property::PropType::Get)
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
		codeDefaultPropertyImpl(aObject, propMethod.first, propMethod.second);
	}
}

void NateCode::codeDefaultPropertyImpl(const ObjectPtr& aObject,
																			 const IdentifierPtr& propId,
																	     const Object::PropData& propdata)
{
	bool addObjectName = !aObject->is(Type::ObjectImpl);
	if (propdata.states[static_cast<int>(Property::PropType::Get)].state == Object::PropState::State::Declared)
	{
		*mOut << in() << codePropHeader(aObject, addObjectName, propId, Property::PropType::Get) 
				  << " { return " << propId->codeName() << "; }" << end();
	}

	if (propdata.states[static_cast<int>(Property::PropType::Set)].state == Object::PropState::State::Declared)
	{
		*mOut << in() << codePropHeader(aObject, addObjectName, propId, Property::PropType::Set) 
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

	bool overriden = aObject->basesIsPropDeclared(aId, Property::PropType::Get);

	const char* startKeys = aId->is(Identifier::Final) || overriden
			                    ? "" : "virtual ";
	const char* abstractKey = aObject->isRole()
			                      ? " = 0" : "";
	const char* endKeys = overriden
			                  ? " override" : "";
	*mOut << in() << startKeys << codePropHeader(aObject, false, aId, Property::PropType::Get) << endKeys << abstractKey << ";" << end();

	if (!aId->is(Identifier::ReadOnly))
	{
		overriden = aObject->basesIsPropDeclared(aId, Property::PropType::Set);
	  startKeys = aId->is(Identifier::Final) || overriden
			          ? "" : "virtual ";
		endKeys = overriden
			        ? " override" : "";
		*mOut << in() << startKeys << codePropHeader(aObject, false, aId, Property::PropType::Set) << endKeys << abstractKey << ";" << end();
	}
}

void NateCode::visit(const StatObject& aStat)
{
	printLineNr(aStat.getLocation());
	
	ObjectPtr object = aStat.getObject();

  if (aStat.isDecl())
	{
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
			*mOut << in(-1) << "public:" << end();
		}
		
		codeStats(aStat.getCompound());
		codeDeclProperties(object);

		--mIndent;
		*mOut << in() << "};" << end() << end();
	}
	else
	{
		if (object->is(Type::ObjectImpl))
		{
			codeObjectBases(object);
			*mOut << in() << end();
			*mOut << in() << "{" << end();
			*mOut << in() << "public:" << end();
			++mIndent;
			codeStats(aStat.getCompound());
			*mOut << in(-1) << "private:" << end();
			codeImplObjectVariables(aStat);

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
			codeImplObjectVariables(aStat);
			*mOut << in(-1) << "public:" << end();
			*mOut << in() << "__impl(" << name << "* aMe) : me(aMe) {}" << end();
			codeImplObjectNested(aStat, true);
			--mIndent;
			*mOut << in() << "};" << end() << end();

			*mOut << in() << name << "::" << name << "()" << end();
			*mOut << in(1) << ": _impl(new __impl(this)) {}" << end();
			*mOut << in() << name << "::~" << name << "() { delete _impl; }" << end();
			codeImplObjectNested(aStat, false);
		}
	
		codeDefaultProperties(object);
	}
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
			if (!base->is(Type::NoCode))
			{
				*mOut << (first ? ": public " : ", public ") << 
								 (base->isRole() ? "virtual " : "") <<
								 toCodeName(base->name());
				first = false;
			}
		}
	}
}

void NateCode::codeImplObjectVariables(const StatObject& aStat)
{
  for (auto& stat : aStat.getCompound())
  {
		if (dynamic_cast<StatDeclareLocal*>(stat.get()) != nullptr)
		{
			stat->accept(this);
		}
	}
}

void NateCode::codeImplObjectNested(const StatObject& aStat, bool inImpl)
{
  for (auto& stat : aStat.getCompound())
  {
		StatDefine* defyne = dynamic_cast<StatDefine*>(stat.get());
		if (defyne != nullptr)
		{
			if (defyne->getDefine()->isObjectMethod() && inImpl == defyne->isImpOnly())
			{				
				codeDefine(*defyne, inImpl ? ObjectPtr() : aStat.getObject());
			}
		}
		else
		{
			StatProperty* property = dynamic_cast<StatProperty*>(stat.get());
			if (property != nullptr)
			{
				if (inImpl == property->isImpOnly())
				{				
					property->accept(this);
				}
			}
		}
	}
}

}
