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

namespace yy
{
	class Lexer;
	class parser;
};

class ExprNode;

class NateParser
{
public:
	NateParser(const std::string& aFilename, std::istream& aIn, std::ostream& aOut);
	virtual ~NateParser();
	std::string in(int aOffset = 0) const;
	int parse();
	void import(const std::string& aName);
	void addAlias(const std::string& aName, const std::string& aValue);
	std::string alias(const std::string& aString);;
	void addLeftMonomial(const std::string& aWord);
	bool isLeftMonomial(const std::string& aWord) const;
	void addWantsUnary(const std::string& aWord);
	bool wantsUnary(const std::string& aWord) const;

	void pushScope(const std::string& aName);
	void pushScope(const ScopePtr& aScope);
	void popScope();
	ScopePtr& curScope();
	void error(const std::string& anError);
	int errorCount() const { return mErrors; }
	void addType(const TypePtr& aType, const std::string& aName = "");
	TypePtr getType(const std::string& aName, Scope* aScope = nullptr);
	TypePtr determineType(const std::string& aName);
	TypePtr makeType(const std::string& aValue);
	IdentifierPtr getIdentifier(const std::string& aName, Scope* aScope = nullptr);
	IdentifierPtr getOrFakeIdentifier(const std::string& aName, Scope* aScope = nullptr);
	void addIdentifier(const IdentifierPtr& aIdentifier);
	std::tuple<bool, std::string> makeIdOrWord(const std::string& aOrig, const std::string& aString);
	std::string uniqueName() const;

	void addCode();
	void endCode();
	Code& curCode();
	void addDefine();
	void declareDefine();
	void endDefine();
	Define& curDefine();
	Method& curMethod();
	void addArgWord(const std::string& aWord);

	void codeStartProgram();
	void codeEndProgram();
	void codeStartScope();
	void codeEndScope();
	void codeDeclareLocalIdentifier(const IdentifierPtr& aIdentifier,
																	bool initializeNonScalars = false);
	void codeDeclareLocalIdentifiers(bool aConst,
																	 const std::vector<std::string>& aNames,
																	 const TypePtr& aType,
																	 const std::vector<Expr>& aInitValues,
																	 bool initializeNonScalars = false);
	void codeStartRecord(const RecordPtr& aRecord);
	void codeDeclareRecordIdentifiers(bool aConst,
																	  const std::vector<std::string>& aNames,
																  	const TypePtr& aType,
																	  const std::vector<Expr>& aInitValues);
	void codeEndRecord();
	void codeAssign(const std::vector<Expr>& aExpressions, Expr& aValue);
	std::string codeId(const std::string& aName, Scope* aScope = nullptr);
	void codeOutputStart(const std::string& aStream);
	void codeOutput(const std::string& aString);
	void codeOutput(const Expr& aValue);
	void codeOutputEnd(bool aAddEnd = true);
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
  void codeStartForLoop(const std::string& aId, 
                        const TypePtr& aType, 
                        bool aDownTo,
                        const Expr& aStart,
                        const Expr& aEnd,
                        const Expr& aStep);
  void codeEndLoop();
  void codeLoopWhile(const Expr& aExpr);
  void codeReturn(const Expr& aValue);
  void codeExpression(const Expr& aExpr);
  Expr evaluate(const Expr& aExpr);
	void printLineNr();

private:
	struct Match
	{
		const Method*  methodFound = nullptr;
		ExprNodesCIter nodeStartIter;
		ExprNodesCIter nodeEndIter;
		const Method*  matchedMethod = nullptr;
		std::string    matchedErrorMsg;
	};
	
	void methodMatches(const Method& aMethod,
		                 ExprNodesCIter& aStartIter, 
		                 ExprNodesCIter& aEndIter,
		                 Match& aMatch);
  void checkLeftToRightMethod(const Method& aMethod,
	                            const Expr& aExpr,
	                            Match& aMatch);
  void checkRightToLeftMethod(const Method& aMethod,
	                            const Expr& aExpr,
	                            Match& aMatch);
	void checkIfMethod(const Method& aMethod, const Expr& aExpr, Match& aMatch);
	void unput(const std::string::const_iterator& aStart,
						 const std::string::const_iterator& aEnd);

	std::unique_ptr<yy::Lexer>	mLexer;
	std::unique_ptr<yy::parser>	mParser;
	std::list<ScopePtr>         mScopes;
	std::list<Code>             mCodes;
	std::list<Define>           mDefines;
	std::list<int>              mLoopWhileCounts;
	std::set<std::string>       mWantsUnary;
	std::set<std::string>       mLeftMonomial;
	int				                  mErrors = 0;
	std::ostream*               mOut;
	std::string                 mCachedOutput;
	std::string                 mStream;
	std::map<std::string, std::string> mAliases;
	std::set<std::string>       mImports;

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


