#pragma once

#include <string>
#include <map>
#include <list>
#include <limits>
#include <fstream>
#include <memory>

#include "NateFunctions.h"
#include "Code.h"
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
	void codeDeclareLocalIdentifier(const Identifier& aIdentifier);
	void codeDeclareLocalIdentifiers(const std::vector<std::string>& aNames,
							         const std::string& aType,
							         const std::vector<Expr>& aInitValues);
	void codeAssign(const std::vector<std::string>& aNames, Expr& aValue);
	std::string codeId(const std::string& aName, Scope* aScope = nullptr);
	void codeOutputStart(const std::string& aStream);
	void codeOutput(const std::string& aString);
	void codeOutput(const Expr& aValue);
	void codeOutputEnd();
	void codeIf(const Expr& aValue);
	void codeElseIf();
	void codeElse();
	void codeEndIf();
  void codeStartLoop();
  void codeStartForLoop(const std::string& aId, 
                        const std::string& aType, 
                        bool aDownTo,
                        const Expr& aStart,
                        const Expr& aEnd,
                        const Expr& aStep);
  void codeEndLoop();
  void codeLoopWhile(const Expr& aExpr);
  Expr evaluate(const Expr& aExpr);
	void printLineNr();

private:
	struct Match
	{
		const Code*    codeFound;
		ExprNodesCIter codeStartIter;
		ExprNodesCIter codeEndIter;
		const Code*    matchedCode;
		std::string    matchedErrorMsg;
	};

	std::string pattern(const ExprNode& aNode) const;
	std::string pattern(std::vector<ExprNode>::const_iterator& aBegin,
  std::vector<ExprNode>::const_iterator& aEnd) const;
	void codeMatches(const Code& aCode,
		               ExprNodesCIter& aStartIter, 
		               ExprNodesCIter& aEndIter,
		               Match& aMatch);
  void checkLeftToRightCodes(const Code& aCode,
	                           const Expr& aExpr,
	                           Match& aMatch);
  void checkRightToLeftCodes(const Code& aCode,
	                           const Expr& aExpr,
	                           Match& aMatch);

	std::unique_ptr<yy::Lexer>	mLexer;
	std::unique_ptr<yy::parser>	mParser;
	std::list<Scope>            mScopes;
	std::list<Code>             mCodes;
	std::list<int>              mLoopWhileCounts;
	int				                  mErrors = 0;
	std::ostream&               mOut;
	std::string                 mCachedOutput;
};


