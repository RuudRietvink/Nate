#include "NateParser.h"
#include "NateParserMath.h"
#include "NateCode.h"
#include "core/Core.h"
#include "StatProgram.h"
#include "StatDeclareLocal.h"
#include "StatAssign.h"
#include "lex.yy.h"
#include <algorithm>
#include <inttypes.h>
#include <cctype>
#include <tuple>

int gDebug = 0;

NateParser::NateParser(const std::string& aFilename, std::istream& aIn, std::ostream& aOut,
											 FileType aFileType)
: mLexer(new yy::Lexer(aIn)),
	mParser(new yy::parser(*mLexer, *this)),
	mOut(&aOut),
	mFileType(aFileType),
	mFileName(aFilename),
	mLibrary("C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\core"),
	mMathParser(new NateParserMath(*this))
{
	mLexer->nate = this;
	mLexer->pushFile(aFilename);
	  
	initOutput();
	initTypesAndObjects();
}

NateParser::~NateParser() = default;

Location NateParser::location(const yy::parser::location_type& aLocation)
{
	return Location(aLocation, mLexer->currentFile());
}

Stat::SPtr NateParser::addStatement(const Stat::SPtr& stat)
{
	if (mStatHolders.empty())
	{
	  mStats.push_back(stat);
	}
	else
	{
	  mStatHolders.back()->addStat(stat);
	}

	return stat;
}

yy::Lexer* NateParser::getLexer()
{
	return mLexer.get();
}

void NateParser::initTypesAndObjects()
{
	pushScope(std::make_shared<Scope>("global", IIdentifiersHolder::ScopeFlag::Global));
	
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
		
	addType(std::make_shared<Type>("rational", getType("number")));
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

	addType(std::make_shared<Type>("Output", getType("object")));
	addType(std::make_shared<Type>("File-Output", getType("Output")));
	addType(std::make_shared<Type>("Input", getType("object")));
	addType(std::make_shared<Type>("Data-Input", getType("Input")));
}

void NateParser::initOutput()
{
	if (mFileType == FileType::ObjectDecl)
	{
		*mOut << "#pragma once" << std::endl;
	}

	*mOut << "#include \"C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\core\\Core.h\"" << std::endl;
}

int NateParser::parseAndCode()
{
	int result = parse();
	if (result == 0)
	{
		result = code();
	}

	return result;
}

int NateParser::parse()
{
	if (mLexer->debug())
	{
		std::cerr << "Seperate parsing: " << mFileName << std::endl;
	}

	if (mFileType == FileType::Normal)
	{
		declareLocalIdentifiers(false, { "output" }, getType("Output"), {}, false, mDummyLocation);
		declareLocalIdentifiers(false, { "error" }, getType("Output"), {}, false, mDummyLocation);
		declareLocalIdentifiers(false, { "input" }, getType("Input"), {}, false, mDummyLocation);
	}
	else
	{	
		addIdentifier(std::make_shared<Identifier>(curIdentifiersHolder(), "output", getType("Output")));
		addIdentifier(std::make_shared<Identifier>(curIdentifiersHolder(), "error", getType("Output")));
		addIdentifier(std::make_shared<Identifier>(curIdentifiersHolder(), "input", getType("Input")));
	}

	if (mFileType != FileType::ObjectDecl)
	{
		for (auto file : { "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\core\\core.ns" })
		{
			parseFile(file);
		}
	}
		
	return mParser->parse();
}

int NateParser::code()
{
	int result = 0;

	NateCode coder(*mOut, this);
	coder.codeStats(mStats);

	if (mLexer->debug())
	{
		std::cerr << "End seperate parsing: " << mFileName << std::endl;
		result = 1;
	}

	return result;
}

void NateParser::parseFile(const std::string& aFilename)
{
	std::ifstream stream(aFilename);
	yy::Lexer lexer(stream);
	lexer.nate = this;
	lexer.pushFile(aFilename);
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

void NateParser::startProgram(const yy::parser::location_type& aLocation)
{
	pushScope(std::make_shared<Scope>("main", IIdentifiersHolder::ScopeFlag::Local));
	pushStatsHolder(addStatement(std::make_shared<StatProgram>(location(aLocation))));
}

void NateParser::endProgram(const yy::parser::location_type& aLocation)
{
	popStatsHolder();
	popScope();
}

void NateParser::doAssign(const std::vector<Expr>& aExpressions,
													const Expr& aValue,
													const yy::parser::location_type& aLocation)
{
	Expr copy(aValue);
	for (auto const& expr : aExpressions)
	{
		if (expr.is(Expr::ConstExpr))
		{
			error("Cannot assign to a constant or readonly");
		}
		else if (!expr.is(Expr::Output))
		{
			std::cerr << expr << std::endl;
			error("Cannot assign to a non-variable");
		}
		else if (expr.is(Identifier::Property) && expr.nodes().size() == 1)
		{
			std::ostringstream errorStr;
			if (!checkProperty(errorStr, expr.nodes().begin()))
			{
				error(errorStr.str());
			}
		}

		const TypePtr& exprType = expr.type();
		bool ok = copy.castToType(exprType);
		if (!ok)
		{
			error("cannot cast '" + copy.text() + "' of type " + copy.type()->name() + " to type " + exprType->name());
		}
	}
	
  addStatement(std::make_shared<StatAssign>(Location(aLocation, mLexer->currentFile()), aExpressions, aValue));
}

void NateParser::doIf(const Expr& aValue, const yy::parser::location_type& aLocation)
{
	if (!aValue.type()->is(Type::Boolean))
	{
		error("Expected boolean expression for IF statement");
	}

	pushScope(std::make_shared<Scope>("if", IIdentifiersHolder::ScopeFlag::Local));
	pushStatsHolder(addStatement(std::make_shared<StatIfThen>(location(aLocation), aValue)));
}

void NateParser::doElseIf(const Expr& aValue, const yy::parser::location_type& aLocation)
{
	popScope();
	popStatsHolder();
	pushScope(std::make_shared<Scope>("elseif", IIdentifiersHolder::ScopeFlag::Local));
	pushStatsHolder(addStatement(std::make_shared<StatIfThen::ElseIf>(location(aLocation), aValue)));
}

void NateParser::doElse(const yy::parser::location_type& aLocation)
{
	popScope();
	pushScope(std::make_shared<Scope>("else", IIdentifiersHolder::ScopeFlag::Local));
	popStatsHolder();
  pushStatsHolder(addStatement(std::make_shared<StatIfThen::Else>(location(aLocation))));
}

void NateParser::doEndIf(const yy::parser::location_type& aLocation)
{
	popScope();
	popStatsHolder();
}

void NateParser::doIfIs(const Expr& aValue, const yy::parser::location_type& aLocation)
{
	IdentifierPtr id = std::make_shared<Identifier>(curIdentifiersHolder(), uniqueName(), aValue.type());
	addIdentifier(id);
	IfIs info;
	info.id = id;
	info.isSwitch = aValue.type()->is(Type::Scalar);
	mIfIs.push(info);
		
  pushStatsHolder(addStatement(std::make_shared<StatIfIs>(location(aLocation), id, aValue)));
}

void NateParser::doCaseIsList(const yy::parser::location_type& aLocation)
{
	pushStatsHolder(addStatement(std::make_shared<StatIfIs::IsList>(location(aLocation))));
}

void NateParser::doCaseIs(const Expr& aValue, const Expr& aIfExpr, const yy::parser::location_type& aLocation)
{
	if (!(aValue.type()->isOfType(aIfExpr.type()->name()) ||
				aValue.type()->is(Type::Real) == aIfExpr.type()->is(Type::Real)))
	{
		error("Expected expression with same type as in IF");
	}
		
	addStatement(std::make_shared<StatIfIs::Is>(location(aLocation), aValue));
}

void NateParser::doElseIs(const yy::parser::location_type& aLocation)
{
  pushStatsHolder(addStatement(std::make_shared<StatIfIs::Else>(location(aLocation))));
}

void NateParser::doEndIs(const yy::parser::location_type& aLocation)
{
	mIfIs.pop();
	popStatsHolder();
}

void NateParser::doInitLoop(const yy::parser::location_type& aLocation)
{
	mLoopWhileCounts.push_back(0);
	pushScope(std::make_shared<Scope>("while", IIdentifiersHolder::ScopeFlag::Local));
}

void NateParser::doStartLoop(const yy::parser::location_type& aLocation)
{
	pushStatsHolder(addStatement(std::make_shared<StatLoop>(location(aLocation))));
}

void NateParser::doWhile(const Expr& aValue, const yy::parser::location_type& aLocation)
{
	if (mLoopWhileCounts.back() > 0)
	{
		error("Only one while allowed in a loop.");
	}

	++mLoopWhileCounts.back();

	if (!aValue.type()->is(Type::Boolean))
	{
		error("Expected boolean condition in while.");
	}
	
	addStatement(std::make_shared<StatLoop::While>(location(aLocation), aValue));
}

void NateParser::doStartLoopForStep(const std::string& aId, 
																		const TypePtr& aType, 
																		bool aDownTo,
																		const Expr& aStart,
																		const Expr& aEnd,
																		const Expr& aStep, 
																		const yy::parser::location_type& aLocation)
{
	TypePtr type = aType->empty()
							   ? aStart.type()
							   : aType;

	IdentifierPtr id = std::make_shared<Identifier>(curIdentifiersHolder(), aId, type);
	addIdentifier(id);
	
	pushStatsHolder(addStatement(std::make_shared<StatLoop::ForStep>(location(aLocation), id, aDownTo, aStart, aEnd, aStep)));
}

void NateParser::doStartLoopForRange(const std::string& aId, 
																	   const Expr& aRange, 
											  					 	 const yy::parser::location_type& aLocation)
{
	TypePtr rangeType = aRange.type();
	if (rangeType->isOfType("container"))
	{
		TypePtr type = rangeType->typenameType();
		IdentifierPtr id = std::make_shared<Identifier>(curIdentifiersHolder(), aId, type);
		addIdentifier(id);
	  pushStatsHolder(addStatement(std::make_shared<StatLoop::ForRange>(location(aLocation), id, aRange)));
	}
	else
	{
		error("Range must be a container, got: " + rangeType->name());
	}
}

void NateParser::doEndLoop(const yy::parser::location_type& aLocation)
{
	mLoopWhileCounts.pop_back();
	popScope();
	popStatsHolder();
}

void NateParser::doStartScope(const yy::parser::location_type& aLocation)
{
	pushStatsHolder(addStatement(std::make_shared<StatScope>(location(aLocation))));
	pushScope(std::make_shared<Scope>("scope", IIdentifiersHolder::ScopeFlag::Local));
}

void NateParser::doEndScope(const yy::parser::location_type& aLocation)
{
	popScope();
	popStatsHolder();
}

void NateParser::doCodeInclude(const yy::parser::location_type& aLocation)
{
	addStatement(std::make_shared<StatCode>(location(aLocation), mCodes.back()->code()));
	mCodes.pop_back();
}

std::string NateParser::in(int aOffset) const
{
	int size = static_cast<int>(mTypesHolders.size()) + aOffset - 1; 
	//if (mCurObject && mOut == &mCurObject->getImplOut())
	{
		++size;
	}
	return std::string(size, '\t');
}

void NateParser::doData(const std::string& aId, const yy::parser::location_type& aLocation)
{
	IdentifierPtr id = std::make_shared<Identifier>(curIdentifiersHolder(), aId, determineType("text"));
	id->setFlag(Identifier::Const);
	addIdentifier(id);
	pushStatsHolder(addStatement(std::make_shared<StatData>(location(aLocation), id)));
}

void NateParser::doDataEnd()
{
	popStatsHolder();
}

void NateParser::doWrite(const Expr& aValue, const yy::parser::location_type& aLocation)
{
	IdentifierPtr writer = getIdentifier("nate__writer");

	if (aValue.isEmpty())
	{
		if (writer)
		{
	    pushStatsHolder(addStatement(std::make_shared<StatWrite>(location(aLocation), writer, aValue, false)));
		}
		else
		{
			error("Need to specify where to write to");
		}
	}
	else if (aValue.type()->isOfType("Output"))
	{
		if (!writer)
		{
			writer = std::make_shared<Identifier>(curIdentifiersHolder(), "nate__writer", determineType("Output"), aValue);
			addIdentifier(writer);
	    pushStatsHolder(addStatement(std::make_shared<StatWrite>(location(aLocation), writer, aValue, true)));
		}
		else
		{
	    pushStatsHolder(addStatement(std::make_shared<StatWrite>(location(aLocation), writer, aValue, false)));
		}
	}
	else
	{
		error("Cannot write to type: " + aValue.type()->name());
	}
}

void NateParser::doOutputStart(const yy::parser::location_type& aLocation)
{
	pushStatsHolder(addStatement(std::make_shared<StatOutput>(location(aLocation))));
}

void NateParser::doOutputComma(const yy::parser::location_type& aLocation)
{
	addStatement(std::make_shared<StatOutput::Comma>(location(aLocation)));
}

void NateParser::doOutputConcat(const yy::parser::location_type& aLocation)
{
	addStatement(std::make_shared<StatOutput::Concat>(location(aLocation)));
}

void NateParser::doOutputEnd(bool aEnd, const yy::parser::location_type& aLocation, bool aPopStatsHolder)
{
	addStatement(std::make_shared<StatOutput::End>(location(aLocation), aEnd));
	if (aPopStatsHolder)
	{
		popStatsHolder();
	}
}

void NateParser::doOutputExpr(const Expr& aValue, const yy::parser::location_type& aLocation)
{
  if (aValue.type() && !aValue.type()->empty())
  {
	  addStatement(std::make_shared<StatOutput::Value>(location(aLocation), aValue));
  }
  else
  {
    error("Cannot have typeless expression in output");
  }
}

void NateParser::doRead(InputType aInputType, const Expr& aValue, const yy::parser::location_type& aLocation)
{
	IdentifierPtr reader = getIdentifier("nate__reader");

	if (aValue.isEmpty())
	{
		if (reader)
		{
	    pushStatsHolder(addStatement(std::make_shared<StatRead>(location(aLocation), aInputType, reader, aValue, false)));
		}
		else
		{
			error("Need to specify where to read from");
		}
	}
	else if (aValue.type()->isOfType("Input"))
	{
		if (!reader)
		{
			reader = std::make_shared<Identifier>(curIdentifiersHolder(), "nate__reader", determineType("Input"), aValue);
			addIdentifier(reader);
	    pushStatsHolder(addStatement(std::make_shared<StatRead>(location(aLocation), aInputType, reader, aValue, true)));
		}
		else
		{
	    pushStatsHolder(addStatement(std::make_shared<StatRead>(location(aLocation), aInputType, reader, aValue, false)));
		}
	}
	else
	{
		error("Cannot read from type: " + aValue.type()->name());
	}
}

void NateParser::doInput(const yy::parser::location_type& aLocation)
{
	pushStatsHolder(addStatement(std::make_shared<StatInput>(location(aLocation))));
}

void NateParser::doInputComma(const yy::parser::location_type& aLocation)
{
	addStatement(std::make_shared<StatInput::Comma>(location(aLocation)));
}

void NateParser::doInputConcat(const yy::parser::location_type& aLocation)
{
	addStatement(std::make_shared<StatInput::Concat>(location(aLocation)));
}

void NateParser::doInputEnd(bool aEnd, const yy::parser::location_type& aLocation)
{
	addStatement(std::make_shared<StatInput::End>(location(aLocation), aEnd));
	popStatsHolder();
}

void NateParser::doError(const yy::parser::location_type& aLocation)
{
	pushStatsHolder(addStatement(std::make_shared<StatError>(location(aLocation))));
}

void NateParser::doInputExpr(const Expr& aValue, const yy::parser::location_type& aLocation)
{
	if (aValue.is(Expr::Output) && !aValue.is(Expr::ConstExpr))
	{
	  addStatement(std::make_shared<StatInput::Value>(location(aLocation), aValue));
	}
	else
	{
		error("Expected non-constant variable for input");
	}
}

void NateParser::doStartRecord(const std::string& anId, const yy::parser::location_type& aLocation)
{
  if (curTypesHolder()->types().get(anId))
  {
		error("Duplicate type of :" + anId);
  }

  RecordPtr record = std::make_shared<Record>(anId);
  curRecordsHolder()->records().add(record, anId);
  data.curRecord.push(record);
	addType(record, record->name());
	pushIdentifiersHolder(record);
	pushStatsHolder(addStatement(std::make_shared<StatRecord>(location(aLocation), record)));
}

void NateParser::doEndRecord(const yy::parser::location_type& aLocation)
{
	popIdentifiersHolder();
	popStatsHolder();
}

void NateParser::doReturn(const Expr& aValue, const yy::parser::location_type& aLocation)
{
	if (!curDefine())
	{
		error("Return outside define");
	}
	else if (!curDefine()->type() || curDefine()->type()->canBeCastedFrom(aValue.type()) == Type::CompareResult::No )
	{
		error("Incompatible return type");
	}

	addStatement(std::make_shared<StatReturn>(location(aLocation), aValue));
}

void NateParser::doDeclObject(const yy::parser::location_type& aLocation)
{	
	addObject(data.object);
  checkObject(curObject());
	pushStatsHolder(addStatement(std::make_shared<StatObject>(location(aLocation), curObject(), true)));
}

void NateParser::doEndDeclObject(const yy::parser::location_type& aLocation)
{
	if (!curObject()->isRole())
	{
		addUndeclaredProperties(curObject(), aLocation);
		addUndeclaredDefines(curObject(), aLocation);
	}

  data.inObject = false;
	endObject();
	popStatsHolder();
}

void NateParser::doImplObject(const yy::parser::location_type& aLocation)
{
  auto objectDecl = getObject(data.object->name());
  bool existingObjectDecl = objectDecl && !objectDecl->is(Type::ObjectImpl);

  if (!existingObjectDecl)
  {
		addObject(data.object);
    curObject()->setCodeType(toCodeName(data.object->name()));
    curObject()->setFlag(Type::Abstract, false);
    curObject()->setFlag(Type::Unknown, false);
    curObject()->setFlag(Type::ObjectImpl);
          
    checkObject(curObject());
		pushStatsHolder(addStatement(std::make_shared<StatObject>(location(aLocation), curObject(), false)));
  }
  else
  {
		if (!data.object->getBases().empty())
		{
			error("Redefinition of base objects/roles");
		}
		
    startObject(objectDecl);
		pushStatsHolder(addStatement(std::make_shared<StatObject>(location(aLocation), objectDecl, false)));
  }
}


void NateParser::doEndImplObject()
{
	endImplementObject();
	popStatsHolder();
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

void NateParser::importBaseObject(const std::string& aBaseName)
{
	if (!getObject(aBaseName))
	{
		import(aBaseName);
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
			
			auto parseResult = nate.parseAndCode();

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
			
			for (auto const& code : nate.mCodes)
			{
				if (!getCode(code))
				{
					mCodes.push_back(code);
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
		std::string nsPath = library + Core::directorySeperator() + aName + ".ns";
		std::string ndPath = library + Core::directorySeperator() + aName + ".nd";
		if (Core::exists(nsPath) && !Core::exists(ndPath))
		{
			mLexer->includeFile(nsPath);
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

void NateParser::pushStatsHolder(const Stat::SPtr& aStatsHolder)
{
	mStatHolders.push_back(aStatsHolder);
}

void NateParser::popStatsHolder()
{
	mStatHolders.pop_back();
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
	mOldScopes.push_back(mScopes.front());	// save for later in evaluations
	mScopes.pop_front();
	popIdentifiersHolder();
	popRecordsHolder();
	popTypesHolder();
	if (mLexer->debug()) std::cerr << "pop to " << mScopes.front()->name() << std::endl;
}

MethodPtr NateParser::curMethod()
{
	return mMethodType == MethodType::Code 
			   ? static_cast<MethodPtr>(curCode()) 
		     : static_cast<MethodPtr>(curDefine());
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

void NateParser::checkObject(const ObjectPtr& aObject)
{
	size_t baseCount = std::count_if(aObject->getBases().begin(), aObject->getBases().end(),
													   	     [&](const ObjectPtr& aBase)
	                                 { return !aBase->isRole(); });
	if (baseCount > 1)
	{
		error("Multiple inheritance is not allowed");
	}
	else if (baseCount == 0 && !aObject->isRole())
	{
    if (aObject->name() != baseObjectName())
    {
      importBaseObject(baseObjectName());
      aObject->addBase(getObject(baseObjectName()));
    }
	}
}

void NateParser::addUndeclaredProperties(const ObjectPtr& aObject, const yy::parser::location_type& aLocation)
{
	for (const auto& base : aObject->getBases())
	{
		if (base->isRole())
		{
			for (auto& propMethod : base->propertyMethods())
			{
				auto const& propId = propMethod.first;
				if (curObject()->getPropState(propId, Property::PropType::Get).state == Object::PropState::State::Unknown)
				{
					IdentifierPtr id = getIdentifier(propId->name(), curIdentifiersHolder().get());
					if (!id || !id->isProperty())
					{
						checkIdentifierName(propId->name());
						id = std::make_shared<Identifier>(curIdentifiersHolder(), propId->name(), propId->type());
						id->setFlags(propId->getFlags());
						addIdentifier(id);
					}
				  curObject()->addProp(id, aLocation, mLexer->currentFile());
				}
			}
			addUndeclaredProperties(base, aLocation);
		}
	}
}

void NateParser::addUndeclaredDefines(const ObjectPtr& aObject, const yy::parser::location_type& aLocation)
{
	if (!aObject->isRole())
	{
		for (const auto& base : aObject->getBases())
		{
			if (base->isRole())
			{
				for (auto& baseMethod : base->defines().get())
				{
					auto foundMethod = aObject->basesGetLike(baseMethod, base);
					if (!foundMethod)
					{
						if (!aObject->defines().getLike(baseMethod))
						{
							DefinePtr newDefine = aObject->defines().add(DefinePtr(new Define(*baseMethod)));
							newDefine->setFlag(Method::Overriden);
							addStatement(std::make_shared<StatDefine>(location(aLocation), newDefine, false /*aImpOnly*/, true /*aIsDecl*/));
						}
					}
				}
				
				addUndeclaredDefines(base, aLocation);
			}
		}
	}
}

void NateParser::endObject()
{
	popIdentifiersHolder();
	popRecordsHolder();
	popTypesHolder();
	popDefinesHolder();
	setCurObject(ObjectPtr());
}

void NateParser::endImplementObject()
{
	if (curObject()->is(Type::ObjectImpl))
	{
		for (const auto& define : curObject()->defines().get())
		{
			if (!define->is(Method::Undeclared) && !define->is(Method::Defined))
			{
				error("Undefined method: " + define->signature());
			}
		}
	}

	for (auto const& base : curObject()->getBases())
	{
		if (base->isRole())
		{
			for (auto const& define : base->defines().get())
			{
				if (!define->is(Method::Defined))
				{
					auto const& like = curObject()->defines().getLike(define);
					if (!like || !like->is(Method::Defined))
					{
						error("Undefined method: " + define->signature());
						//std::cerr << *define << std::endl;
					}
				}
			}
		}
	}

	endObject();
}

ObjectPtr NateParser::getObject(const std::string& aId)
{
	auto iter = std::find_if(mObjects.cbegin(), mObjects.cend(),
												 	 [&](ObjectPtr const& aObject)
													 { return aObject->name() == aId; });
	return iter != mObjects.cend() ? *iter : ObjectPtr();
}

void NateParser::addObjectBase(ObjectPtr& aCurObject, const ObjectPtr& aObject)
{
	if (aObject->isRole())
	{
		error("Base object must not be a role: " + aObject->name());
	}
	aCurObject->addBase(aObject);
}

void NateParser::addObjectRole(ObjectPtr& aCurObject, const ObjectPtr& aObject)
{
	if (!aObject->isRole())
	{
		error("Base role must not be an object: " + aObject->name());
	}
	aCurObject->addBase(aObject);
}

ObjectPtr NateParser::curObject() const
{
	return mCurObject;
}

void NateParser::setCurObject(const ObjectPtr& aObject)
{
	mCurObject = aObject;
}

void NateParser::addCode()
{
	pushScope(std::make_shared<Scope>("code", IIdentifiersHolder::ScopeFlag::Local));
	mCodes.emplace_back(new Code());
	mMethodType = MethodType::Code;
	mSpecialWord = static_cast<int32_t>(SpecialWord::None);
}

void NateParser::endCode()
{
	auto size = curCode()->code().size();
	if (size > 0 && curCode()->code()[size - 1] == '\n')
	{
		curCode()->code().pop_back();
	}

	curCode()->endDecl();
	popScope();
}

CodePtr NateParser::curCode() { return mCodes.back(); }

CodePtr NateParser::getCode(const CodePtr& aCode)
{
	auto iter = std::find_if(mCodes.cbegin(), mCodes.cend(),
												 	 [&](const CodePtr& aCodeFromList)
													 { return aCode->equals(*aCodeFromList); });
	return iter != mCodes.cend() ? *iter : CodePtr();
}

void NateParser::startInbrackets(const yy::parser::location_type& aLocation, const std::string& type)
{
	mMathStart = aLocation;
	mInBracketsType = type;
	mMath = NateParserMath::Math();
	mMath.x = 0;
	mMath.y = aLocation.begin.line;
	data.inBracketsCode.clear();
	if (type != "code" && type != "math")
	{
		error("Unknown {{ type: " + type);
	}
}

void NateParser::endInbrackets()
{
	if (mInBracketsType == "math")
	{
		std::string code = mMathParser->doMath(mMath);
		std::cerr << code << std::endl;
		mLexer->unputString(code);
	}
}

void NateParser::addInbracketsStatWord(const std::string& aWord,
											                 const yy::parser::location_type& aLocation)
{
	if (mInBracketsType == "math")
	{
		if (aWord == "\n")
		{
			mMathStart.begin.column = 0;
		}
		else
		{
			int y = aLocation.begin.line - mMathStart.begin.line;
			int x = aLocation.begin.column - mMathStart.begin.column;
			while (y >= mMath.matrix.size())
			{
				mMath.matrix.push_back(NateParserMath::MathVector());
			}

			while (x >= 0 && x >= mMath.matrix[y].size())
			{
				mMath.matrix[y].push_back(32);
			}

			std::string::const_iterator iter = aWord.begin();
			while (iter != aWord.end())
			{
				uint32_t kar = utf8::next(iter, aWord.end());
				mMath.matrix[y].push_back(kar);
			}
		}
	}
	else
	{
		data.inBracketsCode += aWord;
	}
}

void NateParser::addDefine(bool aInObject)
{
	addGivenDefine(DefinePtr(new Define()), aInObject);
}

void NateParser::addGivenDefine(const DefinePtr& aDefine, bool aInObject)
{
	curDefinesHolder()->defines().add(aDefine);
	mCurDefine = curDefinesHolder()->defines().get().back();
	pushDefineScope(std::make_shared<Scope>("define", IIdentifiersHolder::ScopeFlag::Local));
	mMethodType = MethodType::Define;
	mSpecialWord = static_cast<int32_t>(SpecialWord::None);
	if (aInObject)
	{
		curDefine()->setObject(curObject());
	}
}

void NateParser::deleteCurDefine()
{
	curDefinesHolder()->defines().get().pop_back();
}

void NateParser::doStartDefine(bool aIsDecl, bool aIsImpl,
										           const yy::parser::location_type& aLocation)
{
	curDefine()->endDecl();
	mDefineDecl = aIsDecl;

	if (curDefine()->isObjectMethod())
	{
		if (!aIsDecl)
		{
			DefinePtr defineDecl = curObject()->defines().getLike(curDefine());
			if (defineDecl)
			{
				if (defineDecl->is(Method::Defined))
				{
					error("Redefinition of method: " + defineDecl->signature());
				}

				defineDecl->setFlag(Method::Defined);
				pushStatsHolder(addStatement(std::make_shared<StatDefine>(location(aLocation), defineDecl, false /*aImpOnly*/, false /*aIsDecl*/)));
			}
			else
			{
				curDefine()->setFlag(Method::Defined);
				curDefine()->setFlag(Method::Undeclared);
				pushStatsHolder(addStatement(std::make_shared<StatDefine>(location(aLocation), curDefine(), true /*aImpOnly*/, false /*aIsDecl*/)));
			}
		}
		else
		{
			DefinePtr defineDecl = curObject()->basesGetLike(curDefine());
			if (defineDecl)
			{
				if (curObject()->isRole())
				{
					error("Redefinition of method of base role: " + defineDecl->signature());
				}
				curDefine()->setFlag(Method::Overriden);
			}
			
		  addStatement(std::make_shared<StatDefine>(location(aLocation), curDefine(), false /*aImpOnly*/, true /*aIsDecl*/));
		}

		if (curDefine()->isStatic())
		{
			if (curDefine()->is(Method::Final))
			{
				error("Not allowed keyword for object method: final");
			}
			else if (curObject()->isRole())
			{
				error("Role method requires 'me' object");
			}
			addIdentifier(std::make_shared<Identifier>(curIdentifiersHolder(), "me", curObject()));
		}
		else if (curDefine()->is(Method::Final) && curObject()->isRole())
		{
			error("Not allowed keyword for role method: final");
		}
	}
	else
	{
		if (curDefine()->is(Method::Final))
		{
			error("Not allowed keyword: final");
		}
		
		auto stat = addStatement(std::make_shared<StatDefine>(Location(aLocation, mLexer->currentFile()), curDefine(), false /*aImpOnly*/, aIsDecl));
		if (!aIsDecl)
		{
			pushStatsHolder(stat);
		}
	}

	curDefine()->createCodeCall();
}

void NateParser::doEndDeclDefine(const yy::parser::location_type& aLocation)
{
	mCurDefine.reset();
	popDefineScope();
}

void NateParser::doEndDefine(const yy::parser::location_type& aLocation)
{
	mCurDefine.reset();
	popDefineScope();
	popStatsHolder();
}

DefinePtr NateParser::curDefine() const { return mCurDefine; }

void NateParser::declareProperties(const std::vector<std::string>& aNames,
																	 const TypePtr& aType,
																	 const std::vector<std::string>& flags,
																   const yy::parser::location_type& aLocation)
{
	for (auto const& name : aNames)
	{
		checkIdentifierName(name);
		IdentifierPtr id = std::make_shared<Identifier>(curIdentifiersHolder(), name, aType);
		id->setFlag(Identifier::Property);
		addIdentifier(id);

		optionalError(id->setFlagStrings(flags));
		curObject()->addProp(id, aLocation, mLexer->currentFile());
	}
}

IdentifierPtr NateParser::doProp(const std::string& aName,
						          					const TypePtr& optType,const std::vector<std::string>& flags,const yy::parser::location_type& aLocation)
{
	IdentifierPtr result = getIdentifier(aName, curIdentifiersHolder().get());
	bool declared = (bool)result;
	if (!declared)
	{
		if (optType->empty())
		{
			error("Undeclared property requires type");
		}

		declareProperties({ aName }, optType, flags, aLocation);
		result = getIdentifier(aName, curIdentifiersHolder().get());
	}
	else
	{
		if (!optType->empty())
		{
			error("Redefinition of type of property");
		}
		else if (!flags.empty())
		{
			error("Redefinition of flags of property");
		}
	}
				
	pushStatsHolder(addStatement(std::make_shared<StatProperty>(location(aLocation), result, curObject(), !declared)));

	return result;
}

void NateParser::doEndProp(const yy::parser::location_type& aLocation)
{
	popStatsHolder();
}

void NateParser::doPropDefine(const IdentifierPtr& aIdentifier, Property::PropType aPropType,
										          const yy::parser::location_type& aLocation)
{
	const std::string method = aPropType == Property::PropType::Get ? "get" : "set";

	bool isDeclared = curObject()->isPropDeclared(aIdentifier, aPropType);
	if (!isDeclared && !aIdentifier->is(Identifier::Undeclared))
	{
	  error("Undeclared " + method + " method for property: " + aIdentifier->name());
	}

	if (curObject()->isPropDefined(aIdentifier, aPropType))
	{
	  error("Redefined " + method + " method for property: " + aIdentifier->name());
	}

	if (aIdentifier->is(Identifier::ReadOnly) && aPropType != Property::PropType::Get)
	{
		error("Defined " + method + " method for readonly property: " + aIdentifier->name());
	}

	Object::PropState propState = curObject()->getPropState(aIdentifier, aPropType);
	propState.state = Object::PropState::State::Defined;
	curObject()->setPropState(aIdentifier, aPropType, propState); 
	
	PropertyPtr property{new Property()};
	property->setPropType(aPropType);

	addGivenDefine(property, true);
	curDefine()->setType(aIdentifier->type());
	StatProperty* statProperty = dynamic_cast<StatProperty*>(mStatHolders.back().get());
	statProperty->setProperty(aPropType, property);
	pushStatsHolder(addStatement(std::make_shared<StatDefine>(location(aLocation), curDefine(), !isDeclared, false)));
	mDefineDecl = false;
	if (aPropType != Property::PropType::Get)
	{
		IdentifierPtr value = std::make_shared<Identifier>(curIdentifiersHolder(), "value", aIdentifier->type());
		addIdentifier(value);
	}
}

void NateParser::doEndPropDefine(const yy::parser::location_type& aLocation)
{
	deleteCurDefine();
	popIdentifiersHolder();
	popStatsHolder();
}

void NateParser::doExpressionStatement(const Expr& aExpr, const yy::parser::location_type& aLocation)
{
  addStatement(std::make_shared<StatExpr>(Location(aLocation, mLexer->currentFile()), aExpr));
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

		curMethod()->addArgWord(aWord);
		mSpecialWord = static_cast<int32_t>(SpecialWord::None);
	}
}

void NateParser::addArgId(const std::string& aId, const TypePtr& aType, const std::string& inOut)
{ 
	TypePtr type = aType;

	if (type->empty())
	{
    if (!Identifier::isNameMe(aId))
		{
			error("Expected a type");
		}
		else
		{
			type = curObject();
		}
	}

	auto id = std::make_shared<Identifier>(curIdentifiersHolder(), aId, type);
	addIdentifier(id);
	curMethod()->addArgId(id);
  if (!inOut.empty())
  {
    curMethod()->curArg().setFlagString(inOut);
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
		else if (curObject()->name() != type->name())
		{
      error("The id 'me' must be of type: " + curObject()->name());
		}

    data.objectMe = true;
  }
}

void NateParser::error(const std::string& anError) const
{
	error(mLexer->fileLocation(), anError);
}

void NateParser::error(const std::string& aLocationString, const std::string& anError) const
{
	std::cerr << aLocationString << ": " << anError << std::endl;
	++mErrors;
}

void NateParser::error(const yy::position& aPosition, const std::string& anError) const
{
	error(mLexer->fileLocation(yy::location(aPosition)), anError);
}

void NateParser::optionalError(const std::string& anError) const
{
	if (!anError.empty())
	{
		error(anError);
	}
}

void NateParser::warning(const std::string& aWarning) const
{
	std::cerr << "Warning: " << mLexer->fileLocation() << ": " << aWarning << std::endl;
	++mWarnings;
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

		if (iter != name.cend() && ok)
		{
			auto next = iter;
			utf8::next(next, name.cend());
			pos = utf8::distance(iter, next);
			if (pos != 1 || *iter != 'i' || next == name.cend())
			{
				id = getIdentifier(alias(std::string(iter, next)));
			}
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
			auto var = identifiersHolder->getIdentifier(aName);
			if (var != nullptr)
			{
				return var;
			}
		}

		return IdentifierPtr();
	}
	else
	{
		return aIdentifiersHolder->getIdentifier(aName);
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

bool NateParser::checkProperty(std::ostringstream& error, const ExprNodesCIter& nodeIter) const
{
	bool ok = true;

	if (nodeIter->is(Expr::Property))
	{
		auto id = nodeIter->id();
		if (id)
		{
			IIdentifiersHolderPtr holder = id->identifiersHolder().lock();
			if (holder)
			{
				if (holder != curObject() && (!curDefine() || curDefine()->object() != curObject()))
				{
					error << "Use me's reference when using base object property: " << id->name();
					ok = false;
				}
			}
		}
	}
	
	return ok;
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
			auto type = getType(aName, typesHolder.get());
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

void NateParser::checkIfBetterMatch(const MethodPtr& aMethod,
																		const ExprNodesCIter& aStartIter,
																		const ExprNodesCIter& aEndIter,
																		Match& aMatch,
																		bool aLeftToRight,
																		int aDebug)
{
	if (aMethod->matches(aStartIter, aEndIter, aDebug) &&
			(!aMatch.methodFound || 
			 (aMethod->priority() > aMatch.methodFound->priority() ||
			  (aMethod->priority() == aMatch.methodFound->priority() &&
         ((aLeftToRight  && aStartIter <= aMatch.nodeStartIter) ||
				  (!aLeftToRight && aStartIter >= aMatch.nodeStartIter))))))
	{
		Method::MatchResult matchResult = aMethod->checkArgTypes(*this, aStartIter, aEndIter, aDebug);
		
		if (matchResult.matches)
		{
			if ((!aMatch.methodFound || 
					 aMethod->priority() > aMatch.methodFound->priority() ||
					 (aMethod->priority() == aMatch.methodFound->priority() &&
						((aLeftToRight  && aStartIter < aMatch.nodeStartIter) ||
						 (!aLeftToRight && aStartIter > aMatch.nodeStartIter)))))
			{
				if (aDebug)
				{
					std::cerr << "Better match 1: " << " "
										<< aMethod->code() 
										<< matchResult.castCount << " "
										<< (aMatch.methodFound  ? aMatch.methodFound->code() : "None") << " "
										<< aMatch.matchResult.castCount << " "
										<< std::distance(aStartIter, aMatch.nodeStartIter) << " "
										<< std::distance(aEndIter, aMatch.nodeEndIter) << " "
										<< std::endl;
				}

				aMatch.matchResult = matchResult;
				aMatch.methodFound = aMethod;
				aMatch.nodeStartIter = aStartIter;
				aMatch.nodeEndIter = aEndIter;
			}
			else if (aMethod->priority() == aMatch.methodFound->priority() &&
							 ((aLeftToRight  && aStartIter == aMatch.nodeStartIter) ||
								(!aLeftToRight && aStartIter == aMatch.nodeStartIter)) &&
							 matchResult.castCount < aMatch.matchResult.castCount)
			{

				if (aDebug)
				{
					std::cerr << "Better match 2: " << " "
										<< aMethod->code() 
										<< matchResult.castCount << " "
										<< (aMatch.methodFound ? aMatch.methodFound->code() : "None") << " "
										<< aMatch.matchResult.castCount << " "
										<< std::distance(aStartIter, aMatch.nodeStartIter) << " "
										<< std::distance(aEndIter, aMatch.nodeEndIter) << " "
										<< std::endl;
				}

				aMatch.matchResult = matchResult;
				aMatch.methodFound = aMethod;
				aMatch.nodeStartIter = aStartIter;
				aMatch.nodeEndIter = aEndIter;
			}
			else
			{
				if (aDebug)
				{
					std::cerr << "No match1: " << " "
										<< aMethod->code() << " "
										<< matchResult.castCount << " "
										<< (aMatch.methodFound ? aMatch.methodFound->code() : "None") << " "
										<< aMatch.matchResult.castCount << " "
										<< std::distance(aStartIter, aMatch.nodeStartIter) << " "
										<< std::distance(aEndIter, aMatch.nodeEndIter) << " "
										<< std::endl;
				}

				aMatch.matchedMethod = aMethod;
				aMatch.matchResult = matchResult;
			}
		}
		else
		{
			if (aDebug)
			{
				std::cerr << "No match2: " << " "
					        << aMethod->code() << " "
					        << matchResult.castCount << " "
									<< (aMatch.methodFound ? aMatch.methodFound->code() : "None") << " "
					        << aMatch.matchResult.castCount << " "
									<< std::distance(aStartIter, aMatch.nodeStartIter) << " "
									<< std::distance(aEndIter, aMatch.nodeEndIter) << " "
									<< std::endl;
			}

			if (!aMatch.matchedMethod || !matchResult.error.empty())
			{
				aMatch.matchedMethod = aMethod;
				aMatch.matchResult = matchResult;
			}
		}
	}
}

void NateParser::checkLeftToRightMethod(const MethodPtr& aMethod,
																			  const Expr& aExpr,
																			  Match& aMatch, 
																				int aDebug)
{
	auto size = aMethod->args().size();

	for (auto startIter = aExpr.nodes().cbegin();
			 size <= static_cast<size_t>(std::distance(startIter, aExpr.nodes().cend())); ++startIter)
	{
		auto endIter = startIter + size;
		checkIfBetterMatch(aMethod, startIter, endIter, aMatch, true, aDebug);
	}
}

void NateParser::checkRightToLeftMethod(const MethodPtr& aMethod,
																			  const Expr& aExpr,
																			  Match& aMatch, 
																				int aDebug)
{
	auto size = aMethod->args().size();

	for (auto endIter = aExpr.nodes().cend();
			 size <= static_cast<size_t>(std::distance(aExpr.nodes().cbegin(), endIter)); --endIter)
	{
		auto startIter = endIter - size;
		checkIfBetterMatch(aMethod, startIter, endIter, aMatch, false, aDebug);
	}
}

void NateParser::checkIfMethod(const MethodPtr& aMethod, const Expr& aExpr, Match& aMatch, int aDebug)
{
	auto size = aMethod->args().size();
	if (size <= aExpr.nodes().size() && size > 0)
	{
		if (aMethod->is(Code::RightLeft))
		{
			checkRightToLeftMethod(aMethod, aExpr, aMatch, aDebug);
		}
		else
		{
			checkLeftToRightMethod(aMethod, aExpr, aMatch, aDebug);
		}
	}
}

void NateParser::checkIfObjectDefine(const ObjectPtr& aObject, const Expr& aExpr, int aDebug,
															     	 Match& aMatch)
{
	for (auto const& define : aObject->defines().get())
	{
		checkIfMethod(define, aExpr, aMatch, aDebug);

		for (auto const& base : aObject->getBases())
		{
			checkIfObjectDefine(base, aExpr, aDebug, aMatch);
		}
	}
}

Expr NateParser::evaluate(const Expr& aExpr, int aDebug)
{
	Expr result = aExpr;
	aDebug = std::max(gDebug, aDebug);
	if (aDebug) std::cerr << "+++++ " << aExpr.text() << " " << aExpr << std::endl;

	Match match;
	match.nodeStartIter = aExpr.nodes().cend();
	match.nodeEndIter = aExpr.nodes().cend();

	if (aExpr.nodes().size() > 1 || aExpr.is(Expr::Word))
	{
		for (auto& object : mObjects)
		{		
			checkIfObjectDefine(object, aExpr, aDebug, match);
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
				match.methodFound->createCode(curDefine(), match.nodeStartIter, match.nodeEndIter, aDebug);
			if (!evalResult.error.empty())
			{
				error(evalResult.error);
			}
			else if (evalResult.code == "NI")
			{
				error("Not implemented: " + aExpr.text());
			}

			if (aDebug) std::cerr << "******* " << (evalResult.type ? *evalResult.type : Type()) << " " << evalResult.code << std::endl;
			
			Expr::Node node(evalResult.origText + " ", evalResult.code, evalResult.type);
			node.setFlags(evalResult.flags);

			Expr newExpr;
			newExpr.addNodes(aExpr.nodes().cbegin(), match.nodeStartIter);
			newExpr.addNode(node);
			newExpr.addNodes(match.nodeEndIter, aExpr.nodes().cend());
			if (newExpr.nodes().size() == 1)
			{
				newExpr = node;
			}
			//std::cerr << newExpr << std::endl;
			return evaluate(newExpr, aDebug);
		}
		else
		{
			//if (!match.matchedMethod)
			{
				if (!match.matchResult.error.empty())
				{
					error(result.text() + ": " + 
								match.matchResult.error);
				}
			}
		}
	}
	
	if (aExpr.nodes().size() == 1)
	{
		result = Expr(aExpr.nodes().front());
		std::ostringstream errorStr;
		if (!checkProperty(errorStr, aExpr.nodes().begin()))
		{
			error(errorStr.str());
		}
	}

	if (result.nodes().size() != 1 || result.is(Expr::Word))
	{
		if (match.matchResult.error.empty())
		{
			std::cerr << result << std::endl;
			error("Bad expression: " + result.text());
		}

		return Expr("1", getType("int-32"));
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
	else if (aCommand == "UNQUOTE")
	{
		if (result.size() > 1 && result[0] == '"')
		{
			result = result.substr(1, result.size() - 2);
		}
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
	std::string& code = aExpr.code();
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

void NateParser::declareLocalIdentifiers(
				bool aIsConst,
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
			initValue = Expr("default", "{}", type);
			initValue.setFlag(Expr::Default);
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

			initValue = Expr(*initIter++);
			initValue.castToType(aType);
		}
		else 
		{
			initValue = Expr(aNames.front(), codeId(aNames.front()), type);
			initValue.setFlag(Expr::ConstExpr, aIsConst);
		}

		IdentifierPtr id = std::make_shared<Identifier>(curIdentifiersHolder(), name, type, initValue);
		id->setFlag(Identifier::Const, aIsConst);
		id->setFlag(Identifier::ObjectImpl, data.inObjectImpl);
		
		addIdentifier(id);

		if (id->type()->is(Type::Abstract) && !id->type()->is(Type::Object))
		{
			error("Abstract type: " + id->type()->name());
			return;
		}

		if (id->type()->is(Type::Unknown))
		{
			error("Unknown type: " + id->type()->name());
			return;
		}

		//if (aIdentifier->is(Identifier::Const) && !aIdentifier->initValue().is(Expr::ConstExpr))
		//{
		//	error("Expected constant expression.");
		//}

		addStatement(std::make_shared<StatDeclareLocal>(Location(aLocation, mLexer->currentFile()), id, initValue));
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
		/*else
		{
			warning("hides declaration of: " + aName);
		}*/
	}
}

void NateParser::declareRecordIdentifiers(
				bool aIsConst,
				const std::vector<std::string>& aNames,
				const TypePtr& aType,
				const std::vector<Expr>& aInitValues,
				const yy::parser::location_type& aLocation)
{
	declareLocalIdentifiers(aIsConst, aNames, aType, aInitValues, !NateParser::InitializeVariables, aLocation);
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

std::string NateParser::codeId(const std::string& aName, Scope* aScope)
{
	return getOrFakeIdentifier(aName, aScope)->codeName();
}
