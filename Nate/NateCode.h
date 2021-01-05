#pragma once

#include "NateData.h"

#include <ostream>

class NateParser;

class NateCode
{
public:
  NateCode(std::ostream& aOut, NateParser* aParser);
  void code(const std::shared_ptr<TreeNode>& aStat);

private:
  std::string in(int extra = 0);
  void printLineNr(const Location& aLocation);
	char end();
  void codeProgram(const std::shared_ptr<TreeNode>& aNode);
  void codeOutput(const std::string& aStream, const std::shared_ptr<TreeNode>& aNode);
	void codeOutputNew();
	void codeOutput(const std::string& aString);
	void codeOutput(const Expr& aValue);
	void codeOutputEnd(bool aAddEnd = true);
	std::string codeExpr(const Expr& aValue);
	void codeDeclIdentifier(bool aExtern,
													const IdentifierPtr& aIdentifier,
													bool initializeVariables,
													const Location& aLocation);
	void codeAssign(const std::shared_ptr<TreeNode>& aNode);
	void codeIf(const std::shared_ptr<TreeNode>& aNode);
	void codeElseIf(const std::shared_ptr<TreeNode>& aNode);
	void codeElse(const std::shared_ptr<TreeNode>& aNode);
	void codeEndIf(const std::shared_ptr<TreeNode>& aNode);
  void codeStartLoop(const std::shared_ptr<TreeNode>& aNode);
  void codeStartLoopForStep(const std::shared_ptr<TreeNode>& aNode);
  void codeStartLoopForRange(const std::shared_ptr<TreeNode>& aNode);
  void codeEndLoop(const std::shared_ptr<TreeNode>& aNode);
  void codeWhile(const std::shared_ptr<TreeNode>& aNode);

	NateParser*                 mParser = nullptr;
  std::ostream&								mOut;
  int													mIndent = 0;
	std::string                 mCachedOutput;
	bool                        mDataOutput = false;
	bool                        mFirstOutput = true;
	bool                        mStartOutput = true;
	std::string                 mStream;

	int													mPrevLine = 0;
	std::string									mPrevFile;
};

