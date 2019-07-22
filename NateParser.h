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

	void pushScope(const Scope& aScope);
	void popScope();
	Scope& currentScope();
	void error(const std::string& anError);
	int errorCount() const { return mErrors; }
	Identifier* getIdentifier(const std::string& aName, Scope* aScope = nullptr);
	Identifier& getOrFakeIdentifier(const std::string& aName, Scope* aScope = nullptr);
	void addIdentifier(const Identifier& aIdentifier);
	std::tuple<bool, std::string> makeIdOrWord(const std::string& aString);

	void codeStartProgram();
	void codeEndProgram();
	void addCode();
	void endCode();
	Code& curCode();
	void addDefine();
	void declareDefine();
	void endDefine();
	Define& curDefine();
	Method& curWithArgs();
	void codeDeclareLocalIdentifier(const Identifier& aIdentifier);
	void codeDeclareLocalIdentifiers(const std::vector<std::string>& aNames,
							         const std::string& aType,
							         const std::vector<Expr>& aInitValues);
	void codeAssign(const std::vector<std::string>& aNames, Expr& aValue);
	std::string codeId(const std::string& aName, Scope* aScope = nullptr);
	void codeOutputStart(const std::string& aStream);
	void codeOutput(const std::string& aString);
	void codeOutput(const Expr& aValue);
	void codeOutputEnd(bool aAddEnd = true);
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
  Expr evaluate(const Expr& aExpr);
	void printLineNr();

private:
	struct Match
	{
		const Method*  methodFound;
		ExprNodesCIter nodeStartIter;
		ExprNodesCIter nodeEndIter;
		const Method*  matchedMethod;
		std::string    matchedErrorMsg;
	};

	std::string pattern(const ExprNode& aNode) const;
	std::string pattern(std::vector<ExprNode>::const_iterator& aBegin,
  std::vector<ExprNode>::const_iterator& aEnd) const;
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

	std::unique_ptr<yy::Lexer>	mLexer;
	std::unique_ptr<yy::parser>	mParser;
	std::list<Scope>            mScopes;
	std::list<Code>             mCodes;
	std::list<Define>           mDefines;
	std::list<int>              mLoopWhileCounts;
	int				                  mErrors = 0;
	std::ostream&               mOut;
	std::string                 mCachedOutput;
  bool                        mInCode = false;
};


