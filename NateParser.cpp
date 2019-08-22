#include "NateParser.h"
#include "core/Core.h"
#include "lex.yy.h"
#include "NateParser.tab.h"
#include <algorithm>
#include <inttypes.h>
#include <cctype>
#include <tuple>

bool gDebug = true && false
;

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
	*mOut << "#include <cstdint>" << std::endl;
	*mOut << "#include <cmath>" << std::endl;
	*mOut << "#include <iostream>" << std::endl;
	*mOut << "#include <algorithm>" << std::endl;
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
		
	addType(std::make_shared<Type>("fraction", getType("number")));
	addType(std::make_shared<Type>("imaginary", getType("number")));
	getType("imaginary")->setTypenameType(getType("float-64"));
	addType(std::make_shared<Type>("complex", getType("number")));
	getType("complex")->setTypenameType(getType("float-64"));

	addType(std::make_shared<Type>("object", getType("any")));
	addType(std::make_shared<Type>("record", getType("object")));
	addType(std::make_shared<Type>("container", getType("object")));
	addType(std::make_shared<Type>("sequence-container", getType("container")));
	addType(std::make_shared<Type>("list", getType("sequence-container")));
	addType(std::make_shared<Type>("text", getType("sequence-container")));
	addType(std::make_shared<Type>("char", getType("any")));
	getType("text")->setTypenameType(getType("char"));

	addType(std::make_shared<Type>("output", getType("object")));
	addType(std::make_shared<Type>("file-output", getType("output")));

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
		std::string library = "C:\\Users\\ruud\\source\\repos\\Nate\\core";
		std::string path = library + Core::directorySeperator() + aName + ".ns";
		if (Core::exists(path))
		{
			mLexer->includeFile(path);
		}
		else
		{
			library = ".";
			std::string path = library + Core::directorySeperator() + aName + ".nd";
			if (Core::exists(path))
			{
				mLexer->includeFile(path);
			}
			else
			{
				error("No path to import: " + aName);
			}
		}
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

IRecordHolderPtr& NateParser::curRecordHolder()
{
	return mRecordHolders.front();
}

void NateParser::pushRecordHolder(const IRecordHolderPtr& aRecordHolder)
{
	mRecordHolders.push_front(aRecordHolder);
}

void NateParser::popRecordHolder()
{
	mRecordHolders.pop_front();
}

ITypeHolderPtr& NateParser::curTypeHolder()
{
	return mTypeHolders.front();
}

void NateParser::pushTypeHolder(const ITypeHolderPtr& aTypeHolder)
{
	mTypeHolders.push_front(aTypeHolder);
}

void NateParser::popTypeHolder()
{
	mTypeHolders.pop_front();
}

void NateParser::pushScope(const std::string& aName)
{
	pushScope(std::make_shared<Scope>(aName));
}

void NateParser::pushScope(const ScopePtr& aScope)
{
	if (mLexer->debug()) std::cerr << "push " << aScope->name() << std::endl;
	mScopes.push_front(aScope);
	pushRecordHolder(aScope);
	pushTypeHolder(aScope);
}

void NateParser::popScope()
{
	mScopes.pop_front();
	popRecordHolder();
	popTypeHolder();
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

void NateParser::addObject(const ObjectPtr& aObject)
{
	if (getType(aObject->name()))
	{
		error("Redefinition of type: " + aObject->name());
	}
	else if (getObject(aObject->name()))
	{
		error("Redefinition of object: " + aObject->name());
	}

	addType(aObject, aObject->name());
	
	pushRecordHolder(aObject);
	pushTypeHolder(aObject);
	mCurObject = aObject;

	mObjects.push_back(aObject);
}

void NateParser::endObject()
{
	popRecordHolder();
	popTypeHolder();
}

ObjectPtr NateParser::getObject(const std::string& aId)
{
	auto iter = std::find_if(mObjects.cbegin(), mObjects.cend(),
												 	 [&](ObjectPtr const& aObject)
													 { return aObject->name() == aId; });
	return iter != mObjects.cend() ? *iter : ObjectPtr();
}

ObjectPtr NateParser::curObject()
{
	return mCurObject;
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
	auto size = curCode().code().size();
	if (size > 0 && curCode().code()[size - 1] == '\n')
	{
		curCode().code().pop_back();
	}

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

void NateParser::declareDefine(bool aIsDecl)
{
	mDefineDecl = aIsDecl;
	*mOut << in(-1) << curDefine().createCodeDecl() << (aIsDecl ? ";" : "{") << std::endl;
	curDefine().createCodeCall();
}

void NateParser::endDefine()
{
	curDefine().endDecl();
	popScope();
	mLastWriteStream.clear();
	if (!mDefineDecl)
	{
		*mOut << in() << "}" << std::endl;
	}
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
	std::cerr << mLexer->fileLocation() << ": " << anError << std::endl;
	++mErrors;
}

void NateParser::warning(const std::string& aWarning)
{
	std::cerr << "Warning: " << mLexer->fileLocation() << ": " << aWarning << std::endl;
	++mWarnings;
}

void NateParser::printLineNr()
{
	static int prevLine = 0;
	static std::string prevFile;

	if (mLexer->has_matcher())
	{
		auto const& begin = mLexer->location().begin;
		if (prevLine + 1 != begin.line || prevFile != mLexer->filenames.back())
		{
			prevLine = begin.line;
			prevFile = mLexer->filenames.back();
			*mOut << "#line " << prevLine << " \"" << prevFile << "\"" << std::endl;
		}
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

bool NateParser::isReservedName(const std::string& aString) const
{
	return std::any_of(mAliases.cbegin(), mAliases.cend(),
										 [&](const auto& pair) 
										 { return pair.first == aString || pair.second == aString; });
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
				pos = utf8::distance(iter, next);
				first = false;
			}
			
			utf8::next(iter, name.cend());
		}
	}

	if (id)
	{
		if (pos != 0)
		{
			auto iterOrig = aOrig.cbegin();
			auto iter = aString.cbegin();
			utf8::advance(iterOrig, pos, aOrig.cend());
			utf8::advance(iter, pos, aString.cend());
			name = std::string(aString.cbegin(), iter);
			unput(iterOrig, aOrig.cend());
			//std::cerr << "Orig: " << aOrig << std::endl;
			//std::cerr << "Pos: " << pos << std::endl;
			//std::cerr << "Name: " << name << std::endl;
			//std::cerr << "Unput: " << std::string(iter, aOrig.cend()) << std::endl;
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

TypePtr NateParser::getType(const std::string& aName, ITypeHolder* aTypeHolder)
{
	if (aTypeHolder == nullptr)
	{
		for (auto& scope : mScopes)
		{
			auto type = scope->getType(aName);
			if (type)
			{
				return type;
			}
		}
		
		return TypePtr();
	}
	else
	{
		return aTypeHolder->getType(aName);
	}
}

void NateParser::addType(const TypePtr& aType, const std::string& aName)
{
	curTypeHolder()->addType(aType, aName);
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
		result = getType("float-64");
	}
	else if (aValue.find('E') != std::string::npos)
	{
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

TypePtr NateParser::getNumberType(std::string& aString)
{
  TypePtr result;
  bool isFloat = false;

  if (aString.size() > 2 &&
      aString[0] == '0' && (aString[1] == 'f' || aString[1] == 'F'))
  {
    aString = aString.substr(2);
    isFloat = true;
  }

  Core::parseBaseNumber(aString);
	        
  if (isFloat)
  {
		if (aString.find('.') == std::string::npos && aString.find('E') == std::string::npos)
		{
			aString += ".0";
		}

    aString += 'f';
    result = getType("float-32");
  }
  else
  {
    result = makeType(aString);
  }

	return result;
}

void NateParser::checkIfBetterMatch(const Method& aMethod,
																		const ExprNodesCIter& aStartIter,
																		const ExprNodesCIter& aEndIter,
																		Match& aMatch,
																		bool aLeftToRight,
																		bool aDebug)
{
	if (aMethod.matches(aStartIter, aEndIter, aDebug) &&
			(aMatch.methodFound == nullptr || 
			 (aMethod.priority() > aMatch.methodFound->priority() ||
			  (aMethod.priority() == aMatch.methodFound->priority() &&
         ((aLeftToRight  && aStartIter <= aMatch.nodeStartIter) ||
				  (!aLeftToRight && aStartIter >= aMatch.nodeStartIter))))))
	{
		Method::MatchResult matchResult = aMethod.checkArgTypes(aStartIter, aEndIter, aDebug);
		
		if (matchResult.matches)
		{
			if ((aMatch.methodFound == nullptr || 
					 aMethod.priority() > aMatch.methodFound->priority() ||
					 (aMethod.priority() == aMatch.methodFound->priority() &&
						((aLeftToRight  && aStartIter < aMatch.nodeStartIter) ||
						 (!aLeftToRight && aStartIter > aMatch.nodeStartIter)))))
			{
				if (aDebug)
				{
					std::cerr << "Better match 1: " << " "
										<< aMethod.code() 
										<< matchResult.castCount << " "
										<< (aMatch.methodFound == nullptr ? "None" : aMatch.methodFound->code()) << " "
										<< aMatch.matchResult.castCount << " "
										<< std::distance(aStartIter, aMatch.nodeStartIter) << " "
										<< std::distance(aEndIter, aMatch.nodeEndIter) << " "
										<< std::endl;
				}

				aMatch.matchResult = matchResult;
				aMatch.methodFound = &aMethod;
				aMatch.nodeStartIter = aStartIter;
				aMatch.nodeEndIter = aEndIter;
			}
			else if (aMethod.priority() == aMatch.methodFound->priority() &&
							 ((aLeftToRight  && aStartIter == aMatch.nodeStartIter) ||
								(!aLeftToRight && aStartIter == aMatch.nodeStartIter)) &&
							 matchResult.castCount < aMatch.matchResult.castCount)
			{

				if (aDebug)
				{
					std::cerr << "Better match 2: " << " "
										<< aMethod.code() 
										<< matchResult.castCount << " "
										<< (aMatch.methodFound == nullptr ? "None" : aMatch.methodFound->code()) << " "
										<< aMatch.matchResult.castCount << " "
										<< std::distance(aStartIter, aMatch.nodeStartIter) << " "
										<< std::distance(aEndIter, aMatch.nodeEndIter) << " "
										<< std::endl;
				}

				aMatch.matchResult = matchResult;
				aMatch.methodFound = &aMethod;
				aMatch.nodeStartIter = aStartIter;
				aMatch.nodeEndIter = aEndIter;
			}
			else
			{
				if (aDebug)
				{
					std::cerr << "No match1: " << " "
										<< aMethod.code() << " "
										<< matchResult.castCount << " "
										<< (aMatch.methodFound == nullptr ? "None" : aMatch.methodFound->code()) << " "
										<< aMatch.matchResult.castCount << " "
										<< std::distance(aStartIter, aMatch.nodeStartIter) << " "
										<< std::distance(aEndIter, aMatch.nodeEndIter) << " "
										<< std::endl;
				}

				aMatch.matchedMethod = &aMethod;
				aMatch.matchResult = matchResult;
			}
		}
		else
		{
			if (aDebug)
			{
				std::cerr << "No match2: " << " "
					        << aMethod.code() << " "
					        << matchResult.castCount << " "
									<< (aMatch.methodFound == nullptr ? "None" : aMatch.methodFound->code()) << " "
					        << aMatch.matchResult.castCount << " "
									<< std::distance(aStartIter, aMatch.nodeStartIter) << " "
									<< std::distance(aEndIter, aMatch.nodeEndIter) << " "
									<< std::endl;
			}

			aMatch.matchedMethod = &aMethod;
			aMatch.matchResult = matchResult;
		}
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
		checkIfBetterMatch(aMethod, startIter, endIter, aMatch, true, aDebug);
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
		checkIfBetterMatch(aMethod, startIter, endIter, aMatch, false, aDebug);
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
	aDebug = gDebug || aDebug;
	if (aDebug) std::cerr << "+++++ " << aExpr.text() << " " << aExpr << std::endl;

	Match match;
	match.nodeStartIter = aExpr.nodes().cend();
	match.nodeEndIter = aExpr.nodes().cend();

	if (aExpr.nodes().size() > 1 || aExpr.node().is(ExprNode::Word))
	{
		for (auto const& object : mObjects)
		{		
			for (auto const& define : object->getDefines())
			{
				checkIfMethod(define, aExpr, match, aDebug);
			}
		}

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
			Method::EvaluateResult evalResult = 
				match.methodFound->evaluate(match.nodeStartIter, match.nodeEndIter, aDebug);
			if (!evalResult.error.empty())
			{
				error(evalResult.error);
			}
			else if (evalResult.code == "NI")
			{
				error("Not implemented: " + aExpr.text());
			}

			if (aDebug) std::cerr << "******* " << (evalResult.type ? *evalResult.type : Type()) << " " << evalResult.code << std::endl;
			
			ExprNode node(evalResult.origText, evalResult.code, evalResult.type);
			node.setFlags(evalResult.flags);

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
				if (!match.matchResult.error.empty())
				{
					error("Bad argument types for code: " + match.matchedMethod->signature() + ": " + 
								match.matchResult.error);
				}
			}
		}
	}

	if (aExpr.nodes().size() > 1 || aExpr.nodes().front().is(ExprNode::Word))
	{
		error("Bad expression: " + aExpr.text());
		return Expr(ExprNode("1", getType("int-32")));
	}

	handleCompileCommands(result);

	if (aDebug) std::cerr << result << std::endl;
	return result;
}

std::string NateParser::handleCompileCommand(const std::string& aCommand, const std::string& aData)
{
	std::string result = aData;

	if (aCommand == "GETFORMAT")
	{
	  static const std::string preData = "Core::getFormat(";
	  static const std::string postData = ")";
		size_t endPre = preData.size();

		if (aData.substr(0, endPre) == preData && aData.back() == ')')
		{
			// Core::getFormat("..."), literal string
			if (aData[endPre] == '"')
			{
				size_t endString = aData.find('"', endPre + 1);
				if (endString == aData.size() - 2)
				{
					Core::Format format = Core::getFormat(aData.substr(endPre + 1, endString - endPre - 1));
					result = format.toString();
					if (!format.error.empty())
					{
						error(format.error);
					}
				}
			}
		}
		else
		{
			error("Unknown compile data: " + aData);
		}
	}
	else
	{
		error("Unknown compile command: " + aCommand);
	}

	return result;
}

void NateParser::handleCompileCommands(Expr& aExpr)
{
	size_t pos;
	std::string& code = aExpr.node().code();
	bool ok = true;

	do
	{
		pos = code.find("__X__");
		if (pos != std::string::npos)
		{
			size_t commandStart = pos + 5;
			size_t commandEnd = code.find("__", commandStart);
			if (commandEnd != std::string::npos)
			{
				size_t dataStart = commandEnd + 2;
				size_t dataEnd = code.find("__", dataStart);
				if (dataEnd != std::string::npos)
				{
					code.replace(pos, dataEnd + 2 - pos,
											 handleCompileCommand(code.substr(commandStart, commandEnd - commandStart),
																					  code.substr(dataStart, dataEnd - dataStart)));
				}
				else
				{
					ok = false;
				}
			}
			else
			{
				ok = false;
			}
		}
	} while (pos != std::string::npos && ok);

	if (!ok)
	{
		error("Badly formatted compile command in: " + code);
	}
}

void NateParser::codeStartProgram()
{
	printLineNr();
	*mOut << in() << "int main(int argc, char** argv)\n{" << std::endl;
	pushScope("main");
	*mOut << in() << "output = std::shared_ptr<std::ostream>(&std::cout, [](void*) {});" << std::endl;
	*mOut << in() << "error = std::shared_ptr<std::ostream>(&std::cerr, [](void*) {});" << std::endl;
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

void NateParser::codeCodeInclude()
{
	*mOut << in() << mCodes.back().code() << std::endl;
	mCodes.pop_back();
}

void NateParser::codeDeclareLocalIdentifier(const IdentifierPtr& aIdentifier,
																						bool initializeObjects)
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
	//if (aIdentifier->type()->is(Type::Scalar) || initializeNonScalars)
	if (!aIdentifier->initValue().is(ExprNode::Default) || 
			aIdentifier->type()->is(Type::SingleNr) || 
			initializeObjects)
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
																						 bool initializeObjects)
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
		else if (getIdentifier(name))
		{
			if (isReservedName(name))
			{
				error("reserved name: " + name);
			}
			else
			{
				warning("hides declaration of: " + name);
			}
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
			else if (type->canBeCastedFrom(initIter->type()) == Type::CompareResult::No)
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

		codeDeclareLocalIdentifier(id, initializeObjects);
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
		if (!id->type()->is(Type::SingleNr) && !id->initValue().is(ExprNode::Default))
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

void NateParser::codeStartObject(const ObjectPtr& aObject, bool aIsDecl)
{
	ObjectInfo info;
	info.object = aObject;
	info.isDecl = aIsDecl;
	info.savedOut = mOut;

	if (aIsDecl)
	{
		std::string tempDir = Core::currentDirectory() + Core::directorySeperator() + "created";
		if (!Core::isDirectory(tempDir))
		{
			Core::makeDirectory(tempDir);
		}

		if (!Core::isDirectory(tempDir) || !Core::isWritable(tempDir))
		{
			error("Can't create include directory: " + tempDir);
		}
		else
		{
			info.filename = tempDir + Core::directorySeperator() + aObject->name() + "." + "h";
			info.out = std::make_shared<std::ofstream>(info.filename);
			if (!info.out->good())
			{
				error("Can't create include file: " + info.filename);
			}
			else
			{
				*mOut << "#include \"" << info.filename << "\"" << std::endl;

				mOut = info.out.get();
				*mOut << "#pragma once" << std::endl;
			}
		}										
	}

	*mOut << "class " << toCodeName(aObject->name()) << std::endl;
	*mOut << "{" << std::endl;
	*mOut << "public:" << std::endl;

	mObjectInfo.push(info);
}

void NateParser::codeEndObject()
{
	ObjectInfo info = mObjectInfo.top();
	*mOut << "};" << std::endl;

	mOut = info.savedOut;

	mObjectInfo.pop();
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

void NateParser::codeWriteStart(const Expr& aValue)
{
	if (aValue.isEmpty())
	{
		if (!mLastWriteStream.empty())
		{
			mStream = mLastWriteStream;
		}
		else
		{
			error("Need to specify where to write to");
		}
	}
	else if (aValue.type()->isOfType("output"))
	{
		mStream = "*" + aValue.code();
		mLastWriteStream = mStream;
	}
	else
	{
		error("Cannot write to type: " + aValue.type()->name());
	}

	printLineNr();
	mFirstOutput = true;
	mCachedOutput.clear();
}

void NateParser::codeOutputStart(const std::string& aStream)
{
	mStream = aStream;
	printLineNr();
	mFirstOutput = true;
	mCachedOutput.clear();
}

void NateParser::codeOutputNew()
{
	if (mFirstOutput)
	{
		*mOut << in() << mStream;
		mFirstOutput = false;
	}
}

void NateParser::codeOutput(const std::string& aString)
{
	if ((!mCachedOutput.empty()) && aString[0] != '"')
	{
		codeOutputNew();
		*mOut << " << \"" << mCachedOutput << "\"";
		mCachedOutput.clear();
		if (!aString.empty())
		{
			codeOutputNew();
			*mOut << " << " << aString << ";";
			mFirstOutput = true;
		}
		else
		{
			*mOut << ";";
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
			*mOut << " << " << aString << ";";
			mFirstOutput = true;
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
		codeOutputNew();
		codeOutput("std::endl");
	}
	else
	{
		codeOutput("");
	}

	*mOut << std::endl;
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