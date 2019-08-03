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
	initAliases();

	mLexer->nate = this;
	mLexer->filenames.push_back(Core::replaceAll(aFilename, "\\", "\\\\"));
	  
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
	pushScope("global");

	for (auto file : { "C:\\Users\\ruud\\source\\repos\\Nate\\core\\core.ns" })
	{
		std::ifstream stream(file);
	  yy::Lexer lexer(stream);
		lexer.nate = this;
		lexer.filenames.push_back(Core::replaceAll(file, "\\", "\\\\"));
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

void NateParser::initAliases()
{
	mAliases.insert(std::make_pair("√", "sqrt"));
	mAliases.insert(std::make_pair("÷", "/"));
	mAliases.insert(std::make_pair("×", "*"));
	mAliases.insert(std::make_pair("⋅", "*"));
	mAliases.insert(std::make_pair("π", "pi"));
	mAliases.insert(std::make_pair("τ", "tau"));
}

void NateParser::pushScope(const std::string& aName)
{
	mScopes.push_front(std::make_shared<Scope>(aName));
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
	return mMethodType == MethodType::Code 
			   ? static_cast<Method&>(curCode()) 
		     : static_cast<Method&>(curDefine());
}

void NateParser::addCode()
{
	pushScope("code");
	mCodes.emplace_back();
	mMethodType = MethodType::Code;
}

void NateParser::endCode()
{
	popScope();
}

Code& NateParser::curCode() { return mCodes.back(); }

void NateParser::addDefine()
{
	pushScope("define");
	mDefines.emplace_back();
	mMethodType = MethodType::Define;
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

void NateParser::unput(const std::string::const_iterator& aStart,
											 const std::string::const_iterator& aEnd)
{
	std::string::const_iterator iter = aStart;
	if (iter != aEnd)
	{
		char kar = *iter;
		unput(++iter, aEnd);
		mLexer->matcher().unput(kar);
	}
}

std::string NateParser::alias(const std::string& aString)
{
	std::string result;
	auto iter = mAliases.find(aString);

	if (iter != mAliases.cend())
	{
		result = iter->second;
	}
	else
	{
		result = aString;
	}

	return result;
}

std::tuple<bool, std::string> NateParser::makeIdOrWord(const std::string& aOrig, const std::string& aString)
{	
	std::string name = aString;
	IdentifierPtr id = getIdentifier(alias(name));
	size_t pos = 0;

	while (!id && pos != std::string::npos)
	{
		pos = name.find_last_of("-");
		if (pos != std::string::npos)
		{
			name = name.substr(0, pos);
			IdentifierPtr newId = getIdentifier(alias(name)); // temp needed otherwise it crashes on NULL struct
			id = newId;
		}
	}

	if (!id)
	{
		auto iter = name.cbegin();
		bool ok = true;
		bool first = true;

		while (iter != name.cend() && ok)
		{
			auto next = iter;
			utf8::next(next, name.cend());
			IdentifierPtr newId = getIdentifier(alias(std::string(iter, next)));
			id = newId;
			if (!id)
			{
				ok = false;
			}
			else if (first)
			{
				pos = std::distance(iter, next);
				first = false;
			}
			
			utf8::next(iter, name.cend());
		}
	}

	if (id)
	{
		if (pos != 0)
		{
			auto iter = aOrig.cbegin();
			utf8::advance(iter, pos, aOrig.cend());
			name = aString.substr(0, pos);
			unput(iter, aOrig.cend());
		}
	}
	
	return std::make_tuple(!!id, alias(name));
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
		addIdentifier(std::make_shared<Identifier>(curScope(), aName, std::make_shared<Type>("int-32")));
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
	}
	else
	{
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
		
		//if (aMethod.matches(startIter, endIter))
		//{
		//	Expr exp;
		//	exp.addNodes(startIter, endIter);
		//	std::cerr << "checkLeftToRightMethod " << exp.text() << std::endl;
		//	if (aMatch.methodFound != nullptr)
		//	{
		//		Expr mat;
		//		mat.addNodes(aMatch.nodeStartIter, aMatch.nodeEndIter);
		//		std::cerr <<  mat.text() << std::endl;
		//	}
		//}

		if (aMethod.matches(startIter, endIter) &&
				(aMatch.methodFound == nullptr || 
				 aMethod.priority() > aMatch.methodFound->priority() ||
				 (aMethod.priority() == aMatch.methodFound->priority() &&
          startIter < aMatch.nodeStartIter)))
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
		
		//if (aMethod.matches(startIter, endIter))
		//{
		//	Expr exp;
		//	exp.addNodes(startIter, endIter);
		//	std::cerr << "checkRightToLeftMethod " << exp.text() << std::endl;
		//	if (aMatch.methodFound != nullptr)
		//	{
		//		Expr mat;
		//		mat.addNodes(aMatch.nodeStartIter, aMatch.nodeEndIter);
		//		std::cerr <<  mat.text() << std::endl;
		//	}
		//}

		if (aMethod.matches(startIter, endIter) &&
				(aMatch.methodFound == nullptr || 
				 aMethod.priority() > aMatch.methodFound->priority() ||
				 (aMethod.priority() == aMatch.methodFound->priority() &&
          startIter > aMatch.nodeStartIter)))
		{
			 methodMatches(aMethod, startIter, endIter, aMatch);
		}
	}
}

void NateParser::checkIfMethod(const Method& aMethod, const Expr& aExpr, Match& aMatch)
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

Expr NateParser::evaluate(const Expr& aExpr)
{
	Expr result = aExpr;
	//std::cerr << "+++++ " << aExpr.text() << std::endl;

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
			//std::cerr << "******* " << *methodType << " " << methodStat << std::endl;
			
			ExprNode node(methodStat, methodStat, methodType);
			node.setFlags(nodeFlags);
			if (match.methodFound->is(Method::ConstExpr))
			{
				node.setFlag(ExprNode::ConstExpr);
			}

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
		exit(1);
	}
	//std::cerr << result << std::endl;
	return result;
}

bool NateParser::isLeftMonomial(const std::string& aWord) const
{
	return std::find_if(mCodes.cbegin(), mCodes.cend(), 
											[&aWord](const Code& aItem) 
											{ 
												return aItem.is(Method::LeftMonomial) &&
														    aItem.args().front().word() == aWord;
											}) 
							!= mCodes.cend();
}

void NateParser::codeStartProgram()
{
	printLineNr();
	mOut << "int main(int argc, char** argv)\n{" << std::endl;
	mOut << "SetConsoleOutputCP(65001);" << std::endl;
	//mOut << "std::locale::global(std::locale(\"en_US.UTF8\"));" << std::endl;
    
	pushScope("main");
}

void NateParser::codeEndProgram()
{
	popScope();
	mOut << "}" << std::endl;
}

void NateParser::codeStartScope()
{
	printLineNr();
	pushScope("scope");
	mOut << "{" << std::endl;
}
void NateParser::codeEndScope()
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
	if (aIdentifier->is(Identifier::Const))
	{
		if (aIdentifier->type()->is(Type::Scalar))
		{
			mOut << "constexpr ";		
		}
		else
		{
			mOut << "const ";		
		}
	}

	mOut << aIdentifier->type()->codeType() << " " << aIdentifier->codeName();
	if (aIdentifier->type()->is(Type::Scalar) || initializeNonScalars)
	{
		mOut << " = " << aIdentifier->initValue().code();
	}

	if (aIdentifier->is(Identifier::Const) && !aIdentifier->initValue().is(ExprNode::ConstExpr))
	{
		error("Expected constant expression.");
	}

	mOut<< ";" << std::endl;
}

void NateParser::codeDeclareLocalIdentifiers(bool aConst,
																						 const std::vector<std::string>& aNames,
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
			initValue.node().setFlag(ExprNode::Default);
			if (aConst)
			{
				error("Expected initial values for constants");
			}
		}
		else if (aInitValues.size() != 1 || initIter == aInitValues.cbegin())
		{
			type = initIter->type();
			initValue = *initIter++;
		}
		else 
		{
			initValue = Expr(ExprNode(aNames.front(), codeId(aNames.front()), type));
			initValue.node().setFlag(ExprNode::ConstExpr, aConst);
		}

		IdentifierPtr id = std::make_shared<Identifier>(curScope(), name, type, initValue);
		id->setFlag(Identifier::Const, aConst);

		codeDeclareLocalIdentifier(id, initializeNonScalars);
	}
}

void NateParser::codeStartRecord(const RecordPtr& aRecord)
{
	printLineNr();
	pushScope(aRecord->scope());
	mOut << "struct " << aRecord->codeType() << " {" << std::endl;
}

void NateParser::codeDeclareRecordIdentifiers(bool aConst,
																							const std::vector<std::string>& aNames,
																							const std::string& aType,
																							const std::vector<Expr>& aInitValues)
{
	codeDeclareLocalIdentifiers(aConst, aNames, aType, aInitValues, false);

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
	mStream = aStream;
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

void NateParser::codeInputStart(const std::string& aStream)
{
	mStream = aStream;
	printLineNr();
	mOut << aStream;
}

void NateParser::codeInputSpace()
{
}

void NateParser::codeInputNoSpace()
{
}

void NateParser::codeInput(const Expr& aValue)
{
	if (aValue.is(ExprNode::Output) && !aValue.is(ExprNode::ConstExpr))
	{
		if (aValue.type() && aValue.type()->is(Type::Boolean))
		{
			mOut << ">> std::boolalpha ";
		}
	
		mOut << ">> " << aValue.code();
	}
	else
	{
		error("Expected non-constant variable for input");
	}
}

void NateParser::codeInputEnd(bool aAddEnd)
{
	mOut << ";";
	if (aAddEnd)
	{
		mOut << mStream << ".ignore(std::numeric_limits<std::streamsize>::max(), '\\n');";
	}
	mOut << std::endl;
}

void NateParser::NateParser::codeIf(const Expr& aValue)
{
	printLineNr();
	if (!aValue.type()->is(Type::Boolean))
	{
		error("Expected boolean expression for IF statement");
	}

	mOut << "if (" << aValue.code() << ") {" << std::endl;
	pushScope("if");
}

void NateParser::codeElseIf()
{
	mOut << "else " << std::endl;
}

void NateParser::codeElse()
{
	printLineNr();
	mOut << "else {" << std::endl;
	pushScope("else");
}

void NateParser::codeEndIf()
{
	popScope();
	mOut << "}" << std::endl;
}

void NateParser::codeIfIs(const Expr& aValue)
{
	printLineNr();
	if (!aValue.type()->is(Type::Scalar))
	{
		error("Expected scalar expression for IF IS statement");
	}

	mOut << "switch (" << aValue.code() << ") {" << std::endl;
	pushScope("if is");
}

void NateParser::codeIs(const Expr& aValue, const Expr& aIfExpr)
{
	printLineNr();
	if (!aValue.is(ExprNode::ConstExpr))
	{
		error("Expected constant expression for IS clause");
	}
	
	if (!(aValue.type()->name() == aIfExpr.type()->name() ||
			 aValue.type()->is(Type::Float) == aIfExpr.type()->is(Type::Float)))
	{
		error("Expected expression with same type as in IF");
	}

	mOut << "case " << aValue.code() << ":" << std::endl;
}

void NateParser::codeElseIs()
{
	printLineNr();
	mOut << "default:" << std::endl;
}

void NateParser::codeBeginIs()
{
	printLineNr();
	mOut << "{" << std::endl;
	pushScope("is");
}

void NateParser::codeEndIs()
{
	printLineNr();
	mOut << "break;" << std::endl;
	mOut << "}" << std::endl;
	popScope();
}

void NateParser::codeEndIfIs()
{
	printLineNr();
	popScope();
	mOut << "}" << std::endl;
}

void NateParser::codeInitLoop()
{
	printLineNr();
	mLoopWhileCounts.push_back(0);
	pushScope("while");
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

	IdentifierPtr id = std::make_shared<Identifier>(curScope(), aId, type);
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