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
	enum class FileType
	{
		Normal,
		ObjectDecl,
		ObjectImpl
	};
	
	NateParser(const std::string& aFilename, std::istream& aIn, std::ostream& aOut,
						 FileType aFileType = FileType::Normal);
	virtual ~NateParser();
	std::string in(int aOffset = 0) const;
	int parse();
	void import(const std::string& aName);
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
	ScopePtr& curScope();
	IRecordHolderPtr& curRecordHolder();
	ITypeHolderPtr& curTypeHolder();
	IDefineHolderPtr& curDefineHolder();

	void error(const std::string& anError);
	void warning(const std::string& aWarning);
	int errorCount() const { return mErrors; }
	int warningCount() const { return mWarnings; }
	void addType(const TypePtr& aType, const std::string& aName = "");
	TypePtr getType(const std::string& aName, ITypeHolder* aTypeHolder = nullptr);
	TypePtr determineType(const std::string& aName);
	TypePtr makeType(const std::string& aValue);
	IdentifierPtr getIdentifier(const std::string& aName, Scope* aScope = nullptr);
	IdentifierPtr getOrFakeIdentifier(const std::string& aName, Scope* aScope = nullptr);
	void addIdentifier(const IdentifierPtr& aIdentifier);
	std::tuple<bool, std::string> makeIdOrWord(const std::string& aOrig, const std::string& aString);
	std::string uniqueName() const;
	TypePtr getNumberType(std::string& aString);
	
	void addObject(const ObjectPtr& aObject);
	void endObject();
	ObjectPtr curObject();
	void setCurObject(const ObjectPtr& aObject);
	ObjectPtr getObject(const std::string& aId);

	void addCode();
	void endCode();
	Code& curCode();

	void addDefine(bool aInObject);
	void declareDefine(bool aIsDecl = false);
	void endDefine();
	Define& curDefine();

	Method& curMethod();
	void addArgWord(const std::string& aWord);

	void codeStartProgram();
	void codeEndProgram();
	void codeStartScope();
	void codeEndScope();
	void codeCodeInclude();
	void codeDeclareLocalIdentifier(bool aExtern,
																	const IdentifierPtr& aIdentifier,
																	bool initializeObjects = false);
	void codeDeclareLocalIdentifiers(bool aConst,
																	 const std::vector<std::string>& aNames,
																	 const TypePtr& aType,
																	 const std::vector<Expr>& aInitValues,
																	 bool initializeObjects = false);
	void codeStartRecord(const RecordPtr& aRecord);
	void codeDeclareRecordIdentifiers(bool aConst,
																	  const std::vector<std::string>& aNames,
																  	const TypePtr& aType,
																	  const std::vector<Expr>& aInitValues);
	void codeEndRecord();
	void codeStartDeclObject(const ObjectPtr& aObject);
	void codeEndDeclObject();
	void codeStartImplObject(const ObjectPtr& aObject, bool aExistingDecl);
	void codeEndImplObject(bool aExistingDecl);
	void codeAssign(const std::vector<Expr>& aExpressions, Expr& aValue);
	std::string codeId(const std::string& aName, Scope* aScope = nullptr);
	void codeOutputStart(const std::string& aStream);
	void codeOutput(const std::string& aString);
	void codeOutput(const Expr& aValue);
	void codeOutputEnd(bool aAddEnd = true);
	void codeWriteStart(const Expr& aValue);
	void codeInputStart(const std::string& aStream);
	void codeInputSpace();
	void codeInputNoSpace();
	void codeInput(const Expr& aValue);
	void codeInputEnd(bool aAddEnd = true);
	void codeIf(const Expr& aValue);
	void codeElseIf();
	void codeElse();
	void codeEndIf();
	void codeIfIs(const Expr& aValue, const std::string& idName);
	void codeIs(const Expr& aValue, const Expr& aIfExpr);
	void codeElseIs();
	void codeBeginIs();
	void codeEndIs();
	void codeEndIfIs();
  void codeInitLoop();
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
  void codeLoopWhile(const Expr& aExpr);
  void codeReturn(const Expr& aValue);
  void codeExpression(const Expr& aExpr);
  Expr evaluate(const Expr& aExpr, bool aDebug = false);
	void printLineNr();

private:
	void pushRecordHolder(const IRecordHolderPtr& aRecordHolder);
	void popRecordHolder();
	void pushTypeHolder(const ITypeHolderPtr& aTypeHolder);
	void popTypeHolder();
	void pushDefineHolder(const IDefineHolderPtr& aDefineHolder);
	void popDefineHolder();

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

	std::unique_ptr<yy::Lexer>	mLexer;
	std::unique_ptr<yy::parser>	mParser;
	std::list<ObjectPtr>        mObjects;
	ObjectPtr                   mCurObject;
	std::ostream*               mIncludeOut = nullptr;
	std::ostream*               mSavedOut = nullptr;
	std::list<IRecordHolderPtr> mRecordHolders;
	std::list<ITypeHolderPtr>   mTypeHolders;
	std::list<IDefineHolderPtr> mDefineHolders;
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


