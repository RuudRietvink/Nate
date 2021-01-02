#include "NateCode.h"
#include "NateParser.h"
#include "NateFunctions.h"
#include "Identifier.h"

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

void NateCode::code(const std::shared_ptr<TreeNode>& aStat)
{
  for (auto& stat : aStat->nested)
  {
    switch (stat->code)
    {
    case ByteCode::Program:
      codeProgram(stat);
      break;
    case ByteCode::StdOutput:
      codeOutput("*output", stat);
      break;
    case ByteCode::LocalVar:
      codeDeclIdentifier(false, stat->id, stat->initialize, stat->location);
      break;
    case ByteCode::Assign:
      codeAssign(stat->exprList, stat->expr, stat->location);
      break;
    case ByteCode::Expr:
      mOut << codeExpr(stat->expr);
      break;
		default:
			std::cerr << "Bad bytecode " << (int)stat->code << std::endl;
			break;
    }
  }
}

char NateCode::end()
{
	++mPrevLine;
	return '\n';
}

void NateCode::codeProgram(const std::shared_ptr<TreeNode>& aNode)
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
	code(aNode);
	mOut << in(-1) << "}" << end();
}

void NateCode::codeOutput(const std::string& aStream, const std::shared_ptr<TreeNode>& aNode)
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

void NateCode::codeAssign(const std::vector<Expr>& aExpressions,
													Expr& aValue,
													const Location& aLocation)
{
	std::string endPars;

	printLineNr(aLocation);

	for (auto const& expr : aExpressions)
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

	mOut << codeExpr(aValue) << endPars << ";" << end();
}