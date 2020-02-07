#pragma once

#include <string>
#include <map>
#include <list>
#include <stack>
#include <set>
#include <limits>
#include <fstream>
#include <memory>

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
		bool outputEnd = false;
		std::string prevWriteSink;
		bool inputEnd = true;
		bool prevWasValue = false;
		std::stack<Expr> ifExpr;
		std::stack<std::string> ifId;
		std::string forId;
		std::list<Code*> curParsedCodes;
		bool inObject = false;
		bool objectMe = false;
		std::stack<RecordPtr> curRecord;
		bool assignExpr = false;
		IdentifierPtr propId;
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

	void pushScope(const std::string& aName);
	void pushScope(const ScopePtr& aScope);
	void popScope();
	void pushDefineScope(const std::string& aName);
	void pushDefineScope(const ScopePtr& aScope);
	void popDefineScope();
	IIdentifiersHolderPtr& curIdentifiersHolder();
	IRecordsHolderPtr& curRecordsHolder();
	ITypesHolderPtr& curTypesHolder();
	IDefinesHolderPtr& curDefinesHolder();

	void error(const std::string& anError);
	void warning(const std::string& aWarning);
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
	void startObject(const ObjectPtr& aObject);
	ObjectPtr curObject();
	void setCurObject(const ObjectPtr& aObject);
	ObjectPtr getObject(const std::string& aId);
	void declareProperties(bool aReadonly,
											   const std::vector<std::string>& aNames,
												 const TypePtr& aType);
	void defineProp(const IdentifierPtr& aIdentifier, Object::PropType aPropType);
	void endDefineProp(const IdentifierPtr& aIdentifier, Object::PropType aPropType);

	void addCode();
	void endCode();
	Code& curCode();

	void addDefine(bool aInObject);
	void declareDefine(bool aIsDecl = false);
	void endDefine();
	void deleteCurDefine();
	Define* curDefine();

	Method& curMethod();
	void addArgWord(const std::string& aWord);
	void addArgId(const std::string& aId, const TypePtr& aType, const std::string& inOut);

	void codeStartProgram(const yy::parser::location_type& aLocation);
	void codeEndProgram(const yy::parser::location_type& aLocation);
	void codeStartScope();
	void codeEndScope();
	void codeCodeInclude();
	void codeDeclareLocalIdentifier(bool aExtern,
																	const IdentifierPtr& aIdentifier,
																	bool initializeVariables,
																	const yy::parser::location_type& aLocation = yy::parser::location_type(yy::position(), yy::position()));
	void codeDeclareLocalIdentifiers(bool aIsConst,
																	 const std::vector<std::string>& aNames,
																	 const TypePtr& aType,
																	 const std::vector<Expr>& aInitValues,
																	 bool initializeVariables);
	void codeStartRecord(const RecordPtr& aRecord,
											 const yy::parser::location_type& aLocation);
	void codeDeclareRecordIdentifiers(bool aIsConst,
																	  const std::vector<std::string>& aNames,
																  	const TypePtr& aType,
																	  const std::vector<Expr>& aInitValues);
	void codeEndRecord(const yy::parser::location_type& aLocation);
	void codeStartDeclObject();
	void codeEndDeclObject();
	void codeStartImplObject();
	void codeEndImplObject();
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
	void codeInputStart(const std::string& aStream, const yy::parser::location_type& aLocation);
	void codeInputSpace();
	void codeInputNoSpace();
	void codeInput(const Expr& aValue);
	void codeInputEnd(bool aAddEnd = true);
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
  void codeExpression(const Expr& aExpr, const yy::parser::location_type& aLocation);
  Expr evaluate(const Expr& aExpr, bool aDebug = false);
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

	struct Match
	{
		const Method*				methodFound = nullptr;
		ExprNodesCIter			nodeStartIter;
		ExprNodesCIter			nodeEndIter;
		const Method*				matchedMethod = nullptr;
		Method::MatchResult matchResult;
	};

	std::string handleCompileCommand(const std::string& aCommand, const std::string& aData);
	void handleCompileCommands(Expr& aExpr);
	void checkIfBetterMatch(const Method& aMethod,
													const ExprNodesCIter& aStartIter,
													const ExprNodesCIter& aEndIter,
													Match& aMatch,
													bool aLeftToRight,
													bool aDebug);
  void checkLeftToRightMethod(const Method& aMethod,
	                            const Expr& aExpr,
	                            Match& aMatch,
															bool aDebug = false);
  void checkRightToLeftMethod(const Method& aMethod,
	                            const Expr& aExpr,
	                            Match& aMatch,
															bool aDebug = false);
	void checkIfMethod(const Method& aMethod, const Expr& aExpr, Match& aMatch,
										 bool aDebug = false);
	void unput(const std::string::const_iterator& aStart,
						 const std::string::const_iterator& aEnd);
	void codeOutputNew();
	std::string makeTempDir();
	bool importObjectDefinition(const std::string& aLibrary, const std::string& aName);
	std::string typeScopeName() const;

	std::unique_ptr<yy::Lexer>	mLexer;
	std::unique_ptr<yy::parser>	mParser;
	std::list<ObjectPtr>        mObjects;
	ObjectPtr                   mCurObject;
	Define*                     mCurDefine = nullptr;
	std::ostream*               mSavedOut = nullptr;
	std::list<IIdentifiersHolderPtr> mIdentifiersHolders;
	std::list<IRecordsHolderPtr>mRecordsHolders;
	std::list<ITypesHolderPtr>  mTypesHolders;
	std::list<IDefinesHolderPtr>mDefinesHolders;
	std::list<ScopePtr>         mScopes;
	std::list<Code>             mCodes;
	bool												mDefineDecl = false;
	std::list<int>              mLoopWhileCounts;
	std::set<std::string>       mWantsUnary;
	std::set<std::string>       mLeftMonomial;
	int				                  mErrors = 0;
	int				                  mWarnings = 0;
	std::ostream*               mOut = nullptr;
	std::string                 mCachedOutput;
	bool                        mFirstOutput = true;
	std::string                 mStream;
	std::string                 mLastWriteStream;
	std::map<std::string, std::string> mAliases;
	std::set<std::string>       mImports;
	FileType										mFileType = FileType::Normal;
	std::string                 mFileName;
	std::string                 mLibrary;

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


