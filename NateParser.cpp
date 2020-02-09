#include "NateParser.h"
#include "core/Core.h"
#include "lex.yy.h"
#include <algorithm>
#include <inttypes.h>
#include <cctype>
#include <tuple>

bool gDebug = true && false
;

NateParser::NateParser(const std::string& aFilename, std::istream& aIn, std::ostream& aOut,
											 FileType aFileType)
: mLexer(new yy::Lexer(aIn)),
	mParser(new yy::parser(*mLexer, *this)),
	mOut(&aOut),
	mFileType(aFileType),
	mFileName(aFilename),
	mLibrary("C:\\Users\\ruud\\source\\repos\\Nate\\core")
{
	mLexer->nate = this;
	mLexer->filenames.push_back(Core::replaceAll(aFilename, "\\", "\\\\"));
	  
	initOutput();
	initTypesAndObjects();
}

NateParser::~NateParser() = default;

void NateParser::initTypesAndObjects()
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

	if (mFileType == FileType::Normal)
	{
		codeDeclareLocalIdentifier(false, std::make_shared<Identifier>(curIdentifiersHolder(), "output", getType("output")), InitializeVariables);
		codeDeclareLocalIdentifier(false, std::make_shared<Identifier>(curIdentifiersHolder(), "error", getType("output")), InitializeVariables);
	}
	else
	{	
		addIdentifier(std::make_shared<Identifier>(curIdentifiersHolder(), "output", getType("output")));
		addIdentifier(std::make_shared<Identifier>(curIdentifiersHolder(), "error", getType("output")));
	}
}

void NateParser::initOutput()
{
	if (mFileType == FileType::ObjectDecl)
	{
		*mOut << "#pragma once" << std::endl;
	}

	*mOut << "#include \"C:\\Users\\ruud\\source\\repos\\Nate\\core\\Core.h\"" << std::endl;
}

int NateParser::parse()
{
	if (mLexer->debug())
	{
		std::cerr << "Seperate parsing: " << mFileName << std::endl;
	}

	if (mFileType != FileType::ObjectDecl)
	{
		for (auto file : { "C:\\Users\\ruud\\source\\repos\\Nate\\core\\core.ns" })
		{
			parseFile(file);
		}
	}
		
	auto result = mParser->parse();

	if (mLexer->debug())
	{
		std::cerr << "End seperate parsing: " << mFileName << std::endl;
	}

	return result;
}

void NateParser::parseFile(const std::string& aFilename)
{
	std::ifstream stream(aFilename);
	yy::Lexer lexer(stream);
	lexer.nate = this;
	lexer.filenames.push_back(Core::replaceAll(aFilename, "\\", "\\\\"));
	yy::parser parser(lexer, *this);
	if (mLexer->debug())
	{
		std::cerr << "Parsing: " << aFilename << std::endl;
	}
	parser.parse();
	if (mLexer->debug())
	{
		std::cerr << "End parsing: " << aFilename << std::endl;
	}
}

std::string NateParser::in(int aOffset) const
{
	int size = static_cast<int>(mTypesHolders.size()) + aOffset - 1; 
	if (mCurObject && mOut == &mCurObject->getImplOut())
	{
		++size;
	}
	return std::string(size, '\t');
}

std::string NateParser::makeTempDir()
{
	std::string tempDir = Core::currentDirectory() + Core::directorySeperator() + "created";
	if (!Core::isDirectory(tempDir))
	{
		Core::makeDirectory(tempDir);
	}

	if (!Core::isDirectory(tempDir) || !Core::isWritable(tempDir))
	{
		error("Can't create temp directory: " + tempDir);
	}

	return tempDir;
}

std::string NateParser::baseObjectName() const
{
	return "BaseObject";
}

void NateParser::importBaseObject(const std::string& aInObjectName)
{
	auto baseName = baseObjectName();
	if (aInObjectName != baseName)
	{
		if (mImports.find(baseName) == mImports.cend())
		{
			importObjectDefinition(mLibrary, baseName);
		}
	}
}

bool NateParser::importObjectDefinition(const std::string& aLibrary, const std::string& aName)
{
	bool found = false;

	std::string path = aLibrary + Core::directorySeperator() + aName + ".nd";
	if (Core::exists(path))
	{
		found = true;
		std::string tempDir = makeTempDir();
		std::ifstream in(path);
		std::string outPath = tempDir + Core::directorySeperator() + aName + "." + "h";
		std::ofstream out(outPath);
		if (!out.good())
		{
			error("Can't create include file: " + outPath);
		}
		else
		{
			NateParser nate(path, in, out, FileType::ObjectDecl);
			if (mLexer->debug())
			{
				std::cerr << "Importing: " << path << std::endl;
			}
			
			auto parseResult = nate.parse();

			mErrors += nate.errorCount();
			mWarnings += nate.warningCount();
			*mOut << "#include \"" << outPath << "\"" << std::endl;

			for (auto const& object : nate.mObjects)
			{
				if (!getObject(object->name()))
				{
					mObjects.push_back(object);
					addType(object, object->name());
				}
			}

			if (mLexer->debug())
			{
				std::cerr << "End importing: " << path << std::endl;
			}
		}
	}

	return found;
}

void NateParser::import(const std::string& aName)
{
	if (mImports.find(aName) == mImports.cend())
	{
		mImports.insert(aName);

		std::string library = mLibrary;
		std::string path = library + Core::directorySeperator() + aName + ".ns";
		if (Core::exists(path))
		{
			mLexer->includeFile(path);
		}
		else
		{
			if (!importObjectDefinition(library, aName))
			{ 
				if (!importObjectDefinition(".", aName))
				{
					error("No path to import: " + aName);
				}
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

IIdentifiersHolderPtr& NateParser::curIdentifiersHolder()
{
	return mIdentifiersHolders.front();
}

void NateParser::pushIdentifiersHolder(const IIdentifiersHolderPtr& aIdentifiersHolder)
{
	mIdentifiersHolders.push_front(aIdentifiersHolder);
}

void NateParser::popIdentifiersHolder()
{
	mIdentifiersHolders.pop_front();
}

IRecordsHolderPtr& NateParser::curRecordsHolder()
{
	return mRecordsHolders.front();
}

void NateParser::pushRecordsHolder(const IRecordsHolderPtr& aRecordsHolder)
{
	mRecordsHolders.push_front(aRecordsHolder);
}

void NateParser::popRecordsHolder()
{
	mRecordsHolders.pop_front();
}

ITypesHolderPtr& NateParser::curTypesHolder()
{
	return mTypesHolders.front();
}

void NateParser::pushTypesHolder(const ITypesHolderPtr& aTypesHolder)
{
	mTypesHolders.push_front(aTypesHolder);
}

void NateParser::popTypesHolder()
{
	mTypesHolders.pop_front();
}

IDefinesHolderPtr& NateParser::curDefinesHolder()
{
	return mDefinesHolders.front();
}

void NateParser::pushDefinesHolder(const IDefinesHolderPtr& aDefinesHolder)
{
	mDefinesHolders.push_front(aDefinesHolder);
}

void NateParser::popDefinesHolder()
{
	mDefinesHolders.pop_front();
}

void NateParser::pushScope(const std::string& aName)
{
	pushScope(std::make_shared<Scope>(aName));
}

void NateParser::pushScope(const ScopePtr& aScope)
{
	pushDefineScope(aScope);
	pushDefinesHolder(aScope);
}

void NateParser::popScope()
{
	popDefineScope();
	popDefinesHolder();
}

void NateParser::pushDefineScope(const std::string& aName)
{
	pushDefineScope(std::make_shared<Scope>(aName));
}

void NateParser::pushDefineScope(const ScopePtr& aScope)
{
	if (mLexer->debug()) std::cerr << "push " << aScope->name() << std::endl;
	mScopes.push_front(aScope);
	pushIdentifiersHolder(aScope);
	pushRecordsHolder(aScope);
	pushTypesHolder(aScope);
}

void NateParser::popDefineScope()
{
	mScopes.pop_front();
	popIdentifiersHolder();
	popRecordsHolder();
	popTypesHolder();
	if (mLexer->debug()) std::cerr << "pop to " << mScopes.front()->name() << std::endl;
}

Method& NateParser::curMethod()
{
	return mMethodType == MethodType::Code 
			   ? static_cast<Method&>(curCode()) 
		     : static_cast<Method&>(*curDefine());
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
	mObjects.push_back(aObject);
	startObject(aObject);
}

void NateParser::startObject(const ObjectPtr& aObject)
{
	setCurObject(aObject);
	pushIdentifiersHolder(aObject);
	pushRecordsHolder(aObject);
	pushTypesHolder(aObject);
	pushDefinesHolder(aObject);
}

void NateParser::endObject()
{
	popIdentifiersHolder();
	popRecordsHolder();
	popTypesHolder();
	popDefinesHolder();
	setCurObject(ObjectPtr());
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

void NateParser::setCurObject(const ObjectPtr& aObject)
{
	mCurObject = aObject;
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

void NateParser::addDefine(bool aInObject)
{
	curDefinesHolder()->defines().add(Define());
	mCurDefine = &curDefinesHolder()->defines().get().back();
	pushDefineScope("define");
	mMethodType = MethodType::Define;
	mSpecialWord = static_cast<int32_t>(SpecialWord::None);
	if (aInObject)
	{
		curDefine()->setObject(curObject().get());
	}
}

void NateParser::deleteCurDefine()
{
	curDefinesHolder()->defines().get().pop_back();
}

void NateParser::declareDefine(bool aIsDecl)
{
	curDefine()->endDecl();
	mDefineDecl = aIsDecl;

	if (curDefine()->isObjectMethod())
	{
		if (!aIsDecl)
		{
			Define* defineDecl = curObject()->defines().getLike(curDefine());
			if (defineDecl != nullptr)
			{
				if (defineDecl->is(Method::Defined))
				{
					error("Redefinition of method: " + defineDecl->signature());
				}

				defineDecl->setFlag(Method::Defined);
				mOut = &curObject()->getNormalOut();
				*mOut << in(-1) << curDefine()->createCodeDecl(toCodeName(curObject()->name())) << "\n" << in(-1) << "{" << std::endl;
			}
			else
			{
				curDefine()->setFlag(Method::Defined);
				curDefine()->setFlag(Method::Undeclared);
				mOut = &curObject()->getImplOut();
				*mOut << in(-1) << (curDefine()->isStatic() ? "static " : "") <<
					                 curDefine()->createCodeDecl() << "\n" << in(-1) << "{" << std::endl;
			}

		}
		else
		{
			*mOut << in(-1) << (curDefine()->isStatic() ? "static " : "virtual ") << 
											   curDefine()->createCodeDecl() << ";" << std::endl;
		}

		if (curDefine()->isStatic())
		{
			addIdentifier(std::make_shared<Identifier>(curIdentifiersHolder(), "me", curObject()));
		}
	}
	else
	{
		*mOut << in(-1) << curDefine()->createCodeDecl();
		if (aIsDecl)
		{
			*mOut << ";" << std::endl;
		}
		else
		{
			*mOut << "\n" << in(-1) << "{" << std::endl;
		}
	}

	curDefine()->createCodeCall();
}

void NateParser::endDefine()
{
	mCurDefine = nullptr;
	popDefineScope();
	mLastWriteStream.clear();
	if (!mDefineDecl)
	{
		*mOut << in() << "}\n" << std::endl;
	}
}

Define* NateParser::curDefine() { return mCurDefine; }

void NateParser::declareProperties(bool aReadonly,
																	 const std::vector<std::string>& aNames,
																	 const TypePtr& aType)
{
	for (auto const& name : aNames)
	{
		checkIdentifierName(name);
		IdentifierPtr id = std::make_shared<Identifier>(curIdentifiersHolder(), name, aType);
		id->setFlag(Identifier::Const, aReadonly);
		id->setFlag(Identifier::Property);
		addIdentifier(id);

		TypePtr idType = id->type();
		std::string propType = (idType->is(Type::NeedsRef))
											     ? idType->codeType() + "&"
			                     : idType->codeType();
		std::string declType = idType->codeType();
				
		*mOut << in(-1) << "private:" << std::endl;
		*mOut << in() << declType << " " << id->codeName() << ";" << std::endl;
		*mOut << in(-1) << "public:" << std::endl;

		*mOut << in() << codePropHeader(false, id, Object::PropType::Get) << ";" << std::endl;
		*mOut << in() << codePropHeader(false, id, Object::PropType::Set) << ";" << std::endl;

		if (idType->is(Type::Number))
		{
			//TYPE NAME ## _preInc(const TYPE value) { return NAME ## _set( NAME ## _get() + value); }
			//TYPE NAME ## _preDec(const TYPE value) { return NAME ## _set( NAME ## _get() - value); }
			//TYPE NAME ## _postInc(const TYPE value) { TYPE temp = NAME ## _get(); NAME ## _set(temp + value); return temp; }
			//TYPE NAME ## _postDec(const TYPE value) { TYPE temp = NAME ## _get(); NAME ## _set(temp - value); return temp; }
		}

		curObject()->setPropState(id, Object::PropType::Get, Object::PropState::Declared);
		curObject()->setPropState(id, Object::PropType::Set, Object::PropState::Declared);
	}
}

void NateParser::defineProp(const IdentifierPtr& aIdentifier, Object::PropType aPropType)
{
	const std::string method = aPropType == Object::PropType::Get ? "get" : "set";

	if (!curObject()->isPropDeclared(aIdentifier, aPropType))
	{
	  error("Undeclared " + method + " method for property: " + aIdentifier->name());
	}

	if (curObject()->isPropDefined(aIdentifier, aPropType))
	{
	  error("Redefined " + method + " method for property: " + aIdentifier->name());
	}

	curObject()->setPropState(aIdentifier, aPropType, Object::PropState::Defined);
	
	addDefine(true);
	mDefineDecl = false;
	mOut = &curObject()->getNormalOut();

	if (aPropType == Object::PropType::Get)
	{
		*mOut << in(-1) << codePropHeader(true, aIdentifier, Object::PropType::Get) << "\n" 
			    << in(-1) << "{" << std::endl;
	}
	else
	{
		*mOut << in(-1) << codePropHeader(true, aIdentifier, Object::PropType::Set) << "\n" 
			    << in(-1) << "{" << std::endl;

		IdentifierPtr value = std::make_shared<Identifier>(curIdentifiersHolder(), "value", aIdentifier->type());
		addIdentifier(value);
	}
}

void NateParser::endDefineProp(const IdentifierPtr& aIdentifier, Object::PropType aPropType)
{
	endDefine();
	deleteCurDefine();
}

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

void NateParser::addArgId(const std::string& aId, const TypePtr& aType, const std::string& inOut)
{ 
	auto id = std::make_shared<Identifier>(curIdentifiersHolder(), aId, aType);
	addIdentifier(id);
	curMethod().addArgId(id);
  if (!inOut.empty())
  {
    curMethod().curArg().setArgFlag(inOut);
  }

  if (id->isObjectMe())
  {
    if (!data.inObject)
    {
      error("The id 'me' is reserved for objects");
    }
    else if (data.objectMe)
    {
      error("The id 'me' may only occur once in a define");
    }

    data.objectMe = true;
  }
}

void NateParser::error(const std::string& anError)
{
	std::cerr << mLexer->fileLocation() << ": " << anError << std::endl;
	++mErrors;
}

void NateParser::optionalError(const std::string& anError)
{
	if (!anError.empty())
	{
		error(anError);
	}
}

void NateParser::warning(const std::string& aWarning)
{
	std::cerr << "Warning: " << mLexer->fileLocation() << ": " << aWarning << std::endl;
	++mWarnings;
}

void NateParser::printLineNr(const yy::parser::location_type& aLocation)
{
	static int prevLine = 0;
	static std::string prevFile;

	if (mLexer->has_matcher())
	{	 
		if (aLocation.begin.line != prevLine + 1 || 
				mLexer->filenames.back() != prevFile)
		{
			*mOut << "#line " << aLocation.begin.line;
			if (prevFile != mLexer->filenames.back())
			{
			  *mOut << " \"" << mLexer->filenames.back() << "\"";
			}
			
			*mOut << std::endl;

			prevLine = aLocation.begin.line;
			prevFile = mLexer->filenames.back();
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

IdentifierPtr NateParser::getIdentifier(const std::string& aName, IIdentifiersHolder* aIdentifiersHolder)
{
	if (aIdentifiersHolder == nullptr)
	{
		for (auto& identifiersHolder : mIdentifiersHolders)
		{
			auto var = identifiersHolder->identifiers().get(aName);
			if (var != nullptr)
			{
				return var;
			}
		}

		return IdentifierPtr();
	}
	else
	{
		return aIdentifiersHolder->identifiers().get(aName);
	}
}

IdentifierPtr NateParser::getOrFakeIdentifier(const std::string& aName, IIdentifiersHolder* aIdentifiersHolder)
{
	IdentifierPtr result = getIdentifier(aName, aIdentifiersHolder);
	if (!result)
	{
		error(std::string("Undeclared identifier: ") + aName);
		addIdentifier(std::make_shared<Identifier>(curIdentifiersHolder(), aName, getType("int-32")));
		result = getIdentifier(aName);
	}

	return result;
}

void NateParser::addIdentifier(const IdentifierPtr& aIdentifier)
{
	curIdentifiersHolder()->identifiers().add(aIdentifier);
}

ITypesHolderPtr NateParser::getTypesHolder(const TypePtr& aType) const
{
	for (auto& typesHolder : mTypesHolders)
	{
		if (typesHolder->types().contains(aType))
		{
			return typesHolder;
		}
	}
		
	return ITypesHolderPtr();
}

TypePtr NateParser::getType(const std::string& aName, ITypesHolder* aTypesHolder)
{
	if (aTypesHolder == nullptr)
	{
		for (auto& typesHolder : mTypesHolders)
		{
			auto type = typesHolder->types().get(aName);
			if (type)
			{
				return type;
			}
		}
		
		return TypePtr();
	}
	else
	{
		return aTypesHolder->types().get(aName);
	}
}

void NateParser::addType(TypePtr aType, const std::string& aName)
{
	aType->setTypeScopeName(typeScopeName());
	curTypesHolder()->types().add(aType, aName);
}

TypePtr NateParser::determineType(const std::string& aName)
{
	auto type = getType(aName);
	return !type ? std::make_shared<Type>(aName) : type;
}

bool NateParser::isType(const std::string& aName)
{
	return !!getType(aName);
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

			if (aMatch.matchedMethod == nullptr || !matchResult.error.empty())
			{
				aMatch.matchedMethod = &aMethod;
				aMatch.matchResult = matchResult;
			}
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
	if (size <= aExpr.nodes().size() && size > 0)
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
		for (auto& object : mObjects)
		{		
			for (auto const& define : object->defines().get())
			{
				checkIfMethod(define, aExpr, match, aDebug);
			}
		}

		for (auto const& code : mCodes)
		{
			checkIfMethod(code, aExpr, match, aDebug);
		}
		
		for (auto& scope : mScopes)
		{
			for (auto const& define : scope->defines().get())
			{
				checkIfMethod(define, aExpr, match, aDebug);
			}
		}

		if (match.methodFound)
		{
			Method::EvaluateResult evalResult = 
				match.methodFound->evaluate(curDefine(), match.nodeStartIter, match.nodeEndIter, aDebug);
			if (!evalResult.error.empty())
			{
				error(evalResult.error);
			}
			else if (evalResult.code == "NI")
			{
				error("Not implemented: " + aExpr.text());
			}

			if (aDebug) std::cerr << "******* " << (evalResult.type ? *evalResult.type : Type()) << " " << evalResult.code << std::endl;
			
			ExprNode node(evalResult.origText + " ", evalResult.code, evalResult.type);
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
	else if (aCommand == "TYPE")
	{
		while (result.size() > 1 && result[0] == '(')
		{
			result = result.substr(1, result.size() - 2);
		}

		result = toCodeName(getOrFakeIdentifier(result)->type()->name());
	}
	else if (aCommand == "OBJECT_TYPE")
	{
		result = toCodeName(curObject()->name());
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

void NateParser::codeStartProgram(const yy::parser::location_type& aLocation)
{
	*mOut << in() << "#define NOMINMAX" << std::endl;
	*mOut << in() << "#include <windows.h>" << std::endl;

	printLineNr(aLocation);

	*mOut << in() << "int main(int argc, char** argv)\n" << in() << "{" << std::endl;
	pushScope("main");
	*mOut << in() << "output = std::shared_ptr<std::ostream>(&std::cout, [](void*) {});" << std::endl;
	*mOut << in() << "error = std::shared_ptr<std::ostream>(&std::cerr, [](void*) {});" << std::endl;
	*mOut << in() << "SetConsoleOutputCP(65001);" << std::endl;
	//*mOut << "std::locale::global(std::locale(\"en_US.UTF8\"));" << std::endl;
    
}

void NateParser::codeEndProgram(const yy::parser::location_type& aLocation)
{
	popScope();
	printLineNr(aLocation);
	*mOut << in() << "}\n" << std::endl;
}

void NateParser::codeStartScope()
{
	*mOut << in() << "{" << std::endl;
	pushScope("scope");
}

void NateParser::codeEndScope()
{
	popScope();
	*mOut << in() << "}\n" << std::endl;
}

void NateParser::codeCodeInclude()
{
	*mOut << in() << mCodes.back().code() << std::endl;
	mCodes.pop_back();
}

void NateParser::codeDeclareLocalIdentifier(bool aExtern,
																						const IdentifierPtr& aIdentifier,
																						bool initializeVariables,
																						const yy::parser::location_type& aLocation)
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
	
	//if (aIdentifier->is(Identifier::Const) && !aIdentifier->initValue().is(ExprNode::ConstExpr))
	//{
	//	error("Expected constant expression.");
	//}

	printLineNr(aLocation);
	if (aExtern)
	{
	  *mOut << in() << "extern ";	
	}

	if (aIdentifier->is(Identifier::Const))
	{
		*mOut << in() << "const ";		
	}

	*mOut << in() << aIdentifier->type()->codeType() << " " << aIdentifier->codeName();

	if (initializeVariables)
	{
		*mOut << " = " << aIdentifier->initValue().code();
	}


	*mOut << ";" << std::endl;
}

void NateParser::codeDeclareLocalIdentifiers(bool aIsConst,
																						 const std::vector<std::string>& aNames,
																						 const TypePtr& aType,
																					 	 const std::vector<Expr>& aInitValues,
																						 bool initializeVariables,
																						 const yy::parser::location_type& aLocation)
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
		checkIdentifierName(name);

		Expr initValue;
		if (aInitValues.empty())
		{
			initValue = Expr(ExprNode("default", "{}", type));
			initValue.node().setFlag(ExprNode::Default);
			if (aIsConst)
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
			initValue.node().setFlag(ExprNode::ConstExpr, aIsConst);
		}

		IdentifierPtr id = std::make_shared<Identifier>(curIdentifiersHolder(), name, type, initValue);
		id->setFlag(Identifier::Const, aIsConst);

		codeDeclareLocalIdentifier(false, id, initializeVariables, aLocation);
	}
}

void NateParser::checkIdentifierName(const std::string& aName)
{
	if (getIdentifier(aName, curIdentifiersHolder().get()))
	{
		error("duplicate declaration of: " + aName);
	}
	else if (getIdentifier(aName))
	{
		if (isReservedName(aName))
		{
			error("reserved name: " + aName);
		}
		else
		{
			warning("hides declaration of: " + aName);
		}
	}
}
 
void NateParser::codeStartRecord(const RecordPtr& aRecord,
																 const yy::parser::location_type& aLocation)
{
	printLineNr(aLocation);
	*mOut << in() << "struct " << aRecord->codeType() << "\n" << in() << "{" << std::endl;
  data.curRecord.push(aRecord);
	addType(aRecord, aRecord->name());
	pushIdentifiersHolder(aRecord);
}

void NateParser::codeDeclareRecordIdentifiers(bool aIsConst,
																							const std::vector<std::string>& aNames,
																							const TypePtr& aType,
																							const std::vector<Expr>& aInitValues,
																							const yy::parser::location_type& aLocation)
{
	codeDeclareLocalIdentifiers(aIsConst, aNames, aType, aInitValues, !NateParser::InitializeVariables, aLocation);
}

void NateParser::codeEndRecord(const yy::parser::location_type& aLocation)
{
	RecordPtr record = data.curRecord.top();

	*mOut << in() << record->codeType() << "()" << std::endl;
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

		if (!id->initValue().is(ExprNode::Default))
		{
			*mOut << id->initValue().code();
		}
		*mOut << ")" << std::endl;
	}

	*mOut << in(1) << "{}" << std::endl;
	popIdentifiersHolder();
	printLineNr(aLocation);
	*mOut << in() << "};\n" << std::endl;
}

void NateParser::codeStartDeclObject()
{	
	auto name = toCodeName(curObject()->name());
	*mOut << "class " << name;
	if (curObject()->getBase())
	{
		*mOut << ": public " << toCodeName(curObject()->getBase()->name());
	}
	*mOut << std::endl;
	*mOut << "{" << std::endl;
	*mOut << "private:" << std::endl;
	*mOut << "  class __impl;"  << std::endl;
	*mOut << "  __impl* _impl;"  << std::endl;
	*mOut << "  friend class __impl;"  << std::endl;
	*mOut << "public:" << std::endl;
	*mOut << "  " << name << "();" << std::endl;
	*mOut << "  virtual ~" << name << "();" << std::endl;
}

void NateParser::codeEndDeclObject()
{
	*mOut << "};\n" << std::endl;
}

void NateParser::codeStartImplObject()
{	
	auto name = toCodeName(curObject()->name());
	mSavedOut = mOut;

	if (curObject()->is(Type::ObjectImpl))
	{
		mOut = &curObject()->getNormalOut();
		*mOut << "class " << name;
		if (curObject()->getBase())
		{
			*mOut << ": public " << toCodeName(curObject()->getBase()->name());
		}
		*mOut << "{" << std::endl;
		*mOut << "public:" << std::endl;
	}
	else
	{
		mOut = &curObject()->getImplOut();
		*mOut << "class " << name << "::__impl" << std::endl;
		*mOut << "{" << std::endl;
		*mOut << "private:" << std::endl;
		*mOut << "  " << name << "* me;" << std::endl;
		*mOut << "public:" << std::endl;
		*mOut << "  __impl(" << name << "* aMe) : me(aMe) {}" << std::endl;

		mOut = &curObject()->getNormalOut();
		*mOut << name << "::" << name << "()" << std::endl;
		*mOut << "  : _impl(new __impl(this)) {}\n" << std::endl;
		*mOut << name << "::~" << name << "() { delete _impl; }\n" << std::endl;
	}
}

std::string NateParser::codePropHeader(bool aAddObjectName,
																			 const IdentifierPtr& aId, 
																			 Object::PropType aPropType)
{
	std::stringstream buf;
	
	auto scopeName = aId->type()->typeScopeName();
	std::string propType = (aId->type()->is(Type::NeedsRef))
											    ? scopeName + aId->type()->codeType() + "&"
			                    : aId->type()->codeType();
	std::string objectPrefix = aAddObjectName
													   ? toCodeName(curObject()->name()) + "::"
														 : "";
	if (aPropType == Object::PropType::Get)
	{
		buf << "const " <<  propType << " " << objectPrefix << aId->codeName() << "_get() const";
	}
	else
	{
		buf << "const " <<  propType << " " << objectPrefix << aId->codeName() << "_set(const " << propType << " value)";
	}

	return buf.str();
}

void NateParser::codeEndImplObject()
{
	mOut = &curObject()->getNormalOut();

	for (const auto& propMethod : curObject()->propertyMethods())
	{
		const auto& id = propMethod.first;

		if (curObject()->getPropState(id, Object::PropType::Get) == Object::PropState::Declared)
		{
			*mOut << in() << codePropHeader(true, id, Object::PropType::Get) 
				    << " { return " << id->codeName() << "; }" << std::endl;
		}

		if (curObject()->getPropState(id, Object::PropType::Set) == Object::PropState::Declared)
		{
			*mOut << in() << codePropHeader(true, id, Object::PropType::Set) 
				    << " { return " << id->codeName() << " = value; }" << std::endl;
		}
	}

	mOut = mSavedOut;

	if (curObject()->is(Type::ObjectImpl))
	{
		for (const auto& define : curObject()->defines().get())
		{
			if (!define.is(Method::Undeclared) && !define.is(Method::Defined))
			{
				error("Undefined method: " + define.signature());
			}
		}
		
		*mOut << curObject()->getNormalOut().str();
		*mOut << "};\n" << std::endl;
	}
	else
	{
		*mOut << curObject()->getImplOut().str();
		*mOut << "};\n" << std::endl;
		*mOut << curObject()->getNormalOut().str();
	}
}

std::string NateParser::typeScopeName() const
{
	std::string result;

	for (auto const& holder : mTypesHolders)
	{
		auto name = holder->typeScopeName();
		if (!name.empty())
		{
			result += toCodeName(name);
			result += "::";
		}
	}

	return result;
}

void NateParser::codeAssign(const std::vector<Expr>& aExpressions,
														Expr& aValue,
														const yy::parser::location_type& aLocation)
{
	std::string endPars;

	printLineNr(aLocation);
	for (auto const& expr : aExpressions)
	{
		if (expr.is(ExprNode::ConstExpr))
		{
			error("Cannot assign to a constant or readonly");
		}
		else if (!expr.is(ExprNode::Output))
		{
			error("Cannot assign to a non-variable");
		}

		const TypePtr& exprType = expr.type();
		bool ok = aValue.node().castToType(exprType);
		if (!ok)
		{
			error("cannot cast '" + aValue.text() + "' of type " + aValue.type()->name() + " to type " + exprType->name());
		}
				
		std::string code = expr.code();
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

	*mOut << aValue.code() << endPars << ";" << std::endl;
}

std::string NateParser::codeId(const std::string& aName, Scope* aScope)
{
	return getOrFakeIdentifier(aName, aScope)->codeName();
}

void NateParser::codeWriteStart(const Expr& aValue, const yy::parser::location_type& aLocation)
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

	printLineNr(aLocation);
	mFirstOutput = true;
	mCachedOutput.clear();
}

void NateParser::codeOutputStart(const std::string& aStream, const yy::parser::location_type& aLocation)
{
	mStream = aStream;
	printLineNr(aLocation);
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

void NateParser::codeInputStart(const std::string& aStream, const yy::parser::location_type& aLocation)
{
	mStream = aStream;
	printLineNr(aLocation);
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

void NateParser::NateParser::codeIf(const Expr& aValue, const yy::parser::location_type& aLocation)
{
	printLineNr(aLocation);
	if (!aValue.type()->is(Type::Boolean))
	{
		error("Expected boolean expression for IF statement");
	}

	*mOut << in() << "if (" << aValue.code() << ")\n" << in() << "{" << std::endl;
	pushScope("if");
}

void NateParser::codeElseIf()
{
	*mOut << in() << "else " << std::endl;
}

void NateParser::codeElse(const yy::parser::location_type& aLocation)
{
	printLineNr(aLocation);
	*mOut << in() << "else\n" << in() << "{" << std::endl;
	pushScope("else");
}

void NateParser::codeEndIf()
{
	popScope();
	*mOut << in() << "}" << std::endl;
}

void NateParser::codeIfIs(const Expr& aValue, const std::string& idName, const yy::parser::location_type& aLocation)
{
	printLineNr(aLocation);
	IfIs info;
	info.idName = idName;
	info.isSwitch = aValue.type()->is(Type::Scalar);
	info.out = std::make_shared<std::ostringstream>();
	info.savedOut = mOut;
	mIfIs.push(info);
	
	*mOut << in() << "auto const " << idName << " = " << aValue.code() << ";" << std::endl;
}

void NateParser::codeIs(const Expr& aValue, const Expr& aIfExpr, const yy::parser::location_type& aLocation)
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
		printLineNr(aLocation);
		*mOut << in() << "case " << aValue.code() << ":" << std::endl;
	}
	else
	{
		bool firstExpr = ifIs.isFirstTest;
		
		if (!ifIs.isFirst && firstExpr)
		{
			printLineNr(aLocation);
			*mOut << in() << "else ";
		}

		if (firstExpr)
		{
			if (ifIs.isFirst)
			{
				printLineNr(aLocation);
			}

			*mOut << in() << "if (";
		}
		else
		{
			*mOut << std::endl;			
			printLineNr(aLocation);
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

void NateParser::codeElseIs(const yy::parser::location_type& aLocation)
{
	printLineNr(aLocation);
	auto ifIs = mIfIs.top();
	if (!ifIs.isFirst)
	{
		*mOut << in() << "else" << std::endl;
	}

	ifIs.nextCase = ifIs.isSwitch && !ifIs.out->str().empty();
	if (ifIs.nextCase)
	{
		*mOut << in() << "switch (" << ifIs.idName << ")\n" << in() << "{" << std::endl;
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

void NateParser::codeEndIs(const yy::parser::location_type& aLocation)
{
	auto ifIs = mIfIs.top();
	printLineNr(aLocation);

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

void NateParser::codeEndIfIs(const yy::parser::location_type& aLocation)
{
	auto ifIs = mIfIs.top();
	printLineNr(aLocation);
	if (mIfIs.top().isSwitch && !ifIs.out->str().empty())
	{
		*mOut << in() << "}" << std::endl;
	}

	mIfIs.pop();
}

void NateParser::codeInitLoop(const yy::parser::location_type& aLocation)
{
	printLineNr(aLocation);
	mLoopWhileCounts.push_back(0);
	pushScope("while");
}

void NateParser::codeStartLoop()
{
	*mOut << in(-1) << "while (true)\n" << in(-1) << "{" << std::endl;
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

	IdentifierPtr id = std::make_shared<Identifier>(curIdentifiersHolder(), aId, type);
	addIdentifier(id);

	*mOut << in(-1) << "for (" << id->type()->codeType() << " " 
			 << id->codeName() << "= " << aStart.code() << ";" 
			 << id->name() << (aDownTo ? " >= " : "<=") << aEnd.code() << ";"
			 << id->name() << (aDownTo ? " -= " : "+=") << aStep.code() << ")\n" << in(-1) << "{" << std::endl;
}

void NateParser::codeStartForRangeLoop(const std::string& aId, 
																			 const Expr& aRange)
{
	TypePtr rangeType = aRange.type();
	if (rangeType->isOfType("container"))
	{
		TypePtr type = rangeType->typenameType();
		IdentifierPtr id = std::make_shared<Identifier>(curIdentifiersHolder(), aId, type);
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
			   << increment << ")\n" << in(-1) << "{" << std::endl;
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

void NateParser::codeLoopWhile(const Expr& aExpr, const yy::parser::location_type& aLocation)
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
		
	printLineNr(aLocation);
	*mOut << in() << "if (!(" << aExpr.code() << ")) break;" << std::endl;
}

 void NateParser::codeReturn(const Expr& aValue, const yy::parser::location_type& aLocation)
 {
	 printLineNr(aLocation);
	 *mOut << in() << "return " << aValue.code() << ";" << std::endl;
 }

 void NateParser::codeExpression(const Expr& aValue, const yy::parser::location_type& aLocation)
 {
	 printLineNr(aLocation);
	 *mOut << in() << aValue.code() << ";" << std::endl;
 }