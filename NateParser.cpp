#include "NateParser.h"

#include "lex.yy.h"
#include "NateParser.tab.h"
#include <algorithm>
#include <inttypes.h>
#include <cctype>
#include <tuple>

NateParser::NateParser(const std::string& aFilename, std::istream& aIn, std::ostream& aOut)
: mLexer(new yy::Lexer(aIn)),
  mParser(new yy::parser(*mLexer, *this)),
  mOut(aOut)
{
	mLexer->nate = this;
	mLexer->filenames.push_back(replaceAll(aFilename, "\\", "\\\\"));
	mOut << "#include <string>" << std::endl;
	mOut << "#include <cstdint>" << std::endl;
	mOut << "#include <cmath>" << std::endl;
	mOut << "#include <iostream>" << std::endl;
}

NateParser::~NateParser() = default;

int NateParser::parse()
{
	return mParser->parse();
}

void NateParser::pushScope(const Scope& aScope)
{
  //std::cerr << "push " << aScope.name() << std::endl;
	mScopes.push_front(aScope);
}

void NateParser::popScope()
{
  //std::cerr << "pop " << mScopes.front().name() << std::endl;
	mScopes.pop_front();
}

Scope& NateParser::currentScope()
{
	return mScopes.front();
}

void NateParser::addCode()
{
	pushScope(Scope("code"));
	mCodes.emplace_back();
}

void NateParser::endCode()
{
	popScope();
}

Code& NateParser::curCode() { return mCodes.back(); }

void NateParser::error(const std::string& anError)
{
	std::cerr << mLexer->location() << ": " << anError << std::endl;
	++mErrors;
}

void NateParser::printLineNr()
{
  static int prevLine = 0;
  static std::string prevFile;

	auto const& begin = mLexer->location().begin;
  if (prevLine + 1 != begin.line || prevFile != mLexer->filenames.back())
  {
    prevLine = begin.line;
    prevFile = mLexer->filenames.back();
    mOut << "#line " << prevLine << " \"" << prevFile << "\"" << std::endl;
  }
}

std::tuple<bool, std::string> NateParser::makeIdOrWord(const std::string& aString)
{
	Identifier* id = getIdentifier(aString);

	std::string name = aString;

	return std::make_tuple((id != nullptr), name);
}

Identifier* NateParser::getIdentifier(const std::string& aName, Scope* aScope)
{
	if (aScope == nullptr)
	{
		for (auto& scope : mScopes)
		{
			auto var = scope.getIdentifier(aName);
			if (var != nullptr)
			{
				return var;
			}
		}
	}
	else
	{
		return aScope->getIdentifier(aName);
	}

	return nullptr;
}

Identifier& NateParser::getOrFakeIdentifier(const std::string& aName, Scope* aScope)
{
	Identifier* result = getIdentifier(aName, aScope);
	if (!result)
	{
		error(std::string("Undeclared identifier: ") + aName);
		addIdentifier(Identifier(aName, Type("int-32")));
		result = getIdentifier(aName);
	}

	return *result;
}

void NateParser::addIdentifier(const Identifier& aIdentifier)
{
	mScopes.front().addIdentifier(aIdentifier);
}

std::string NateParser::pattern(const ExprNode& aNode) const
{
	std::string result;

	if (aNode.is(ExprNode::Word))
	{
		result = aNode.text();
	}
	else
	{
		result = "%E%";
	}

	return result;
}

std::string NateParser::pattern(std::vector<ExprNode>::const_iterator& aBegin, 
							                  std::vector<ExprNode>::const_iterator& aEnd) const
{
	std::string result;

	for (auto iter = aBegin; iter != aEnd; ++iter)
	{
		result += pattern(*iter);
	}

	return result;
}

void NateParser::codeMatches(const Code& aCode,
	                           ExprNodesCIter& aStartIter, ExprNodesCIter& aEndIter,
	                           Match& aMatch)
{
	bool result;
	std::string errorMsg;
	std::tie(errorMsg, result) = aCode.checkArgTypes(aStartIter, aEndIter);

	if (result)
	{
		aMatch.codeFound = &aCode;
		aMatch.codeStartIter = aStartIter;
		aMatch.codeEndIter = aEndIter;
		//std::cerr << std::get<1>(aMatch.codeFound->evaluate(aMatch.codeStartIter, aMatch.codeEndIter)) << std::endl;
	}
	else
	{
		aMatch.matchedCode = &aCode;
		aMatch.matchedErrorMsg = errorMsg;
	}
}

void NateParser::checkLeftToRightCodes(const Code& aCode,
	                                     const Expr& aExpr,
	                                     Match& aMatch)
{
	auto codeSize = aCode.codeArgs().size();

	for (auto startIter = aExpr.nodes().cbegin();
		   codeSize <= static_cast<size_t>(std::distance(startIter, aExpr.nodes().cend())); ++startIter)
	{
		auto endIter = startIter + codeSize;
	  auto exprPattern = pattern(startIter, endIter);

    if (aCode.matches(exprPattern) &&
        startIter < aMatch.codeStartIter)
    {
      codeMatches(aCode, startIter, endIter, aMatch);
    }
	}
}

void NateParser::checkRightToLeftCodes(const Code& aCode,
	                                     const Expr& aExpr,
	                                     Match& aMatch)
{
	auto codeSize = aCode.codeArgs().size();

	for (auto endIter = aExpr.nodes().cend();
       codeSize <= static_cast<size_t>(std::distance(aExpr.nodes().cbegin(), endIter)); --endIter)
	{
		auto startIter = endIter - codeSize;
	  auto exprPattern = pattern(startIter, endIter);

    if (aCode.matches(exprPattern) &&
        (aMatch.codeStartIter == aExpr.nodes().cend() || startIter > aMatch.codeStartIter))
    {
       codeMatches(aCode, startIter, endIter, aMatch);
    }
	}
}

Expr NateParser::evaluate(const Expr& aExpr)
{
	Expr result = aExpr;
	//std::cerr << aExpr << std::endl;

	Match match;
	match.codeFound = nullptr;
	match.codeStartIter = aExpr.nodes().cend();
	match.codeEndIter = aExpr.nodes().cend();

  if (aExpr.nodes().size() > 1 || aExpr.node().is(ExprNode::Word))
  {
		match.matchedCode = nullptr;

    for (auto const& code : mCodes)
    {
			if (!match.codeFound || code.priority() >= match.codeFound->priority())
			{
				auto codeSize = code.codeArgs().size();
				if (codeSize <= aExpr.nodes().size())
				{
          if (code.is(Code::RightLeft))
          {
             checkRightToLeftCodes(code, aExpr, match);
          }
          else
          {
            checkLeftToRightCodes(code, aExpr, match);
          }
				}
			}
		}

		if (match.codeFound)
		{
			std::string errorMsg;
			std::string codeStat;
			Type codeType;
			std::tie(errorMsg, codeStat, codeType) = match.codeFound->evaluate(match.codeStartIter, match.codeEndIter);
			if (!errorMsg.empty())
			{
				error(errorMsg);
			}
			//std::cerr << codeType << " " << codeStat << std::endl;

			Expr newExpr;
			newExpr.addNodes(aExpr.nodes().cbegin(), match.codeStartIter);
			newExpr.addNode(ExprNode(codeStat, codeStat, codeType));
			newExpr.addNodes(match.codeEndIter, aExpr.nodes().cend());
			//std::cerr << newExpr << std::endl;
			return evaluate(newExpr);
		}
		else
		{
			if (match.matchedCode != nullptr)
			{
				error("Bad argument types for code: " + match.matchedCode->signature() + ": " + match.matchedErrorMsg);
			}
		}
	}

	if (aExpr.nodes().size() > 1 || aExpr.nodes().front().is(ExprNode::Word))
	{
		error("Bad expression: " + aExpr.text());
	}
	return result;
}

void NateParser::codeStartProgram()
{
	printLineNr();
	mOut << "int main(int argc, char** argv)\n{" << std::endl;
	pushScope(Scope("main"));
}

void NateParser::codeEndProgram()
{
	popScope();
	mOut << "}" << std::endl;
}

void NateParser::codeDeclareLocalIdentifier(const Identifier& aIdentifier)
{
	addIdentifier(aIdentifier);
	if (aIdentifier.type().is(Type::Unknown))
	{
		error("Unknown type: " + aIdentifier.type().name());
	}

	printLineNr();
	mOut << aIdentifier.type().codeType() << " " << aIdentifier.codeName() << " = " << aIdentifier.initValue().code() << ";" << std::endl;
}

void NateParser::codeDeclareLocalIdentifiers(const std::vector<std::string>& aNames,
	                                         const std::string& aType,
	                                         const std::vector<Expr>& aInitValues)
{
	//std::cout << join(aNames, ", ") << ":" << aType << ":" << join(aInitValues, ", ") << std::endl;

	if (aType.empty() && aInitValues.empty())
	{
		error("expected either type or initial value.");
	}
	else if (aType.empty() && (aInitValues.size() != 1 && aInitValues.size() != aNames.size()))
	{
		error("expected as many initial values as identifiers.");
	}

	auto initIter = aInitValues.cbegin();
	Type type;

	for (auto const& name : aNames)
	{
		if (getIdentifier(name))
		{
			error("duplicate declaration of: " + name);
		}
		Expr initValue;
    if (aInitValues.empty())
    {
      type = Type(aType);
      initValue = Expr(ExprNode("default", "{}", type));
    }
    else if (aInitValues.size() != 1 || initIter == aInitValues.cbegin())
    {
      type = initIter->type();
      initValue = *initIter++;
    }
    else 
    {
      initValue = Expr(ExprNode(aNames.front(), codeId(aNames.front()), type));
    }

		codeDeclareLocalIdentifier(Identifier(name, type, initValue));
	}
}

void NateParser::codeAssign(const std::vector<std::string>& aNames, Expr& aValue)
{
  printLineNr();
  for (auto const& name : aNames)
  {
	  const Type& nameType = getOrFakeIdentifier(name).type();
	  bool ok = aValue.node().castToType(nameType);
	  if (!ok)
	  {
		  error("cannot cast '" + aValue.text() + "' to type " + nameType.name());
	  }
        
	  mOut << codeId(name) << " = ";
  }

  mOut << aValue.code() << ";" << std::endl;
}

std::string NateParser::codeId(const std::string& aName, Scope* aScope)
{
	return getOrFakeIdentifier(aName, aScope).codeName();
}

void NateParser::codeOutputStart(const std::string& aStream)
{
	printLineNr();
	mOut << aStream;
	mCachedOutput.clear();
}

void NateParser::codeOutput(const std::string& aString)
{
	if ((!mCachedOutput.empty()) && aString[0] != '"')
	{
		mOut << " << \"" << mCachedOutput << "\"";
		mCachedOutput.clear();
		mOut << " << " << aString;
	}
	else if (!mCachedOutput.empty())
	{
		mCachedOutput += aString.substr(1, aString.size() - 2);
	}
	else if (aString[0] == '"')
	{
		mCachedOutput = aString.substr(1, aString.size() - 2);
	}
	else
	{
		mOut << " << " << aString;
	}
}

void NateParser::codeOutput(const Expr& aValue)
{
	if (aValue.type().is(Type::Boolean))
	{
		codeOutput("std::boolalpha ");
	}
  
	if (aValue.is(ExprNode::Literal))
	{
		codeOutput(aValue.code());
	}
	else
	{
	  if (aValue.type().name() == "int-8")
	  {
		  codeOutput("static_cast<int>(" + aValue.code() + ")");
	  }
    else
    {
		  codeOutput("(" + aValue.code() + ")");
    }
	}
}

void NateParser::codeOutputEnd()
{
	codeOutput("std::endl;");
	mOut << std::endl;
}

void NateParser::codeIf(const Expr& aValue)
{
	printLineNr();
	if (!aValue.type().is(Type::Boolean))
	{
		error("Expected boolean expression for IF statement");
	}

	mOut << "if (" << aValue.code() << ") {" << std::endl;
	pushScope(Scope("if"));
}

void NateParser::codeElseIf()
{
	mOut << "else " << std::endl;
}

void NateParser::codeElse()
{
	printLineNr();
	mOut << "else {" << std::endl;
	pushScope(Scope("else"));
}

void NateParser::codeEndIf()
{
  popScope();
	mOut << "}" << std::endl;
}

void NateParser::codeStartLoop()
{
	printLineNr();
  mLoopWhileCounts.push_back(0);
	pushScope(Scope("while"));
  mOut << "while (true) {" << std::endl;
}

void NateParser::codeStartForLoop(const std::string& aId,
                                  const std::string& aType,   
                                  bool aDownTo,
                                  const Expr& aStart,
                                  const Expr& aEnd,
                                  const Expr& aStep)
{
	printLineNr();
  mLoopWhileCounts.push_back(0);
	pushScope(Scope("while"));
  Type type = aType.empty()
              ? aStart.type()
              : Type(aType);

  Identifier id(aId, type);
  addIdentifier(id);

  mOut << "for (" << id.type().codeType() << " " 
       << id.codeName() << "= " << aStart.code() << ";" 
       << id.name() << (aDownTo ? " >= " : "<=") << aEnd.code() << ";"
       << id.name() << (aDownTo ? " -= " : "+=") << aStep.code() << ") {" << std::endl;
}

void NateParser::codeEndLoop()
{
  mLoopWhileCounts.pop_back();
  mOut << "}" << std::endl;
  popScope();
}

void NateParser::codeLoopWhile(const Expr& aExpr)
{
  if (mLoopWhileCounts.back() > 0)
  {
    error("Only one while allowed in a loop.");
  }

  ++mLoopWhileCounts.back();

  if (!aExpr.type().is(Type::Boolean))
  {
    error("Expected boolean condition in while.");
  }
    
	printLineNr();
  mOut << "if (!(" << aExpr.code() << ")) break;" << std::endl;
}