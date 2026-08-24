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

#include "NateData.h"
#include "NateFunctions.h"
#include "MathParser.h"
#include "NateParser.tab.h"
#include "Code.h"
#include "Property.h"
#include "Identifier.h"
#include "Scope.h"
#include "Record.h"
#include "Object.h"
#include "Stat.h"


namespace nate
{
class Expr;
class Lexer;
class parser;
class NateCode;

class NateParser
{
public:
	static const bool InitializeVariables = true;
	

	struct ParseData
	{
		std::string prevReadSource;
		bool prevWasValue = false;
		std::stack<Expr> ifExpr;
		std::stack<nate::parser::location_type> ifLocation;
		std::string forId;
		std::list<CodePtr> curParsedCodes;
		bool inObject = false;
		bool inObjectImpl = false;
		bool objectMe = false;
		std::stack<RecordPtr> curRecord;
		bool assignExpr = false;
		IdentifierPtr propId;
		WithFlags* flagsHolder = nullptr;
		ObjectPtr object;
		std::string inBracketsCode;
	};


	enum class FileType
	{
		Normal,
		ObjectDecl,
		ObjectImpl
	};
	
	ParseData data;

	NateParser(NateCode& aCoder, const std::string& aFilename, std::istream& aIn, std::ostream& aOut,
			   FileType aFileType = FileType::Normal);
	virtual ~NateParser();
	std::string in(int aOffset = 0) const;
	int parseAndCode();
	int parse();
	int code();
    const Stat::List& getStats() const;
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
	void popStatsHolder();
	
	nate::Lexer* getLexer();

	void startProgram(const nate::parser::location_type& aLocation);
	void endProgram(const nate::parser::location_type& aLocation);
	void doAssign(const std::vector<Expr>& aExpressions, const Expr& aValue, const nate::parser::location_type& aLocation);
	void doIf(const Expr& aValue, const nate::parser::location_type& aLocation);
	void doElseIf(const Expr& aValue, const nate::parser::location_type& aLocation);
	void doElse(const nate::parser::location_type& aLocation);
	void doEndIf(const nate::parser::location_type& aLocation);
	void doIfIs(const Expr& aValue, const nate::parser::location_type& aLocation);
	void doCaseIsList(const nate::parser::location_type& aLocation);
	void doCaseIs(const Expr& aValue, const Expr& aIfExpr, const nate::parser::location_type& aLocation);
	void doElseIs(const nate::parser::location_type& aLocation);
	void doEndIs(const nate::parser::location_type& aLocation);
	void doInitLoop(const nate::parser::location_type& aLocation);
	void doStartLoop(const nate::parser::location_type& aLocation);
	void doWhile(const Expr& aValue, const nate::parser::location_type& aLocation);
    void doStartLoopForStep(const std::string& aId, 
							const TypePtr& aType, 
							bool aDownTo,
							const Expr& aStart,
							const Expr& aEnd,
							const Expr& aStep, 
							const nate::parser::location_type& aLocation);
    void doStartLoopForRange(const std::string& aId, 
							const Expr& aRange, 
						    const nate::parser::location_type& aLocation);
	void doEndLoop(const nate::parser::location_type& aLocation);
	void doData(const std::string& aId, const nate::parser::location_type& aLocation);
	void doDataEnd();
	void doWrite(const Expr& aValue, const nate::parser::location_type& aLocation);
	void doOutputStart(const nate::parser::location_type& aLocation);
	void doOutputComma(const nate::parser::location_type& aLocation);
	void doOutputConcat(const nate::parser::location_type& aLocation);
	void doOutputExpr(const Expr& aValue, const nate::parser::location_type& aLocation);
	void doOutputEnd(bool aEnd, const nate::parser::location_type& aLocation, bool aPopStatsHolder = true);
	void doError(const nate::parser::location_type& aLocation);
	void doStartRecord(const std::string& anId, const nate::parser::location_type& aLocation);
	void doEndRecord(const nate::parser::location_type& aLocation);
	void doStartDefine(bool aIsDecl, bool aIsImpl,
										 const nate::parser::location_type& aLocation);
	void doEndDeclDefine(const nate::parser::location_type& aLocation);
	void doEndDefine(const nate::parser::location_type& aLocation);
    void doReturn(const Expr& aValue, const nate::parser::location_type& aLocation);
	void doRead(InputType aInputType, const Expr& aValue, const nate::parser::location_type& aLocation);
	void doInput(const nate::parser::location_type& aLocation);
	void doInputComma(const nate::parser::location_type& aLocation);
	void doInputConcat(const nate::parser::location_type& aLocation);
	void doInputExpr(const Expr& aValue, const nate::parser::location_type& aLocation);
	void doInputEnd(bool aEnd, const nate::parser::location_type& aLocation);	
	void doStartScope(const nate::parser::location_type& aLocation);
	void doEndScope(const nate::parser::location_type& aLocation);
	void doCodeInclude(const nate::parser::location_type& aLocation);	
	void doDeclObject(const nate::parser::location_type& aLocation);
	void doEndDeclObject(const nate::parser::location_type& aLocation);
	void doImplObject(const nate::parser::location_type& aLocation);
	void doEndImplObject();
	IdentifierPtr doProp(const std::string& aName, 
						 const TypePtr& optType, const std::vector<std::string>& flags,
						 const nate::parser::location_type& aLocation);
	void doEndProp(const nate::parser::location_type& aLocation);
	void doPropDefine(const IdentifierPtr& aIdentifier, Property::PropType aPropType,
					  const nate::parser::location_type& aLocation);
	void doEndPropDefine(const nate::parser::location_type& aLocation);
    void doExpressionStatement(const Expr& aExpr, const nate::parser::location_type& aLocation);


	void pushScope(const ScopePtr& aScope);
	void popScope();
	void pushDefineScope(const ScopePtr& aScope);
	void popDefineScope();
	IIdentifiersHolderPtr& curIdentifiersHolder();
	IRecordsHolderPtr& curRecordsHolder();
	ITypesHolderPtr& curTypesHolder();
	IDefinesHolderPtr& curDefinesHolder();
	
	void error(const nate::position& aPosition, const std::string& anError) const;
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
    bool checkProperty(std::ostringstream& error, const ExprNodesCIter& nodeIter) const;
	IdentifierPtr getIdentifier(const std::string& aName, IIdentifiersHolder* aIdentifiersHolder = nullptr);
	IdentifierPtr getOrFakeIdentifier(const std::string& aName, IIdentifiersHolder* aIdentifiersHolder = nullptr);
	void addIdentifier(const IdentifierPtr& aIdentifier);
	std::tuple<bool, std::string> makeIdOrWord(const std::string& aOrig, const std::string& aString);
	std::string uniqueName() const;
	TypePtr getNumberType(std::string& aString);
	
	void addObject(const ObjectPtr& aObject);
	void endObject();
	void addUndeclaredProperties(const ObjectPtr& aObject, const nate::parser::location_type& aLocation);
	void addUndeclaredDefines(const ObjectPtr& aObject, const nate::parser::location_type& aLocation);
	void endImplementObject();
	void startObject(const ObjectPtr& aObject);
	ObjectPtr curObject() const;
	void checkObject(const ObjectPtr& aObject);
	void setCurObject(const ObjectPtr& aObject);
	ObjectPtr getObject(const std::string& aId);
	void addObjectBase(ObjectPtr& aCurObject, const ObjectPtr& aObject);
	void addObjectRole(ObjectPtr& aCurObject, const ObjectPtr& aObject);
	void declareProperties(const std::vector<std::string>& aNames,
						   const TypePtr& aType,
						   const std::vector<std::string>& flags,
						   const nate::parser::location_type& aLocation);
	
	void startInbrackets(const nate::parser::location_type& aLocation, const std::string& type);
	void endInbrackets();
	void addInbracketsStatWord(const std::string& aWord,
							   const nate::parser::location_type& aLocation);
	void addCode();
	void endCode();
	CodePtr curCode();
	CodePtr getCode(const CodePtr& aCode);

	void addDefine(bool aInObject);
	void addGivenDefine(const DefinePtr& aDefine, bool aInObject);
	void deleteCurDefine();
	DefinePtr curDefine() const;

	MethodPtr curMethod();
	void addArgWord(const std::string& aWord);
	void addArgId(const std::string& aId, const TypePtr& aType, const std::string& inOut);

	void declareLocalIdentifiers(
					bool aIsConst,
					const std::vector<std::string>& aNames,
					const TypePtr& aType,
					const std::vector<Expr>& aInitValues,
					bool initializeVariables,
					const nate::parser::location_type& aLocation);
	void declareRecordIdentifiers(
					bool aIsConst,
					const std::vector<std::string>& aNames,
					const TypePtr& aType,
					const std::vector<Expr>& aInitValues,
					const nate::parser::location_type& aLocation);
	
	std::string codeId(const std::string& aName, Scope* aScope = nullptr);
    Expr evaluate(const Expr& aExpr, int aDebug = 0);

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
	void pushStatsHolder(const Stat::SPtr& aStatsHolder);
	

	struct Match
	{
		MethodPtr	      		methodFound;
		ExprNodesCIter			nodeStartIter;
		ExprNodesCIter			nodeEndIter;
		MethodPtr	      		matchedMethod;
		Method::MatchResult     matchResult;
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
	std::string makeTempDir();
	bool importObjectDefinition(const std::string& aLibrary, const std::string& aName);
	std::string typeScopeName() const;

	Location location(const nate::parser::location_type& aLocation);

	Stat::SPtr addStatement(const Stat::SPtr& stat);
		
	
    NateCode&                       mCoder;
	std::unique_ptr<nate::Lexer>	mLexer;
	std::unique_ptr<nate::parser>	mParser;
	std::list<ObjectPtr>            mObjects;
	ObjectPtr                       mCurObject;
	DefinePtr                       mCurDefine;
	std::ostream*                   mSavedOut = nullptr;
	std::list<IIdentifiersHolderPtr> mIdentifiersHolders;
	std::list<IRecordsHolderPtr>    mRecordsHolders;
	std::list<ITypesHolderPtr>      mTypesHolders;
	std::list<IDefinesHolderPtr>    mDefinesHolders;
	std::list<ScopePtr>             mScopes;
	std::list<ScopePtr>             mOldScopes;
	std::list<CodePtr>              mCodes;
	Stat::List                      mStats;
	std::list<Stat::SPtr>           mStatHolders;                 
	bool						    mDefineDecl = false;
	std::list<int>                  mLoopWhileCounts;
	std::set<std::string>           mWantsUnary;
	std::set<std::string>           mLeftMonomial;
	mutable int	                    mErrors = 0;
	mutable int				        mWarnings = 0;
	std::ostream*                   mOut = nullptr;
	std::string                     mCachedOutput;
	bool                            mDataOutput = false;
	std::string                     mStream;
	std::string                     mLastReadStream;
	std::map<std::string, std::string> mAliases;
	std::set<std::string>           mImports;
	FileType					    mFileType = FileType::Normal;
	std::string                     mFileName;
	std::string                     mLibrary;
	std::unique_ptr<MathParser>     mMathParser;
	Math						    mMath;
	std::string					    mInBracketsType;
	nate::parser::location_type     mMathStart;
	nate::parser::location_type     mDummyLocation;

	struct IfIs
	{
		bool isSwitch = true;
		IdentifierPtr id;
	};
	std::stack<IfIs> mIfIs;
	
	enum class MethodType
	{
		Code,
		Define,
		Value,
	};

    MethodType mMethodType = MethodType::Define;

	enum class SpecialWord
	{
		None		    = 0x0,
		WantsUnary		= 0x01,
		IsAlias		    = 0x02,
		LeftMonomial	= 0x04,
	};

	int32_t mSpecialWord = static_cast<int32_t>(SpecialWord::None);
};
}
