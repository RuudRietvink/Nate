#include "NateCode.h"
#include "NateParser.h"
#include "NateFunctions.h"
#include "Identifier.h"

#include <algorithm>

NateCode::NateCode(std::ostream& aOut, NateParser* aParser)
  : mOut(aOut),
	  mParser(aParser)
{
}

std::string NateCode::in(int extra)
{
	return std::string(mIndent + extra, '\t');
}

void NateCode::printLineNr(const Location& aLocation)
{

	if (aLocation.beginLine != mPrevLine || 
			aLocation.filename != mPrevFile)
	{
		mOut << "#line " << aLocation.beginLine;
		if (mPrevFile != aLocation.filename)
		{
			mOut << " \"" << aLocation.filename << "\"";
		}
			
		mOut << std::endl;

		mPrevLine = aLocation.beginLine;
		mPrevFile = aLocation.filename;
	}
}

std::string NateCode::codeDesc(const TreeNodePtr& aNode)
{
	std::string result;
	switch (aNode->code)
	{
	case ByteCode::Code: result = "Code"; break;
	case ByteCode::Program: result = "Program"; break;
	case ByteCode::Expr: result = "Expr"; break;
	case ByteCode::Block: result = "Block"; break;
	case ByteCode::StdOutput: result = "StdOutput"; break;
	case ByteCode::OutputSepComma: result = "OutputSepComma"; break;
	case ByteCode::OutputSepConcat: result = "OutputSepConcat"; break;
	case ByteCode::LocalVar: result = "LocalVar"; break;
	case ByteCode::Assign: result = "Assign"; break;
	case ByteCode::If: result = "If"; break;
	case ByteCode::Else: result = "Else"; break;
	case ByteCode::ElseIf: result = "ElseIf"; break;
	case ByteCode::EndIf: result = "EndIf"; break;
	case ByteCode::IfIs: result = "IfIs"; break;
	case ByteCode::CaseIsList: result = "CaseIsList"; break;
	case ByteCode::CaseIs: result = "CaseIs"; break;
	case ByteCode::ElseIs: result = "ElseIs"; break;
	case ByteCode::LoopStart: result = "LoopStart"; break;
	case ByteCode::LoopStartForStep: result = "LoopStartForStep"; break;
	case ByteCode::LoopStartForRange: result = "LoopStartForRange"; break;
	case ByteCode::While: result = "While"; break;
	}

	if (aNode->id)
	{
		result += " " + aNode->id->codeName();
	}

	return result;
}

void NateCode::codeTreeDesc(const TreeNodePtr& aStat)
{
	std::cout << in() << codeDesc(aStat) << std::endl;
	++mIndent;
  for (auto& stat : aStat->nested)
  {
		codeTreeDesc(stat);
	}
	--mIndent;
}

void NateCode::codeNested(const TreeNodePtr& aStat)
{
  for (auto& stat : aStat->nested)
  {
		code(stat);
	}
}

void NateCode::code(const TreeNodePtr& aStat)
{
  switch (aStat->code)
  {
  case ByteCode::Program:
    codeProgram(aStat);
    break;
  case ByteCode::StdOutput:
    codeOutput("*output", aStat);
    break;
  case ByteCode::LocalVar:
    codeDeclIdentifier(false, aStat->id, aStat->bool1, aStat->location);
    break;
  case ByteCode::Assign:
    codeAssign(aStat);
    break;
  case ByteCode::Expr:
    mOut << codeExpr(aStat->expr);
    break;
  case ByteCode::If:
    codeIf(aStat);
    break;
  case ByteCode::ElseIf:
    codeElseIf(aStat);
    break;
  case ByteCode::Else:
    codeElse(aStat);
    break;
  case ByteCode::EndIf:
    codeEndIf(aStat);
    break;
  case ByteCode::IfIs:
    codeIfIs(aStat);
    break;
  case ByteCode::LoopStart:
    codeStartLoop(aStat);
    break;
  case ByteCode::LoopStartForStep:
    codeStartLoopForStep(aStat);
    break;
  case ByteCode::LoopStartForRange:
    codeStartLoopForRange(aStat);
    break;
  case ByteCode::While:
    codeWhile(aStat);
    break;
	default:
		std::cerr << "Bad bytecode " << (int)aStat->code << std::endl;
		break;
  }
}

char NateCode::end()
{
	++mPrevLine;
	return '\n';
}

void NateCode::codeProgram(const TreeNodePtr& aNode)
{
	mOut << in() << "#define NOMINMAX" << end();
	mOut << in() << "#include <windows.h>" << end();

	printLineNr(aNode->location);

	mOut << in() << "int main(int argc, char** argv)\n" << in() << "{" << end();
	mOut << in(1) << "output = std::shared_ptr<std::ostream>(&std::cout, [](void*) {});" << end();
	mOut << in(1) << "error = std::shared_ptr<std::ostream>(&std::cerr, [](void*) {});" << end();
	mOut << in(1) << "SetConsoleOutputCP(65001);" << end();
	//mOut << "std::locale::global(std::locale(\"en_US.UTF8\"));" << end();

	++mIndent;
	codeNested(aNode);
	mOut << in(-1) << "}" << end();
}

void NateCode::codeOutput(const std::string& aStream, const TreeNodePtr& aNode)
{
	printLineNr(aNode->location);
	mStream = aStream;
	mFirstOutput = true;
	mStartOutput = true;
	mDataOutput = false;
	mCachedOutput.clear();

  for (auto& part : aNode->nested)
  {
    switch (part->code)
    {
    case ByteCode::Expr:
      codeOutput(part->expr);
      break;
    case ByteCode::OutputSepComma:
      codeOutput("\" \"");
      break;
    case ByteCode::OutputSepConcat:
      break;
		default:
			std::cerr << "Bad Output bytecode " << (int)part->code << std::endl;
			break;
    }
  }

	codeOutputEnd(aNode->nested.empty() || aNode->nested.back()->code == ByteCode::Expr);
}

void NateCode::codeOutputNew()
{
	if (mFirstOutput && !mDataOutput)
	{
		if (mStartOutput)
		{
			mOut << in();
	    mStartOutput = false;
		}

		mOut << mStream;
		mFirstOutput = false;
	}
}

void NateCode::codeOutput(const std::string& aString)
{
	if ((!mCachedOutput.empty()) && aString[0] != '"')
	{
		codeOutputNew();
		mOut << " << \"" << mCachedOutput << "\"";
		mCachedOutput.clear();
		if (!aString.empty())
		{
			codeOutputNew();
			mOut << " << " << aString << (mDataOutput ? "" : ";");
			mFirstOutput = true;
		}
		else
		{
			mOut << (mDataOutput ? "" : ";");
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
			mOut << " << " << aString << (mDataOutput ? "" : ";");
			mFirstOutput = true;
		}
	}
}

void NateCode::codeOutput(const Expr& aValue)
{
	if (aValue.type() && aValue.type()->is(Type::Boolean))
	{
		codeOutput("std::boolalpha ");
	}
	
	if (aValue.is(ExprNode::Literal))
	{
		codeOutput(codeExpr(aValue));
	}
	else
	{
		if (aValue.type() && aValue.type()->name() == "int-8")
		{
			codeOutput("static_cast<int>(" + codeExpr(aValue) + ")");
		}
		else
		{
			Expr outExpr(Expr("stream-out"), aValue);
			Expr resExpr = mParser->evaluate(outExpr);
			if (!resExpr.isEmpty())
			{
				codeOutput("(" + codeExpr(resExpr) + ")");
			}
			else
			{
				codeOutput("(" + codeExpr(aValue) + ")");
			}
		}
	}
}

void NateCode::codeOutputEnd(bool aAddEnd)
{
	if (aAddEnd)
	{
		codeOutputNew();
		codeOutput("std::endl");
	}
	else
	{
		codeOutput("");
	}

	mOut << end();
}

void NateCode::codeDeclIdentifier(bool aExtern,
																	const IdentifierPtr& aIdentifier,
																	bool initializeVariables,
																	const Location& aLocation)
{
	printLineNr(aLocation);

	mOut << in();

	if (aExtern)
	{
	  mOut << "extern ";	
	}

	if (aIdentifier->is(Identifier::Const))
	{
		mOut << "const ";		
	}

	mOut << aIdentifier->type()->codeType() << " " << aIdentifier->codeName();

	if (initializeVariables)
	{
		mOut << " = " << codeExpr(*aIdentifier->initValue());
	}
	
	mOut << ";" << end();
}

std::string NateCode::codeExpr(const Expr& aValue)
{
	std::string result = aValue.code();
	if (aValue.is(ExprNode::Identifier) && !aValue.is(ExprNode::Property))
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
				/*	if (holder->scopeFlag() == IIdentifiersHolder::ScopeFlag::ObjectImpl)
					{
						if (curDefine() && !curDefine()->is(Define::Undeclared))
						{
							result = "_impl->" + result;
						}
					}*/
				}
			}
		}
	}

	return result;
}

void NateCode::codeAssign(const TreeNodePtr& aNode)
{
	std::string endPars;

	printLineNr(aNode->location);

	for (auto const& expr : aNode->exprList)
	{				
		std::string code = codeExpr(expr);
		size_t size = code.size();
		if (size > 6 && code.substr(size - 6, 6) == "_get()")
		{
			code[size - 5] = 's';
			code[size - 1] = '\0';
			endPars += ")";
			mOut << in() << code;
		}
		else
		{
			mOut << in() << code << " = ";
		}
	}

	mOut << codeExpr(aNode->expr) << endPars << ";" << end();
}

void NateCode::codeIf(const TreeNodePtr& aNode)
{
	printLineNr(aNode->location);

	mOut << in() << "if (" << codeExpr(aNode->expr) << ")" << end() << in() << "{" << end();
	++mIndent;
	codeNested(aNode);
}

void NateCode::codeElseIf(const TreeNodePtr& aNode)
{
	--mIndent;
	mOut << in() << "}" << end();
	printLineNr(aNode->location);
	mOut << in() << "else if (" << codeExpr(aNode->expr) << ")" << end() << in() << "{" << end();
	++mIndent;
	codeNested(aNode);
}

void NateCode::codeElse(const TreeNodePtr& aNode)
{
	--mIndent;
	mOut << in() << "}" << end();
	printLineNr(aNode->location);
	mOut << in() << "else" << end() << in() << "{" << end();
	++mIndent;
	codeNested(aNode);
}

void NateCode::codeEndIf(const TreeNodePtr& aNode)
{
	--mIndent;
	printLineNr(aNode->location);
	mOut << in() << "}" << end();
}

bool NateCode::isConstIntScalar(const Expr& aExpr)
{
  return (aExpr.is(ExprNode::ConstExpr) && aExpr.type()->is(Type::Scalar) &&
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
	mOut << in() << "auto const " << aNode->id->codeName() << " = " << codeExpr(aNode->expr) << ";" << end();

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
			mOut << in() << "else" << end();
			mOut << in() << "{" << end();
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

			mOut << in() << "}" << end();
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
		mOut << in() << (firstIf ? "if " : "else if ") << "(";
		for (auto& part : aNode->nested)
		{
			if (part->code == ByteCode::CaseIs)
			{
				codeCaseIsIf(part, aIfIsNode, firstCond);
				firstCond = false;
			}
		}

		mOut << ")" << end() << in() << "{" << end();
		++mIndent;
		for (auto& part : aNode->nested)
		{
			if (part->code != ByteCode::CaseIs)
			{
				code(part);
			}
		}

		--mIndent;
		mOut << in() << "}" << end();

		firstIf = false;
	}

	return result;
}

void NateCode::codeCaseIsIf(const TreeNodePtr& aNode, const TreeNodePtr& aIfIsNode, bool firstCond)
{
	if (!firstCond)
	{
		mOut << end();
	  printLineNr(aNode->location);
		mOut << end() << in(4) << " || ";
	}

	mOut << "(" << aIfIsNode->id->codeName() << " == " << codeExpr(aNode->expr) << ")";
}

void NateCode::codeSwitch(const TreeNodePtr& aNode, const TreeNodePtr& aElsePart)
{
	printLineNr(aNode->location);
	mOut << in() << "switch (" + aNode->id->codeName() << ")" << end();
	mOut << in() << "{" << end();
	
  for (auto& part : aNode->nested)
  {
    if (part->code == ByteCode::CaseIsList)
    {
      codeCaseIsListSwitch(part);
    }
  }
	
	mOut << in() << "default:" << end();
	mOut << in() << "{" << end();
	codeElseIs(aElsePart);
	mOut << in(1) << "break;" << end();
	mOut << in() << "}" << end();
	mOut << in() << "}" << end();
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

		mOut << in() << "{" << end();
		++mIndent;
		for (auto& part : aNode->nested)
		{
			if (part->code != ByteCode::CaseIs)
			{
				code(part);
			}
		}

		--mIndent;
		mOut << in(1) << "break;" << end();
		mOut << in() << "}" << end();
	}
}

void NateCode::codeCaseIsSwitch(const TreeNodePtr& aNode)
{
	printLineNr(aNode->location);
	mOut << in() << "case " << codeExpr(aNode->expr) << ":" << end();
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
	mOut << in() << "while (true)" << end() << in() << "{" << end();
	++mIndent;
	codeNested(aNode);
	--mIndent;
	mOut << in() << "}" << end();
}

void NateCode::codeStartLoopForStep(const TreeNodePtr& aNode)
{
	printLineNr(aNode->location);

	mOut << in() << "for (" << aNode->id->type()->codeType() << " " 
			 << aNode->id->codeName() << "= " << codeExpr(aNode->expr) << ";" 
			 << aNode->id->name() << (aNode->bool1 ? " >= " : "<=") << codeExpr(aNode->expr2) << ";"
			 << aNode->id->name() << (aNode->bool1 ? " -= " : "+=") << codeExpr(aNode->expr3) << ")" << end()
		   << in() << "{" << end();
	++mIndent;
	codeNested(aNode);
	--mIndent;
	mOut << in() << "}" << end();
}

void NateCode::codeStartLoopForRange(const TreeNodePtr& aNode)
{
	TypePtr rangeType = aNode->expr.type();
	auto range = mParser->uniqueName();
	auto iter = mParser->uniqueName();
	auto next = mParser->uniqueName();

	std::string ref = aNode->expr.is(ExprNode::Output) ? "&" : "";
	std::string increment;

	if (rangeType->isOfType("text"))
	{
		increment = iter + "=" + next;
		mOut << in() << "const std::string" << ref << " " << range << " = " << codeExpr(aNode->expr) << ";" << end(); 
		mOut << in() << "auto " << next << " = " << range << ".cbegin();" << end();
	}
	else
	{
		increment = "++" + iter;
		mOut << in() << "auto const" << ref << " " << range << " = " << codeExpr(aNode->expr) << ";" << end(); 
	}

	mOut << in() << "for (auto " << iter << " = " << range << ".cbegin(); "
			 << iter << " != " << range << ".cend(); "
			 << increment << ")" << end() << in() << "{" << end();

	++mIndent;

	if (rangeType->isOfType("text"))
	{
		mOut << in() << "uint32_t " << aNode->id->codeName() << " = utf8::next(" + next + "," + range + ".cend());" << end();
	}
	else
	{
		mOut << in() << "auto const& " << aNode->id->codeName() << " = *" << iter << ";" << end();
	}

	codeNested(aNode);
	--mIndent;
	mOut << in() << "}" << end();
}

void NateCode::codeWhile(const TreeNodePtr& aNode)
{
	printLineNr(aNode->location);
	mOut << in() << "if (!(" << codeExpr(aNode->expr) << ")) break;" << end();
}
