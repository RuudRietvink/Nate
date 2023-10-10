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
  /// <summary>
  /// 
  /// </summary>
  /// <param name="aStat"></param>
  void visit(const StatProgram& aStat) override;
  void visit(const StatDeclareLocal& aStat) override;
  void visit(const StatAssign& aStat) override;
  void visit(const StatExpr& aStat) override;
  void visit(const StatCode& aStat) override;
  void visit(const StatIfThen& aStat) override;
  void visit(const StatIfThen::ElseIf& aStat) override;
  void visit(const StatIfThen::Else& aStat) override;
  void visit(const StatIfIs& aStat) override;
  void visit(const StatLoop& aStat) override;
  void visit(const StatLoop::While& aStat) override;
  void visit(const StatLoop::ForStep& aStat) override;
  void visit(const StatLoop::ForRange& aStat) override;
  void visit(const StatData& aStat) override;
  void visit(const StatOutput& aStat) override;
  void visit(const StatOutput::Comma& aStat) override;
  void visit(const StatOutput::Concat& aStat) override;
  void visit(const StatOutput::End& aStat) override;
  void visit(const StatOutput::Value& aStat) override;
  void visit(const StatInput& aStat) override;
  void visit(const StatInput::Comma& aStat) override;
  void visit(const StatInput::Concat& aStat) override;
  void visit(const StatInput::End& aStat) override;
  void visit(const StatInput::Value& aStat) override;
  void visit(const StatError& aStat) override;
  void visit(const StatWrite& aStat) override;
  void visit(const StatRead& aStat) override;
  void visit(const StatDefine& aStat) override;

private:
  std::string in(int extra = 0);
  void printLineNr(const Location& aLocation);
	char end();
	void codeOutputNew();
	void codeOutput(const std::string& aString);
	void codeInputStart(const StatInput& aStat, const std::string& aString, InputType inputType = InputType::Normal);
	std::string codeExpr(const Expr& aValue);
	void codeLocalVar(const TreeNodePtr& aNode, bool inImplObject = false);
	void codeDeclIdentifier(bool aExtern,
													const IdentifierPtr& aIdentifier,
													bool initializeVariables,
													const Location& aLocation);
	void codeCaseIsListIf(const StatIfIs::IsList& aStat, const StatIfIs& aIfIsStat, bool firstIf);
	void codeCaseIsListSwitch(const Stat& aStat);
	void codeIfIsSwitch(const StatIfIs& aStat);
	void codeIfIsIfs(const StatIfIs& aStat, const Stat::SPtr& aElsePart);
	void codeCaseIsSwitch(const StatIfIs::Is& aStat);
  void codeCodeInclude(const TreeNodePtr& aNode);
	void codeOutputStart(const StatOutput& aStat, const std::string& aOutput, bool aDataOutput = false);
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
	bool isNestedConstIntScalar(const Stat& aStat);
	bool isNestedNonConstIntScalar(const Stat& aStat);

	NateParser*                 mParser = nullptr;
  std::ostream*								mOut;
  int													mIndent = 0;
	std::string                 mCachedOutput;
	bool                        mDataOutput = false;
	bool                        mFirstOutput = true;
	bool                        mStartOutput = true;
  InputType                   mCurInputType = InputType::Normal;
  bool                        mLastInputComma = false;
  bool                        mNextInputEnd = false;
	std::string                 mStream;

	int													mPrevLine = 0;
	std::string									mPrevFile;
};

