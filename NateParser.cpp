#include "NateParser.h"
#include "core/Core.h"
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
	  
	mOut << "#define NOMINMAX" << std::endl;
	mOut << "#include <windows.h>" << std::endl;
	mOut << "#include \"C:\\Users\\ruud\\source\\repos\\Nate\\core\\Core.h\"" << std::endl;
	mOut << "#include <string>" << std::endl;
	mOut << "#include <cstdint>" << std::endl;
	mOut << "#include <cmath>" << std::endl;
	mOut << "#include <iostream>" << std::endl;
	mOut << "#include <algorithm>" << std::endl;
}

NateParser::~NateParser() = default;

int NateParser::parse()
{
	pushScope(std::make_shared<Scope>("global"));

	for (auto file : { "C:\\Users\\ruud\\source\\repos\\Nate\\core\\core.ns" })
	{
		std::ifstream stream(file);
	  yy::Lexer lexer(stream);
		lexer.nate = this;
		lexer.filenames.push_back(replaceAll(file, "\\", "\\\\"));
		yy::parser parser(lexer, *this);
		parser.parse();
	}

	return mParser->parse();
}

void NateParser::import(const std::string& aName)
{
  std::string library = "C:\\Users\\ruud\\source\\repos\\Nate\\core\\";
	mLexer->includeFile(library + aName + ".ns");
}

void NateParser::pushScope(const ScopePtr& aScope)
{
	//std::cerr << "push " << aScope.name() << std::endl;
	mScopes.push_front(aScope);
}

void NateParser::popScope()
{
	//std::cerr << "pop " << mScopes.front().name() << std::endl;
	mScopes.pop_front();
}

ScopePtr& NateParser::curScope()
{
	return mScopes.front();
}

Method& NateParser::curMethod()
{
	return mInCode ? static_cast<Method&>(curCode()) : static_cast<Method&>(curDefine());
}

void NateParser::addCode()
{
	pushScope(std::make_shared<Scope>("code"));
	mCodes.emplace_back();
	mInCode = true;
}

void NateParser::endCode()
{
	popScope();
}

Code& NateParser::curCode() { return mCodes.back(); }

void NateParser::addDefine()
{
	pushScope(std::make_shared<Scope>("define"));
	mDefines.emplace_back();
	mInCode = false;
}

void NateParser::declareDefine()
{
	mOut << curDefine().createCodeDecl() << " {" << std::endl;
	curDefine().createCodeCall();
}

void NateParser::endDefine()
{
	popScope();
	mOut << "}" << std::endl;
}

Define& NateParser::curDefine() { return mDefines.back(); }

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
	IdentifierPtr id = getIdentifier(aString);
	std::string name = aString;
	size_t pos = 0;

	while (!id && pos != std::string::npos)
	{
		pos = name.find_last_of("-");
		if (pos != std::string::npos)
		{
			name = name.substr(0, pos);
			id = getIdentifier(name);
		}
	}

	if (!id)
	{
		name = aString;
	}
	else
	{
		if (pos != 0)
		{
			for (auto ind = aString.size() - 1; ind >= pos; --ind)
			{
				mLexer->matcher().unput(aString[ind]);
			}

			name = aString.substr(0, pos);
		}
	}
	
	return std::make_tuple(!!id, name);
}

IdentifierPtr NateParser::getIdentifier(const std::string& aName, Scope* aScope)
{
	if (aScope == nullptr)
	{
		for (auto& scope : mScopes)
		{
			auto var = scope->getIdentifier(aName);
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
}

IdentifierPtr NateParser::getOrFakeIdentifier(const std::string& aName, Scope* aScope)
{
	IdentifierPtr result = getIdentifier(aName, aScope);
	if (!result)
	{
		error(std::string("Undeclared identifier: ") + aName);
		addIdentifier(std::make_shared<Identifier>(aName, std::make_shared<Type>("int-32")));
		result = getIdentifier(aName);
	}

	return result;
}

void NateParser::addIdentifier(const IdentifierPtr& aIdentifier)
{
	mScopes.front()->addIdentifier(aIdentifier);
}

TypePtr NateParser::getType(const std::string& aName, Scope* aScope)
{
	if (aScope == nullptr)
	{
		for (auto& scope : mScopes)
		{
			auto type = scope->getType(aName);
			if (type != nullptr)
			{
				return type;
			}
		}
	}
	else
	{
		return aScope->getType(aName);
	}
}

TypePtr NateParser::determineType(const std::string& aName)
{
	auto type = getType(aName);
	return !type ? std::make_shared<Type>(aName) : type;
}

void NateParser::methodMatches(const Method& aMethod,
														   ExprNodesCIter& aStartIter, ExprNodesCIter& aEndIter,
														   Match& aMatch)
{
	bool result;
	std::string errorMsg;
	std::tie(errorMsg, result) = aMethod.checkArgTypes(aStartIter, aEndIter);

	if (result)
	{
		aMatch.methodFound = &aMethod;
		aMatch.nodeStartIter = aStartIter;
		aMatch.nodeEndIter = aEndIter;
	  //std::cerr << std::get<1>(aMatch.methodFound->evaluate(aMatch.nodeStartIter, aMatch.nodeEndIter)) << std::endl;
	}
	else
	{
		//std::cerr << "MatchedMethod: " << aMethod << " -> " << errorMsg << std::endl;
		aMatch.matchedMethod = &aMethod;
		aMatch.matchedErrorMsg = errorMsg;
	}
}

void NateParser::checkLeftToRightMethod(const Method& aMethod,
																			  const Expr& aExpr,
																			  Match& aMatch)
{
	auto size = aMethod.args().size();

	for (auto startIter = aExpr.nodes().cbegin();
			 size <= static_cast<size_t>(std::distance(startIter, aExpr.nodes().cend())); ++startIter)
	{
		auto endIter = startIter + size;

		if (aMethod.matches(startIter, endIter) &&
				(aMatch.methodFound == nullptr || 
				 aMethod.priority() > aMatch.methodFound->priority() ||
				 startIter < aMatch.nodeStartIter ||
				 (startIter == aMatch.nodeStartIter && endIter > aMatch.nodeEndIter)))
		{
			methodMatches(aMethod, startIter, endIter, aMatch);
		}
	}
}

void NateParser::checkRightToLeftMethod(const Method& aMethod,
																			  const Expr& aExpr,
																			  Match& aMatch)
{
	auto size = aMethod.args().size();

	for (auto endIter = aExpr.nodes().cend();
			 size <= static_cast<size_t>(std::distance(aExpr.nodes().cbegin(), endIter)); --endIter)
	{
		auto startIter = endIter - size;

		if (aMethod.matches(startIter, endIter) &&
				(aMatch.methodFound == nullptr || 
				 aMethod.priority() > aMatch.methodFound->priority() ||
				 startIter > aMatch.nodeStartIter ||
				 (startIter == aMatch.nodeStartIter && endIter > aMatch.nodeEndIter)))
		{
			 methodMatches(aMethod, startIter, endIter, aMatch);
		}
	}
}

void NateParser::checkIfMethod(const Method& aMethod, const Expr& aExpr, Match& aMatch)
{
	if (!aMatch.methodFound || aMethod.priority() >= aMatch.methodFound->priority())
	{
		auto size = aMethod.args().size();
		if (size <= aExpr.nodes().size())
		{
			if (aMethod.is(Code::RightLeft))
			{
				checkRightToLeftMethod(aMethod, aExpr, aMatch);
			}
			else
			{
				checkLeftToRightMethod(aMethod, aExpr, aMatch);
			}
		}
	}
}

Expr NateParser::evaluate(const Expr& aExpr)
{
	Expr result = aExpr;
	//std::cerr << aExpr << std::endl;

	Match match;
	match.methodFound = nullptr;
	match.nodeStartIter = aExpr.nodes().cend();
	match.nodeEndIter = aExpr.nodes().cend();

	if (aExpr.nodes().size() > 1 || aExpr.node().is(ExprNode::Word))
	{
		match.matchedMethod = nullptr;

		for (auto const& code : mCodes)
		{
			checkIfMethod(code, aExpr, match);
		}
		
		for (auto const& define : mDefines)
		{
			checkIfMethod(define, aExpr, match);
		}

		if (match.methodFound)
		{
			std::string errorMsg;
			std::string methodStat;
			TypePtr methodType;
			Flags nodeFlags;
			std::tie(errorMsg, methodStat, methodType, nodeFlags) = 
				match.methodFound->evaluate(match.nodeStartIter, match.nodeEndIter);
			if (!errorMsg.empty())
			{
				error(errorMsg);
			}
			//std::cerr << *methodType << " " << methodStat << std::endl;
			
			ExprNode node(methodStat, methodStat, methodType);
			node.setFlags(nodeFlags);

			Expr newExpr;
			newExpr.addNodes(aExpr.nodes().cbegin(), match.nodeStartIter);
			newExpr.addNode(node);
			newExpr.addNodes(match.nodeEndIter, aExpr.nodes().cend());
			//std::cerr << newExpr << std::endl;
			return evaluate(newExpr);
		}
		else
		{
			if (match.matchedMethod != nullptr)
			{
				error("Bad argument types for code: " + match.matchedMethod->signature() + ": " + match.matchedErrorMsg);
			}
		}
	}

	if (aExpr.nodes().size() > 1 || aExpr.nodes().front().is(ExprNode::Word))
	{
		error("Bad expression: " + aExpr.text());
	}
	//std::cerr << result << std::endl;
	return result;
}

void NateParser::codeStartProgram()
{
	printLineNr();
	mOut << "int main(int argc, char** argv)\n{" << std::endl;
	mOut << "SetConsoleOutputCP(65001);" << std::endl;
	mOut << "std::locale::global(std::locale(\"en_US.UTF8\"));" << std::endl;
    
	pushScope(std::make_shared<Scope>("main"));
}

void NateParser::codeEndProgram()
{
	popScope();
	mOut << "}" << std::endl;
}

void NateParser::codeDeclareLocalIdentifier(const IdentifierPtr& aIdentifier,
																						bool initializeNonScalars)
{
	addIdentifier(aIdentifier);
	if (aIdentifier->type()->is(Type::Unknown))
	{
		error("Unknown type: " + aIdentifier->type()->name());
		return;
	}

	printLineNr();
	mOut << aIdentifier->type()->codeType() << " " << aIdentifier->codeName();
	if (aIdentifier->type()->is(Type::Scalar) || initializeNonScalars)
	{
		mOut << " = " << aIdentifier->initValue().code();
	}

	mOut<< ";" << std::endl;
}

void NateParser::codeDeclareLocalIdentifiers(const std::vector<std::string>& aNames,
																						 const std::string& aType,
																					 	 const std::vector<Expr>& aInitValues,
																						 bool initializeNonScalars)
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
	TypePtr type;

	for (auto const& name : aNames)
	{
		if (getIdentifier(name, curScope().get()))
		{
			error("duplicate declaration of: " + name);
		}
		Expr initValue;
		if (aInitValues.empty())
		{
			type = determineType(aType);
			initValue = Expr(ExprNode("default", "{}", type));
			initValue.node().setFlag(ExprNode::Default, true);
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

		codeDeclareLocalIdentifier(std::make_shared<Identifier>(name, type, initValue), initializeNonScalars);
	}
}

void NateParser::codeStartRecord(const RecordPtr& aRecord)
{
	printLineNr();
	pushScope(aRecord->scope());
	mOut << "struct " << aRecord->codeType() << " {" << std::endl;
}

void NateParser::codeDeclareRecordIdentifiers(const std::vector<std::string>& aNames,
																							const std::string& aType,
																							const std::vector<Expr>& aInitValues)
{
	codeDeclareLocalIdentifiers(aNames, aType, aInitValues, false);

	for (auto& id : curScope()->getIdentifiers())
	{
		if (!id->type()->is(Type::Scalar) && !id->initValue().is(ExprNode::Default))
		{
			error("Cannot initialize member: " + id->name() + " of record " + curScope()->name());
		}
	}
}

void NateParser::codeEndRecord()
{
	mOut << "};" << std::endl;
}

void NateParser::codeAssign(const std::vector<Expr>& aExpressions, Expr& aValue)
{
	printLineNr();
	for (auto const& expr : aExpressions)
	{
		if (!expr.is(ExprNode::Output))
		{
			error("Cannot assign to a non-variable");
		}
		else
		{
			const TypePtr& exprType = expr.type();
			bool ok = aValue.node().castToType(exprType);
			if (!ok)
			{
				error("cannot cast '" + aValue.text() + "' of type " + aValue.type()->name() + " to type " + exprType->name());
			}
				
			mOut << expr.code() << " = ";
		}
	}

	mOut << aValue.code() << ";" << std::endl;
}

std::string NateParser::codeId(const std::string& aName, Scope* aScope)
{
	return getOrFakeIdentifier(aName, aScope)->codeName();
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
		if (!aString.empty())
		{
			mOut << " << " << aString;
		}
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
		if (!aString.empty())
		{
			mOut << " << " << aString;
		}
	}
}

void NateParser::codeOutput(const Expr& aValue)
{
	if (aValue.type() && aValue.type()->is(Type::Boolean))
	{
		codeOutput("std::boolalpha ");
	}
	
	if (aValue.is(ExprNode::Literal))
	{
		codeOutput(aValue.code());
	}
	else
	{
		if (aValue.type() && aValue.type()->name() == "int-8")
		{
			codeOutput("static_cast<int>(" + aValue.code() + ")");
		}
		else
		{
			codeOutput("(" + aValue.code() + ")");
		}
	}
}

void NateParser::codeOutputEnd(bool aAddEnd)
{
	if (aAddEnd)
	{
		codeOutput("std::endl");
	}
	else
	{
		codeOutput("");
	}

	mOut << ";" << std::endl;
}

void NateParser::codeIf(const Expr& aValue)
{
	printLineNr();
	if (!aValue.type()->is(Type::Boolean))
	{
		error("Expected boolean expression for IF statement");
	}

	mOut << "if (" << aValue.code() << ") {" << std::endl;
	pushScope(std::make_shared<Scope>("if"));
}

void NateParser::codeElseIf()
{
	mOut << "else " << std::endl;
}

void NateParser::codeElse()
{
	printLineNr();
	mOut << "else {" << std::endl;
	pushScope(std::make_shared<Scope>("else"));
}

void NateParser::codeEndIf()
{
	popScope();
	mOut << "}" << std::endl;
}

void NateParser::codeInitLoop()
{
	printLineNr();
	mLoopWhileCounts.push_back(0);
	pushScope(std::make_shared<Scope>("while"));
}

void NateParser::codeStartLoop()
{
	mOut << "while (true) {" << std::endl;
}

void NateParser::codeStartForLoop(const std::string& aId,
																	const std::string& aType,   
																	bool aDownTo,
																	const Expr& aStart,
																	const Expr& aEnd,
																	const Expr& aStep)
{
	TypePtr type = aType.empty()
							   ? aStart.type()
							   : determineType(aType);

	IdentifierPtr id = std::make_shared<Identifier>(aId, type);
	addIdentifier(id);

	mOut << "for (" << id->type()->codeType() << " " 
			 << id->codeName() << "= " << aStart.code() << ";" 
			 << id->name() << (aDownTo ? " >= " : "<=") << aEnd.code() << ";"
			 << id->name() << (aDownTo ? " -= " : "+=") << aStep.code() << ") {" << std::endl;
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

	if (!aExpr.type()->is(Type::Boolean))
	{
		error("Expected boolean condition in while.");
	}
		
	printLineNr();
	mOut << "if (!(" << aExpr.code() << ")) break;" << std::endl;
}

 void NateParser::codeReturn(const Expr& aValue)
 {
	 printLineNr();
	 mOut << "return " << aValue.code() << ";" << std::endl;
 }

 void NateParser::codeExpression(const Expr& aValue)
 {
	 printLineNr();
	 mOut << aValue.code() << ";" << std::endl;
 }