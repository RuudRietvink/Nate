#pragma once

#include "NateData.h"
#include "Object.h"
#include "ICodeVisitor.h"

#include <ostream>

class NateParser;

class NateCode : public ICodeVisitor
{
public:
  NateCode(std::ostream& aOut, NateParser* aParser);
  void codeStats(const std::list<Stat::SPtr>& aStats);
  void codeNested(const TreeNodePtr& aStat);
  void codeCompound(const Stat& aStat);
  void codeTreeDesc(const TreeNodePtr& aStat, std::ostream& out = std::cout);
  /// <summary>
  /// 
  /// </summary>
  /// <param name="aStat"></param>
  void visit(const StatProgram& aStat) override;
  void visit(const StatDeclareLocal& aStat) override;
  void visit(const StatAssign& aStat) override;
  void visit(const StatIf& aStat) override;
  void visit(const StatElseIf& aStat) override;
  void visit(const StatElse& aStat) override;
  void visit(const StatExpr& aStat) override;
  void visit(const StatOutput& aStat) override;
  void visit(const StatOutputComma& aStat) override;
  void visit(const StatOutputConcat& aStat) override;
  void visit(const StatOutputEnd& aStat) override;
  void visit(const StatOutputExpr& aStat) override;

private:
  std::string in(int extra = 0);
  void printLineNr(const Location& aLocation);
	char end();
	void codeData(const TreeNodePtr& aNode);
	void codeOutputNew();
	void codeOutput(const std::string& aString);
	void codeInput(const std::string& aString, const TreeNodePtr& aNode);
	std::string codeExpr(const Expr& aValue);
	void codeLocalVar(const TreeNodePtr& aNode, bool inImplObject = false);
	void codeDeclIdentifier(const TreeNodePtr& aNode, 
													bool aExtern,
													const IdentifierPtr& aIdentifier,
													bool initializeVariables,
													const Location& aLocation);
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
  void codeRead(const TreeNodePtr& aNode);
  void codeRecord(const TreeNodePtr& aNode);
  void codeDefine(const TreeNodePtr& aNode);
	std::string createCodeDeclArgs(const DefinePtr& aDefine, const std::vector<Arg>& aArgs);
	std::string createCodeDecl(const DefinePtr& aDefine, const std::string& aObjectName = "");
  void codeReturn(const TreeNodePtr& aNode);
  void codeDeclObject(const TreeNodePtr& aNode);
	void codeObjectBases(const ObjectPtr& aObject);
	void codeDeclObjectDefine(const TreeNodePtr& aNode);
  void codeImplObject(const TreeNodePtr& aNode);
  void codeProp(const TreeNodePtr& aNode);
	std::string codePropHeader(const ObjectPtr& aObject,
														 bool aAddObjectName,
												 	 	 const IdentifierPtr& aId, 
													 	 Object::PropType aPropType);
	void codeDefaultPropertyImpl(const ObjectPtr& aObject,
															 const IdentifierPtr& propId);
	void codeDefaultProperties(const ObjectPtr& aObject);
	void codeDeclProperties(const ObjectPtr& aObject);
	void codeDeclareProperty(const ObjectPtr& aObject,
												 	 const IdentifierPtr& aId,
													 const Location& aLocation);
	void codeExprStat(const TreeNodePtr& aNode);
	void codeImplObjectVariables(const TreeNodePtr& aNode);
	void codeImplObjectNested(const TreeNodePtr& aNode, bool inImpl);


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

