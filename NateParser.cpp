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
	mOut(&aOut)
{
	mLexer->nate = this;
	mLexer->filenames.push_back(Core::replaceAll(aFilename, "\\", "\\\\"));
	  
	*mOut << "#define NOMINMAX" << std::endl;
	*mOut << "#include <windows.h>" << std::endl;
	*mOut << "#include \"C:\\Users\\ruud\\source\\repos\\Nate\\core\\Core.h\"" << std::endl;
	*mOut << "#include <string>" << std::endl;
	*mOut << "#include <list>" << std::endl;
	*mOut << "#include <cstdint>" << std::endl;
	*mOut << "#include <cmath>" << std::endl;
	*mOut << "#include <iostream>" << std::endl;
	*mOut << "#include <algorithm>" << std::endl;
	*mOut << "#include <complex>" << std::endl;
}

NateParser::~NateParser() = default;

int NateParser::parse()
{
	pushScope("global");
	
	addType(std::make_shared<Type>("any"));
	addType(std::make_shared<Type>("number", getType("any")));
	addType(std::make_shared<Type>("integer", getType("number")));
	addType(std::make_shared<Type>("real", getType("number")));
	addType(std::make_shared<Type>("int-8", getType("integer")));
	addType(std::make_shared<Type>("int-16", getType("integer")));
	addType(std::make_shared<Type>("int-32", getType("integer")));
	addType(std::make_shared<Type>("int-64", getType("integer")));
	addType(getType("int-32"), "int");
	addType(std::make_shared<Type>("float-32", getType("real")));
	addType(std::make_shared<Type>("float-64", getType("real")));
	addType(getType("float-32"), "float");
	addType(std::make_shared<Type>("boolean", getType("any")));
	addType(getType("boolean"), "bool");
	addType(std::make_shared<Type>("record", getType("any")));
		
	addType(std::make_shared<Type>("container", getType("any")));
	addType(std::make_shared<Type>("sequence-container", getType("container")));
	addType(std::make_shared<Type>("list", getType("sequence-container")));
	addType(std::make_shared<Type>("text", getType("sequence-container")));
	addType(std::make_shared<Type>("char"));
	getType("text")->setTypenameType(getType("char"));
	addType(std::make_shared<Type>("fraction", getType("number")));
	addType(std::make_shared<Type>("imaginary", getType("number")));
	addType(std::make_shared<Type>("complex", getType("number")));

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

std::string NateParser::in(int aOffset) const
{
	return std::string(mScopes.size() + aOffset - 1, '\t');
}

void NateParser::import(const std::string& aName)
{
	if (mImports.find(aName) == mImports.cend())
	{
		mImports.insert(aName);
		std::string library = "C:\\Users\\ruud\\source\\repos\\Nate\\core\\";
		mLexer->includeFile(library + aName + ".ns");
	}
}

void NateParser::addAlias(const std::string& aName, const std::string& aValue)
{
	mAliases.insert(std::make_pair(aName, aValue));
}

void NateParser::addLeftMonomial(const std::string& aWord)
{
	mLeftMonomial.insert(aWord);
}

bool NateParser::isLeftMonomial(const std::string& aWord) const
{
	return mLeftMonomial.find(aWord) != mLeftMonomial.cend();
}

void NateParser::addWantsUnary(const std::string& aWord)
{
	mWantsUnary.insert(aWord);
}

bool NateParser::wantsUnary(const std::string& aWord) const
{
	return mWantsUnary.find(aWord) != mWantsUnary.cend();
}

std::string NateParser::uniqueName() const
{
	static int count = 0;

	return "tmp__" + std::to_string(count++) + "__";
}

void NateParser::pushScope(const std::string& aName)
{
	mScopes.push_front(std::make_shared<Scope>(aName));
}

void NateParser::pushScope(const ScopePtr& aScope)
{
	if (mLexer->debug()) std::cerr << "push " << aScope->name() << std::endl;
	mScopes.push_front(aScope);
}

void NateParser::popScope()
{
	mScopes.pop_front();
	if (mLexer->debug()) std::cerr << "pop to " << mScopes.front()->name() << std::endl;
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
	mSpecialWord = static_cast<int32_t>(SpecialWord::None);
}

void NateParser::endCode()
{
	curCode().endDecl();
	popScope();
}

Code& NateParser::curCode() { return mCodes.back(); }

void NateParser::addDefine()
{
	pushScope("define");
	mDefines.emplace_back();
	mMethodType = MethodType::Define;
	mSpecialWord = static_cast<int32_t>(SpecialWord::None);
}

void NateParser::declareDefine()
{
	*mOut << in(-1) << curDefine().createCodeDecl() << " {" << std::endl;
	curDefine().createCodeCall();
}

void NateParser::endDefine()
{
	curDefine().endDecl();
	popScope();
	*mOut << in() << "}" << std::endl;
}

Define& NateParser::curDefine() { return mDefines.back(); }

void NateParser::addArgWord(const std::string& aWord)
{
	if (aWord == "\\u")
	{
		mSpecialWord |= static_cast<int32_t>(SpecialWord::WantsUnary);
	}
	else if (aWord == "\\a")
	{
		mSpecialWord |= static_cast<int32_t>(SpecialWord::IsAlias);
	}
	else if (aWord == "\\l")
	{
		mSpecialWord |= static_cast<int32_t>(SpecialWord::LeftMonomial);
	}
	else
	{
		if (mSpecialWord & static_cast<int32_t>(SpecialWord::WantsUnary))
		{
			addWantsUnary(aWord);
		}
		if (mSpecialWord & static_cast<int32_t>(SpecialWord::LeftMonomial))
		{
			addLeftMonomial(aWord);
		}

		curMethod().addArgWord(aWord);
		mSpecialWord = static_cast<int32_t>(SpecialWord::None);
	}
}

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
		*mOut << "#line " << prevLine << " \"" << prevFile << "\"" << std::endl;
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
	else
	{
		name = aString;
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

		return IdentifierPtr();
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
		addIdentifier(std::make_shared<Identifier>(curScope(), aName, getType("int-32")));
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

		return TypePtr();
	}
	else
	{
		return aScope->getType(aName);
	}
}

void NateParser::addType(const TypePtr& aType, const std::string& aName)
{
	curScope()->addType(aType, aName);
}

TypePtr NateParser::determineType(const std::string& aName)
{
	auto type = getType(aName);
	return !type ? std::make_shared<Type>(aName) : type;
}

TypePtr NateParser::makeType(const std::string& aValue)
{
	TypePtr result = std::make_shared<Type>("");

	if (aValue[0] == '"')
	{
		result = getType("text");
	}
	else if (aValue == "false" || aValue == "true")
	{
		result = getType("bool");
	}
	else if (aValue.find('.') != std::string::npos)
	{
		/*double value;
		Core::strtodbl(aValue.c_str(), value);
		if (value > std::numeric_limits<float>::max() || 
		    (value < 0 && value < std::numeric_limits<float>::lowest()) ||
		    (value > 0 && value < std::numeric_limits<float>::min()))
		{
		  result = getType(float-64");
		}
		else
		{
		  result = getType("float-32");
		}*/
		result = getType("float-64");
	}
	else
	{
		int64_t value;
		Core::strtoi64(aValue.c_str(), value);
		if (value > std::numeric_limits<int32_t>::max() || value < std::numeric_limits<int32_t>::lowest())
		{
			result = getType("int-64");
		}
		else
		{
			result = getType("int-32");
		}
	}

	return result;
}

void NateParser::methodMatches(const Method& aMethod,
														   ExprNodesCIter& aStartIter, ExprNodesCIter& aEndIter,
														   Match& aMatch,
															 bool aDebug)
{
	Method::MatchResult matchResult;
	std::string errorMsg;
	std::tie(errorMsg, matchResult) = aMethod.checkArgTypes(aStartIter, aEndIter, aDebug);

	aMatch.matchResult = matchResult;

	if (matchResult != Method::MatchResult::No)
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
																			  Match& aMatch, 
																				bool aDebug)
{
	auto size = aMethod.args().size();

	for (auto startIter = aExpr.nodes().cbegin();
			 size <= static_cast<size_t>(std::distance(startIter, aExpr.nodes().cend())); ++startIter)
	{
		auto endIter = startIter + size;
		
		if (aMethod.matches(startIter, endIter, aDebug) &&
				(aMatch.methodFound == nullptr || 
				 aMethod.priority() > aMatch.methodFound->priority() ||
				 (aMethod.priority() == aMatch.methodFound->priority() &&
          startIter < aMatch.nodeStartIter)))
		{
			methodMatches(aMethod, startIter, endIter, aMatch, aDebug);
		}
	}
}

void NateParser::checkRightToLeftMethod(const Method& aMethod,
																			  const Expr& aExpr,
																			  Match& aMatch, 
																				bool aDebug)
{
	auto size = aMethod.args().size();

	for (auto endIter = aExpr.nodes().cend();
			 size <= static_cast<size_t>(std::distance(aExpr.nodes().cbegin(), endIter)); --endIter)
	{
		auto startIter = endIter - size;
		
		if (aMethod.matches(startIter, endIter, aDebug) &&
				(aMatch.methodFound == nullptr || 
				 aMethod.priority() > aMatch.methodFound->priority() ||
				 (aMethod.priority() == aMatch.methodFound->priority() &&
          startIter > aMatch.nodeStartIter)))
		{
			 methodMatches(aMethod, startIter, endIter, aMatch, aDebug);
		}
	}
}

void NateParser::checkIfMethod(const Method& aMethod, const Expr& aExpr, Match& aMatch, bool aDebug)
{
	auto size = aMethod.args().size();
	if (size <= aExpr.nodes().size())
	{
		if (aMethod.is(Code::RightLeft))
		{
			checkRightToLeftMethod(aMethod, aExpr, aMatch, aDebug);
		}
		else
		{
			checkLeftToRightMethod(aMethod, aExpr, aMatch, aDebug);
		}
	}
}

Expr NateParser::evaluate(const Expr& aExpr, bool aDebug)
{
	Expr result = aExpr;
	if (aDebug) std::cerr << "+++++ " << aExpr.text() << " " << aExpr << std::endl;

	Match match;
	match.nodeStartIter = aExpr.nodes().cend();
	match.nodeEndIter = aExpr.nodes().cend();

	if (aExpr.nodes().size() > 1 || aExpr.node().is(ExprNode::Word))
	{
		for (auto const& code : mCodes)
		{
			checkIfMethod(code, aExpr, match, aDebug);
		}
		
		for (auto const& define : mDefines)
		{
			checkIfMethod(define, aExpr, match, aDebug);
		}

		if (match.methodFound)
		{
			std::string errorMsg;
			std::string methodStat;
			TypePtr methodType;
			Flags nodeFlags;
			std::tie(errorMsg, methodStat, methodType, nodeFlags) = 
				match.methodFound->evaluate(match.nodeStartIter, match.nodeEndIter, aDebug);
			if (!errorMsg.empty())
			{
				error(errorMsg);
			}
			if (aDebug) std::cerr << "******* " << (methodType ? *methodType : Type()) << " " << methodStat << std::endl;
			
			ExprNode node(methodStat, methodStat, methodType);
			node.setFlags(nodeFlags);

			Expr newExpr;
			newExpr.addNodes(aExpr.nodes().cbegin(), match.nodeStartIter);
			newExpr.addNode(node);
			newExpr.addNodes(match.nodeEndIter, aExpr.nodes().cend());
			//std::cerr << newExpr << std::endl;
			return evaluate(newExpr, aDebug);
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
		return Expr(ExprNode("1", getType("int-32")));
	}
	if (aDebug) std::cerr << result << std::endl;
	return result;
}

void NateParser::codeStartProgram()
{
	printLineNr();
	*mOut << in() << "int main(int argc, char** argv)\n{" << std::endl;
	pushScope("main");
	*mOut << in() << "SetConsoleOutputCP(65001);" << std::endl;
	//*mOut << "std::locale::global(std::locale(\"en_US.UTF8\"));" << std::endl;
    
}

void NateParser::codeEndProgram()
{
	popScope();
	*mOut << in() << "}" << std::endl;
}

void NateParser::codeStartScope()
{
	*mOut << in() << "{" << std::endl;
	pushScope("scope");
}
void NateParser::codeEndScope()
{
	popScope();
	*mOut << in() << "}" << std::endl;
}

void NateParser::codeDeclareLocalIdentifier(const IdentifierPtr& aIdentifier,
																						bool initializeNonScalars)
{
	addIdentifier(aIdentifier);
	if (aIdentifier->type()->is(Type::Abstract))
	{
		error("Abstract type: " + aIdentifier->type()->name());
		return;
	}

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
			*mOut << in() << "constexpr ";		
		}
		else
		{
			*mOut << in() << "const ";		
		}
	}

	*mOut << in() << aIdentifier->type()->codeType() << " " << aIdentifier->codeName();
	if (aIdentifier->type()->is(Type::Scalar) || initializeNonScalars)
	{
		*mOut << " = " << aIdentifier->initValue().code();
	}

	if (aIdentifier->is(Identifier::Const) && !aIdentifier->initValue().is(ExprNode::ConstExpr))
	{
		error("Expected constant expression.");
	}

	*mOut << ";" << std::endl;
}

void NateParser::codeDeclareLocalIdentifiers(bool aConst,
																						 const std::vector<std::string>& aNames,
																						 const TypePtr& aType,
																					 	 const std::vector<Expr>& aInitValues,
																						 bool initializeNonScalars)
{
	//std::cout << Core::join(aNames, ", ") << ":" << aType << ":" << Core::join(aInitValues, ", ") << std::endl;

	if (aType->empty() && aInitValues.empty())
	{
		error("expected either type or initial value.");
	}
	else if (aType->empty() && (aInitValues.size() != 1 && aInitValues.size() != aNames.size()))
	{
		error("expected as many initial values as identifiers.");
	}

	auto initIter = aInitValues.cbegin();
	TypePtr type(aType);

	for (auto const& name : aNames)
	{
		if (getIdentifier(name, curScope().get()))
		{
			error("duplicate declaration of: " + name);
		}
		Expr initValue;
		if (aInitValues.empty())
		{
			initValue = Expr(ExprNode("default", "{}", type));
			initValue.node().setFlag(ExprNode::Default);
			if (aConst)
			{
				error("Expected initial values for constants");
			}
		}
		else if (aInitValues.size() != 1 || initIter == aInitValues.cbegin())
		{
			if (type->empty())
			{
				type = initIter->type();
			}
			else if (!type->canBeCastedFrom(initIter->type()))
			{
				error("Incompatible type for initial value: " + initIter->code());
			}

			initValue = *initIter++;
			initValue.node().castToType(aType);
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
	*mOut << in() << "struct " << aRecord->codeType() << " {" << std::endl;
	pushScope(aRecord->scope());
}

void NateParser::codeDeclareRecordIdentifiers(bool aConst,
																							const std::vector<std::string>& aNames,
																							const TypePtr& aType,
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
	popScope();
	*mOut << in() << "};" << std::endl;
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
				
			*mOut << in() << expr.code() << " = ";
		}
	}

	*mOut << aValue.code() << ";" << std::endl;
}

std::string NateParser::codeId(const std::string& aName, Scope* aScope)
{
	return getOrFakeIdentifier(aName, aScope)->codeName();
}

void NateParser::codeOutputStart(const std::string& aStream)
{
	mStream = aStream;
	printLineNr();
	*mOut << in() << aStream;
	mCachedOutput.clear();
}

void NateParser::codeOutput(const std::string& aString)
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
			Expr outExpr(Expr("stream-out"), aValue);
			Expr resExpr = evaluate(outExpr);
			if (!resExpr.isEmpty())
			{
				codeOutput("(" + resExpr.code() + ")");
			}
			else
			{
				codeOutput("(" + aValue.code() + ")");
			}
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

	*mOut << ";" << std::endl;
}

void NateParser::codeInputStart(const std::string& aStream)
{
	mStream = aStream;
	printLineNr();
	*mOut << in() << aStream;
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
			*mOut << ">> std::boolalpha ";
		}
	
		*mOut << ">> " << aValue.code();
	}
	else
	{
		error("Expected non-constant variable for input");
	}
}

void NateParser::codeInputEnd(bool aAddEnd)
{
	*mOut << ";";
	if (aAddEnd)
	{
		*mOut << mStream << ".ignore(std::numeric_limits<std::streamsize>::max(), '\\n');";
	}
	*mOut << std::endl;
}

void NateParser::NateParser::codeIf(const Expr& aValue)
{
	printLineNr();
	if (!aValue.type()->is(Type::Boolean))
	{
		error("Expected boolean expression for IF statement");
	}

	*mOut << in() << "if (" << aValue.code() << ") {" << std::endl;
	pushScope("if");
}

void NateParser::codeElseIf()
{
	*mOut << in() << "else " << std::endl;
}

void NateParser::codeElse()
{
	printLineNr();
	*mOut << in() << "else {" << std::endl;
	pushScope("else");
}

void NateParser::codeEndIf()
{
	popScope();
	*mOut << in() << "}" << std::endl;
}

void NateParser::codeIfIs(const Expr& aValue, const std::string& idName)
{
	printLineNr();
	IfIs info;
	info.idName = idName;
	info.isSwitch = aValue.type()->is(Type::Scalar);
	info.out = std::make_shared<std::ostringstream>();
	info.savedOut = mOut;
	mIfIs.push(info);
	
	*mOut << in() << "auto const " << idName << " = " << aValue.code() << ";" << std::endl;
}

void NateParser::codeIs(const Expr& aValue, const Expr& aIfExpr)
{
	auto ifIs = mIfIs.top();
	if (!(aValue.type()->isOfType(aIfExpr.type()->name()) ||
				aValue.type()->is(Type::Real) == aIfExpr.type()->is(Type::Real)))
	{
		error("Expected expression with same type as in IF");
	}

	auto prevCase = ifIs.nextCase;
	ifIs.nextCase = (ifIs.isSwitch && aValue.is(ExprNode::ConstExpr) && ifIs.isFirstTest);

	if (ifIs.isFirstTest && prevCase && !ifIs.nextCase)
	{
		error("Non-constant expression needs to be first in multi-IS");
	}

	if (ifIs.nextCase)
	{
		mOut = ifIs.out.get();
		printLineNr();
		*mOut << in() << "case " << aValue.code() << ":" << std::endl;
	}
	else
	{
		bool firstExpr = ifIs.isFirstTest;
		
		if (!ifIs.isFirst && firstExpr)
		{
			printLineNr();
			*mOut << in() << "else ";
		}

		if (firstExpr)
		{
			if (ifIs.isFirst)
			{
				printLineNr();
			}

			*mOut << in() << "if (";
		}
		else
		{
			*mOut << std::endl;			
			printLineNr();
			*mOut << in(2) << " || ";
		}
		
		if (ifIs.isFirst || firstExpr)
		{
			ifIs.isFirst = false;
			ifIs.isFirstTest = false;
		}

		*mOut << "(" << mIfIs.top().idName << " == " << aValue.code() << ")";
	}

	mIfIs.pop();
	mIfIs.push(ifIs);
}

void NateParser::codeElseIs()
{
	printLineNr();
	auto ifIs = mIfIs.top();
	if (!ifIs.isFirst)
	{
		*mOut << in() << "else" << std::endl;
	}

	ifIs.nextCase = ifIs.isSwitch && !ifIs.out->str().empty();
	if (ifIs.nextCase)
	{
		*mOut << in() << "switch (" << ifIs.idName << ") {" << std::endl;
		*mOut << in() << ifIs.out->str();
		*mOut << in() << "default:" << std::endl;
		mOut = ifIs.savedOut;
	}
	else
	{
		ifIs.nextElse = true;
	}

	mIfIs.pop();
	mIfIs.push(ifIs);
}

void NateParser::codeBeginIs()
{
	auto ifIs = mIfIs.top();

	if (!ifIs.nextCase && !ifIs.nextElse)
	{
		*mOut << ")" << std::endl;
	}
	
	*mOut << in() << "{" << std::endl;
	pushScope("is");
}

void NateParser::codeEndIs()
{
	auto ifIs = mIfIs.top();
	printLineNr();

	if (ifIs.nextCase)
	{
		*mOut << in() << "break;" << std::endl;
	}

	popScope();
	*mOut << in() << "}" << std::endl;

	if (ifIs.nextCase)
	{
		mOut = ifIs.savedOut;
	}
	
	mIfIs.pop();
	ifIs.isFirstTest = true;
	ifIs.nextCase = false;
	mIfIs.push(ifIs);
}

void NateParser::codeEndIfIs()
{
	auto ifIs = mIfIs.top();
	printLineNr();
	if (mIfIs.top().isSwitch && !ifIs.out->str().empty())
	{
		*mOut << in() << "}" << std::endl;
	}

	mIfIs.pop();
}

void NateParser::codeInitLoop()
{
	printLineNr();
	mLoopWhileCounts.push_back(0);
	pushScope("while");
}

void NateParser::codeStartLoop()
{
	*mOut << in(-1) << "while (true) {" << std::endl;
}

void NateParser::codeStartForStepLoop(const std::string& aId,
																			const TypePtr& aType,   
																			bool aDownTo,
																			const Expr& aStart,
																			const Expr& aEnd,
																			const Expr& aStep)
{
	TypePtr type = aType->empty()
							   ? aStart.type()
							   : aType;

	IdentifierPtr id = std::make_shared<Identifier>(curScope(), aId, type);
	addIdentifier(id);

	*mOut << in(-1) << "for (" << id->type()->codeType() << " " 
			 << id->codeName() << "= " << aStart.code() << ";" 
			 << id->name() << (aDownTo ? " >= " : "<=") << aEnd.code() << ";"
			 << id->name() << (aDownTo ? " -= " : "+=") << aStep.code() << ") {" << std::endl;
}

void NateParser::codeStartForRangeLoop(const std::string& aId, 
																			 const Expr& aRange)
{
	TypePtr rangeType = aRange.type();
	if (rangeType->isOfType("container"))
	{
		TypePtr type = rangeType->typenameType();
		IdentifierPtr id = std::make_shared<Identifier>(curScope(), aId, type);
		addIdentifier(id);

		auto range = uniqueName();
		auto iter = uniqueName();
		auto next = uniqueName();

		std::string ref = aRange.is(ExprNode::Output) ? "&" : "";
		std::string increment;

		if (rangeType->isOfType("text"))
		{
			increment = iter + "=" + next;
			*mOut << in(-1) << "const std::string" << ref << " " << range << " = " << aRange.code() << ";" << std::endl; 
			*mOut << in(-1) << "auto " << next << " = " << range << ".cbegin();" << std::endl;
		}
		else
		{
			increment = "++" + iter;
			*mOut << in(-1) << "auto const" << ref << " " << range << " = " << aRange.code() << ";" << std::endl; 
		}

		*mOut << in(-1) << "for (auto " << iter << " = " << range << ".cbegin(); "
				 << iter << " != " << range << ".cend(); "
			   << increment << ") {" << std::endl;
		if (rangeType->isOfType("text"))
		{
			*mOut << in() << "uint32_t " << id->codeName() << " = utf8::next(" + next + "," + range + ".cend());" << std::endl;
		}
		else
		{
			*mOut << in() << "auto const& " << id->codeName() << " = *" << iter << ";" << std::endl;
		}
	}
	else
	{
		error("Range must be a container, got: " + rangeType->name());
	}
}

void NateParser::codeEndLoop()
{
	mLoopWhileCounts.pop_back();
	popScope();
	*mOut << in() << "}" << std::endl;
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
	*mOut << in() << "if (!(" << aExpr.code() << ")) break;" << std::endl;
}

 void NateParser::codeReturn(const Expr& aValue)
 {
	 printLineNr();
	 *mOut << in() << "return " << aValue.code() << ";" << std::endl;
 }

 void NateParser::codeExpression(const Expr& aValue)
 {
	 printLineNr();
	 *mOut << in() << aValue.code() << ";" << std::endl;
 }