#pragma once

#include <string>
#include <map>
#include <list>
#include <stack>
#include <set>
#include <limits>
#include <fstream>
#include <memory>
#include <variant>
#include <optional>

#include "NateFunctions.h"
#include "NateParser.tab.h"
#include "Code.h"
#include "Define.h"
#include "Identifier.h"
#include "Scope.h"
#include "Record.h"
#include "Object.h"

namespace yy
{
	class Lexer;
	class parser;
};

class ExprNode;

class NateParser
{
public:
	static const bool InitializeVariables = true;

	struct ParseData
	{
		std::string prevWriteSink;
		std::string prevReadSource;
		bool prevWasValue = false;
		std::stack<Expr> ifExpr;
		std::stack<std::string> ifId;
		std::string forId;
		std::list<CodePtr> curParsedCodes;
		bool inObject = false;
		bool inObjectImpl = false;
		bool objectMe = false;
		std::stack<RecordPtr> curRecord;
		bool assignExpr = false;
		IdentifierPtr propId;
		WithFlags* flagsHolder = nullptr;
	};

	enum class FileType
	{
		Normal,
		ObjectDecl,
		ObjectImpl
	};
	
	ParseData data;

	NateParser(const std::string& aFilename, std::istream& aIn, std::ostream& aOut,
						 FileType aFileType = FileType::Normal);
	virtual ~NateParser();
	std::string in(int aOffset = 0) const;
	int parse();
	void import(const std::string& aName);
	void importBaseObject(const std::string& aInObjectName);
	std::string baseObjectName() const;
	void addAlias(const std::string& aName, const std::string& aValue);
	std::string alias(const std::string& aString);
	bool isReservedName(const std::string& aString) const;
	void addLeftMonomial(const std::string& aWord);
	bool isLeftMonomial(const std::string& aWord) const;
	void addWantsUnary(const std::string& aWord);
	bool wantsUnary(const std::string& aWord) const;
	
	yy::Lexer* getLexer();

	void pushScope(const ScopePtr& aScope);
	void popScope();
	void pushDefineScope(const ScopePtr& aScope);
	void popDefineScope();
	IIdentifiersHolderPtr& curIdentifiersHolder();
	IRecordsHolderPtr& curRecordsHolder();
	ITypesHolderPtr& curTypesHolder();
	IDefinesHolderPtr& curDefinesHolder();
	
	void error(const yy::position& aPosition, const std::string& anError) const;
	void error(const std::string& aLocationString, const std::string& anError) const;
	void error(const std::string& anError) const;
	void optionalError(const std::string& anError) const;
	void warning(const std::string& aWarning) const;
	int errorCount() const { return mErrors; }
	int warningCount() const { return mWarnings; }
	void addType(TypePtr aType, const std::string& aName = "");
	ITypesHolderPtr getTypesHolder(const TypePtr& aType) const;
	TypePtr getType(const std::string& aName, ITypesHolder* aTypesHolder = nullptr);
	bool isType(const std::string& aName);
	TypePtr determineType(const std::string& aName);
	TypePtr makeType(const std::string& aValue);
	IdentifierPtr getIdentifier(const std::string& aName, IIdentifiersHolder* aIdentifiersHolder = nullptr);
	IdentifierPtr getOrFakeIdentifier(const std::string& aName, IIdentifiersHolder* aIdentifiersHolder = nullptr);
	void addIdentifier(const IdentifierPtr& aIdentifier);
	std::tuple<bool, std::string> makeIdOrWord(const std::string& aOrig, const std::string& aString);
	std::string uniqueName() const;
	TypePtr getNumberType(std::string& aString);
	
	void addObject(const ObjectPtr& aObject);
	void endObject();
	void addUndeclaredProperties(const ObjectPtr& aObject);
	void endDeclObject();
	void endImplementObject();
	void startObject(const ObjectPtr& aObject);
	ObjectPtr curObject();
	void checkObject(const ObjectPtr& aObject);
	void setCurObject(const ObjectPtr& aObject);
	ObjectPtr getObject(const std::string& aId);
	void addObjectBase(const ObjectPtr& aObject);
	void addObjectRole(const ObjectPtr& aObject);
	void declareProperties(const std::vector<std::string>& aNames,
												 const TypePtr& aType,
												 const std::vector<std::string>& flags,
												 const yy::parser::location_type& aLocation);
	void defineProp(const IdentifierPtr& aIdentifier, Object::PropType aPropType);
	void endDefineProp(const IdentifierPtr& aIdentifier, Object::PropType aPropType);
	
	void startMath(const yy::parser::location_type& aLocation);
	void endMath();
	void addMathStatWord(const std::string& aWord,
											 const yy::parser::location_type& aLocation);
	void addCode();
	void endCode();
	CodePtr curCode();
	CodePtr getCode(const CodePtr& aCode);

	void addDefine(bool aInObject);
	void declareDefine(bool aIsDecl = false);
	void endDefine();
	void deleteCurDefine();
	DefinePtr curDefine();

	MethodPtr curMethod();
	void addArgWord(const std::string& aWord);
	void addArgId(const std::string& aId, const TypePtr& aType, const std::string& inOut);

	void declareLocalIdentifiers(
					bool aIsConst,
					const std::vector<std::string>& aNames,
					const TypePtr& aType,
					const std::vector<Expr>& aInitValues,
					bool initializeVariables,
					const yy::parser::location_type& aLocation);
	void codeStartRecord(const RecordPtr& aRecord,
											 const yy::parser::location_type& aLocation);
	void declareRecordIdentifiers(
					bool aIsConst,
					const std::vector<std::string>& aNames,
					const TypePtr& aType,
					const std::vector<Expr>& aInitValues,
					const yy::parser::location_type& aLocation);
	
	std::string codeExpr(const Expr& aValue);
	void codeStartProgram(const yy::parser::location_type& aLocation);
	void codeEndProgram(const yy::parser::location_type& aLocation);
	void codeStartScope();
	void codeEndScope();
	void codeCodeInclude();
	void codeDeclareLocalIdentifier(bool aExtern,
																	const IdentifierPtr& aIdentifier,
																	bool initializeVariables,
																	const yy::parser::location_type& aLocation = yy::parser::location_type(yy::position(), yy::position()));
	void codeEndRecord(const yy::parser::location_type& aLocation);
	void codeStartDeclObject(const yy::parser::location_type& aLocation);
	void codeEndDeclObject();
	void codeStartImplObject(const yy::parser::location_type& aLocation);
	void codeEndImplObject();
	void codeObjectMethodHeaderDecl(const ObjectPtr& aObject, const DefinePtr& aDefine);
	void codeDeclareProperty(const IdentifierPtr& aId, const yy::parser::location_type& aLocation);
	void codeDefaultPropertyImpl(const IdentifierPtr& propId);

	std::string codePropHeader(bool aAddObjectName,
													 	const IdentifierPtr& aId, 
													 	Object::PropType aPropType);
	void codeAssign(const std::vector<Expr>& aExpressions, Expr& aValue, const yy::parser::location_type& aLocation);
	std::string codeId(const std::string& aName, Scope* aScope = nullptr);
	void codeOutputStart(const std::string& aStream, const yy::parser::location_type& aLocation);
	void codeOutput(const std::string& aString);
	void codeOutput(const Expr& aValue);
	void codeOutputEnd(bool aAddEnd = true);
	void codeWriteStart(const Expr& aValue, const yy::parser::location_type& aLocation);
	void codeReadStart(const Expr& aValue, const yy::parser::location_type& aLocation);
	void codeInputStart(const std::string& aStream, const yy::parser::location_type& aLocation);
	void codeInputSpace();
	void codeInputNoSpace();
	void codeInput(const Expr& aValue);
	void codeInputEnd(bool aAddEnd = true);
	void codeDataStart(const std::string& aId, const yy::parser::location_type& aLocation);
	void codeDataEnd(const std::string& aId, const yy::parser::location_type& aLocation);
  void codeDataOutputEnd(bool aAddEnd);
	void codeIf(const Expr& aValue, const yy::parser::location_type& aLocation);
	void codeElseIf();
	void codeElse(const yy::parser::location_type& aLocation);
	void codeEndIf();
	void codeIfIs(const Expr& aValue, const std::string& idName, const yy::parser::location_type& aLocation);
	void codeIs(const Expr& aValue, const Expr& aIfExpr, const yy::parser::location_type& aLocation);
	void codeElseIs(const yy::parser::location_type& aLocation);
	void codeBeginIs();
	void codeEndIs(const yy::parser::location_type& aLocation);
	void codeEndIfIs(const yy::parser::location_type& aLocation);
  void codeInitLoop(const yy::parser::location_type& aLocation);
  void codeStartLoop();
  void codeStartForStepLoop(const std::string& aId, 
														const TypePtr& aType, 
														bool aDownTo,
														const Expr& aStart,
														const Expr& aEnd,
														const Expr& aStep);
  void codeStartForRangeLoop(const std::string& aId, 
														 const Expr& aRange);
  void codeEndLoop();
  void codeLoopWhile(const Expr& aExpr, const yy::parser::location_type& aLocation);
  void codeReturn(const Expr& aValue, const yy::parser::location_type& aLocation);
  void codeExpressionStatement(const Expr& aExpr, const yy::parser::location_type& aLocation);
  Expr evaluate(const Expr& aExpr, int aDebug = 0);
	void printLineNr(const yy::parser::location_type& aLocation);

private:
	void checkIdentifierName(const std::string& aName);
	void initTypesAndObjects();
	void initOutput();
	void parseFile(const std::string& aFilename);
	void pushIdentifiersHolder(const IIdentifiersHolderPtr& aIdentifiersHolder);
	void popIdentifiersHolder();
	void pushRecordsHolder(const IRecordsHolderPtr& aRecordsHolder);
	void popRecordsHolder();
	void pushTypesHolder(const ITypesHolderPtr& aTypesHolder);
	void popTypesHolder();
	void pushDefinesHolder(const IDefinesHolderPtr& aDefinesHolder);
	void popDefinesHolder();
	
	void codeObjectBases(const ObjectPtr& aObject);

	struct Match
	{
		MethodPtr	      		methodFound;
		ExprNodesCIter			nodeStartIter;
		ExprNodesCIter			nodeEndIter;
		MethodPtr	      		matchedMethod;
		Method::MatchResult matchResult;
	};

	std::string handleCompileCommand(const std::string& aCommand, const std::string& aData);
	void handleCompileCommands(Expr& aExpr);
	void checkIfBetterMatch(const MethodPtr& aMethod,
													const ExprNodesCIter& aStartIter,
													const ExprNodesCIter& aEndIter,
													Match& aMatch,
													bool aLeftToRight,
													int aDebug);
  void checkLeftToRightMethod(const MethodPtr& aMethod,
	                            const Expr& aExpr,
	                            Match& aMatch,
															int aDebug = 0);
  void checkRightToLeftMethod(const MethodPtr& aMethod,
	                            const Expr& aExpr,
	                            Match& aMatch,
															int aDebug = 0);
	void checkIfMethod(const MethodPtr& aMethod, const Expr& aExpr, Match& aMatch,
										 int aDebug = 0);
	void checkIfObjectDefine(const ObjectPtr& aObject, const Expr& aExpr, int aDebug,
								 					 Match& aMatch);
	void unput(const std::string::const_iterator& aStart,
						 const std::string::const_iterator& aEnd);
	void codeOutputNew();
	std::string makeTempDir();
	bool importObjectDefinition(const std::string& aLibrary, const std::string& aName);
	std::string typeScopeName() const;
	
	struct Math;
	struct Position;
	struct MathValue;

	typedef std::vector<MathValue> MathVector;
	typedef std::vector<MathVector> MathMatrix;
	typedef std::optional<Position> OptPosition;
	
	struct Math
	{
		size_t x = 0;
		size_t y = 0;
		MathMatrix matrix;
	};

	struct MathValue
	{
		MathValue(uint32_t aValue)
			: value(aValue) {}
		
		MathValue(const Math& aMath1, const Math& aMath2, const std::string& aOper)
			: value(0U), 
			  oper(aOper), 
			  embedded1(aMath1), 
			  embedded2(aMath2) {}

		uint32_t value;
		std::string oper;
		Math embedded1;
		Math embedded2;
	};

	struct Position
	{
		int x = 0;
		int y = 0;
	};


	yy::position mathPos(const Math& aMath, 
											 const Position& aPosition) const;
	void doMath(Math& aMath);
	void fillUpMath(Math& aMath);
	void printMath(const Math& aMath) const;
	std::string mathString(const Math& aMath) const;
	void doMathParsing(Math& aMath);
	void embedSubMath(Math& aMath, 
										const Math& aSubMath1, 
										const Math& aSubMath2, 
										const std::string& aOper,
										const Position& aLeftUpperPosition,
										const Position& aRightLowerPosition
										);
	void doMathParentheses(Math& aMath);
	void doMathDivision(Math& aMath);
	OptPosition findAny(const Math& aMath, 
											uint32_t aSearchChar) const;
	OptPosition findMatchingRight(const Math& aMath, 
																const Position& aLeftPosition,
																uint32_t aSearchChar) const;
	OptPosition findRepeatingRight(const Math& aMath, 
																 const Position& aLeftPosition,
																 uint32_t aSearchChar) const;
	OptPosition findMatchingDown(const Math& aMath, 
														   const Position& aUpperPosition,
															 uint32_t aInbetweenChar,
															 uint32_t aSearchChar) const;
	OptPosition findUntilUp(const Math& aMath, 
													const Position& aLowerPosition,
													uint32_t aSearchChar) const;
	OptPosition findUntilDown(const Math& aMath, 
												  	const Position& aUpperPosition,
												  	uint32_t aSearchChar) const;

	void mathError(const Math& aMath, 
								 const Position& aPosition,
								 uint32_t aBadChar,
							 	 uint32_t aInbetweenChar,
								 uint32_t aSearchChar) const;
	Math getSubMath(const Math& aMath, 
									const Position& aLeftUpperPosition, 
									const Position& aRightLowerPosition);
	

	std::unique_ptr<yy::Lexer>	mLexer;
	std::unique_ptr<yy::parser>	mParser;
	std::list<ObjectPtr>        mObjects;
	ObjectPtr                   mCurObject;
	DefinePtr                   mCurDefine;
	std::ostream*               mSavedOut = nullptr;
	std::list<IIdentifiersHolderPtr> mIdentifiersHolders;
	std::list<IRecordsHolderPtr>mRecordsHolders;
	std::list<ITypesHolderPtr>  mTypesHolders;
	std::list<IDefinesHolderPtr>mDefinesHolders;
	std::list<ScopePtr>         mScopes;
	std::list<CodePtr>          mCodes;
	bool												mDefineDecl = false;
	std::list<int>              mLoopWhileCounts;
	std::set<std::string>       mWantsUnary;
	std::set<std::string>       mLeftMonomial;
	mutable int	                mErrors = 0;
	mutable int				          mWarnings = 0;
	std::ostream*               mOut = nullptr;
	std::string                 mCachedOutput;
	bool                        mDataOutput = false;
	bool                        mFirstOutput = true;
	bool                        mStartOutput = true;
	std::string                 mStream;
	std::string                 mLastWriteStream;
	std::string                 mLastReadStream;
	std::map<std::string, std::string> mAliases;
	std::set<std::string>       mImports;
	FileType										mFileType = FileType::Normal;
	std::string                 mFileName;
	std::string                 mLibrary;
	Math                        mMath;
	yy::parser::location_type   mMathStart;

	struct IfIs
	{
		bool isSwitch = true;
		bool isFirst = true;
		bool isFirstTest = true;
		bool nextElse = false;
		bool nextCase = false;
		std::string idName;
		std::shared_ptr<std::ostringstream> out;
		std::ostream* savedOut = nullptr;
	};
	std::stack<IfIs> mIfIs;
	
	enum class MethodType
	{
		Code,
		Define,
		Value,
	};

  MethodType                  mMethodType = MethodType::Define;

	enum class SpecialWord
	{
		None					= 0x0,
		WantsUnary		= 0x01,
		IsAlias				= 0x02,
		LeftMonomial	= 0x04,
	};

	int32_t  			          	  mSpecialWord = static_cast<int32_t>(SpecialWord::None);
};


