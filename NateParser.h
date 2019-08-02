#pragma once

#include <string>
#include <map>
#include <list>
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
	int parse();
	void import(const std::string& aName);
	std::string alias(const std::string& aString);
	bool isLeftMonomial(const std::string& aWord) const;

	void pushScope(const ScopePtr& aScope);
	void popScope();
	ScopePtr& curScope();
	void error(const std::string& anError);
	int errorCount() const { return mErrors; }
	TypePtr getType(const std::string& aName, Scope* aScope = nullptr);
	TypePtr determineType(const std::string& aName);
	IdentifierPtr getIdentifier(const std::string& aName, Scope* aScope = nullptr);
	IdentifierPtr getOrFakeIdentifier(const std::string& aName, Scope* aScope = nullptr);
	void addIdentifier(const IdentifierPtr& aIdentifier);
	std::tuple<bool, std::string> makeIdOrWord(const std::string& aOrig, const std::string& aString);

	void addCode();
	void endCode();
	Code& curCode();
	void addDefine();
	void declareDefine();
	void endDefine();
	Define& curDefine();
	Method& curMethod();

	void codeStartProgram();
	void codeEndProgram();
	void codeStartScope();
	void codeEndScope();
	void codeDeclareLocalIdentifier(const IdentifierPtr& aIdentifier,
																	bool initializeNonScalars = true);
	void codeDeclareLocalIdentifiers(bool aConst,
																	 const std::vector<std::string>& aNames,
																	 const std::string& aType,
																	 const std::vector<Expr>& aInitValues,
																	 bool initializeNonScalars = true);
	void codeStartRecord(const RecordPtr& aRecord);
	void codeDeclareRecordIdentifiers(bool aConst,
																	  const std::vector<std::string>& aNames,
																  	const std::string& aType,
																	  const std::vector<Expr>& aInitValues);
	void codeEndRecord();
	void codeAssign(const std::vector<Expr>& aExpressions, Expr& aValue);
	std::string codeId(const std::string& aName, Scope* aScope = nullptr);
	void codeOutputStart(const std::string& aStream);
	void codeOutput(const std::string& aString);
	void codeOutput(const Expr& aValue, const Expr& aDesc);
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
  void codeInitLoop();
  void codeStartLoop();
  void codeStartForLoop(const std::string& aId, 
                        const std::string& aType, 
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
	void initAliases();

	std::unique_ptr<yy::Lexer>	mLexer;
	std::unique_ptr<yy::parser>	mParser;
	std::list<ScopePtr>         mScopes;
	std::list<Code>             mCodes;
	std::list<Define>           mDefines;
	std::list<int>              mLoopWhileCounts;
	int				                  mErrors = 0;
	std::ostream&               mOut;
	std::string                 mCachedOutput;
	std::string                 mStream;
	std::map<std::string, std::string> 
															mAliases;

	enum class MethodType
	{
		Code,
		Define,
		Value,
	};

  MethodType                  mMethodType = MethodType::Define;
};


