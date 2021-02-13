#pragma once

#include "NateData.h"

#include <ostream>

class NateParser;

class NateCode
{
public:
  NateCode(std::ostream& aOut, NateParser* aParser);
  void code(const TreeNodePtr& aStat);
  void codeNested(const TreeNodePtr& aStat);
  void codeTreeDesc(const TreeNodePtr& aStat);

private:
  std::string in(int extra = 0);
  void printLineNr(const Location& aLocation);
	char end();
  void codeProgram(const TreeNodePtr& aNode);
	void codeData(const TreeNodePtr& aNode);
  void codeOutput(const std::string& aStream, const TreeNodePtr& aNode);
	void codeOutputNew();
	void codeOutput(const std::string& aString);
	void codeOutput(const Expr& aValue);
	void codeOutputEnd(bool aAddEnd = true);
	void codeInput(const std::string& aString, const TreeNodePtr& aNode);
	std::string codeExpr(const Expr& aValue);
	void codeDeclIdentifier(bool aExtern,
													const IdentifierPtr& aIdentifier,
													bool initializeVariables,
													const Location& aLocation);
	void codeAssign(const TreeNodePtr& aNode);
	void codeIfThen(const TreeNodePtr& aNode);
	void codeIf(const TreeNodePtr& aNode);
	void codeElseIf(const TreeNodePtr& aNode);
	void codeElse(const TreeNodePtr& aNode);
	void codeIfIs(const TreeNodePtr& aNode);
	bool codeCaseIsListIf(const TreeNodePtr& aNode, const TreeNodePtr& aIfIsNode, bool& firstIf);
	void codeCaseIsListSwitch(const TreeNodePtr& aNode);
	void codeSwitch(const TreeNodePtr& aNode, const TreeNodePtr& aElsePart);
	void codeCaseIsIf(const TreeNodePtr& aNode, const TreeNodePtr& aIfIsNode, bool firstCond);
	void codeCaseIsSwitch(const TreeNodePtr& aNode);
	void codeElseIs(const TreeNodePtr& aNode);
  void codeStartLoop(const TreeNodePtr& aNode);
  void codeStartLoopForStep(const TreeNodePtr& aNode);
  void codeStartLoopForRange(const TreeNodePtr& aNode);
  void codeWhile(const TreeNodePtr& aNode);
  void codeScope(const TreeNodePtr& aNode);
  void codeCodeInclude(const TreeNodePtr& aNode);
  void codeWrite(const TreeNodePtr& aNode);
  void codeRecord(const TreeNodePtr& aNode);
  void codeDefine(const TreeNodePtr& aNode);
	void createCodeDeclArgs(const DefinePtr& aDefine, const std::vector<Arg>& aArgs);
	void createCodeDecl(const DefinePtr& aDefine, const std::string& aObjectName);
  void codeReturn(const TreeNodePtr& aNode);


	bool isConstIntScalar(const Expr& aExpr);
	bool isNestedConstIntScalar(const TreeNodePtr& aNode);
	bool isNestedNonConstIntScalar(const TreeNodePtr& aNode);

	std::string codeDesc(const TreeNodePtr& aNode);

	NateParser*                 mParser = nullptr;
  std::ostream*								mOut;
  int													mIndent = 0;
	std::string                 mCachedOutput;
	bool                        mDataOutput = false;
	bool                        mFirstOutput = true;
	bool                        mStartOutput = true;
	std::string                 mStream;

	int													mPrevLine = 0;
	std::string									mPrevFile;
};

