#pragma once

#include "NateData.h"
#include "Object.h"
#include "ICodeVisitor.h"
#include "Define.h"

#include <ostream>

namespace nate
{
class NateParser;

class NateCode : public ICodeVisitor
{
public:
	NateCode();
    void setData(std::ostream& aOut, NateParser* aParser);

    void createCodeCall(Define* define);
    Expr createTypeCastNode(
        const ExprNodesCIter& aNodeIter,
        const Arg& aArg,
        const TypePtr& aTemplateType,
        const TypePtr& aFirstType,
        const TypePtr& aHighestType,
        std::string& aNodeCode) const;
    Method::EvaluateResult
        createMethodCode(const Method* aMethod, const DefinePtr& aCurDefine, const ExprNodesCIter& aBegin, const ExprNodesCIter& aEnd, int aDebug) const;
    void createArgCode(
        const Method* aMethod,
        const Arg& aArg,
        const Expr& aNode,
        const DefinePtr& aCurDefine,
        const std::string& aNodeCode,
        bool aIsObjectArg,
        // ->
        std::string& resultCode) const;

    bool castToType(Expr* aExpr, const TypePtr& aToType) const;
    bool castToType(Expr::Node* aNode, const TypePtr& aToType) const;

	void codeStats(const std::list<Stat::SPtr>& aStats);
	void codeCompound(const Stat& aStat);
	void codeBlock(const Stat& aStat);
	void visit(const StatAssign& aStat) override;
	void visit(const StatCode& aStat) override;
	void visit(const StatData& aStat) override;
	void visit(const StatDeclareLocal& aStat) override;
	void visit(const StatDefine& aStat) override;
	void visit(const StatError& aStat) override;
	void visit(const StatExpr& aStat) override;
	void visit(const StatIfIs& aStat) override;
	void visit(const StatIfIs::Else& aStat) override;
	void visit(const StatIfThen& aStat) override;
	void visit(const StatIfThen::ElseIf& aStat) override;
	void visit(const StatIfThen::Else& aStat) override;
	void visit(const StatInput& aStat) override;
	void visit(const StatInput::Comma& aStat) override;
	void visit(const StatInput::Concat& aStat) override;
	void visit(const StatInput::End& aStat) override;
	void visit(const StatInput::Value& aStat) override;
	void visit(const StatLoop& aStat) override;
	void visit(const StatLoop::ForRange& aStat) override;
	void visit(const StatLoop::ForStep& aStat) override;
	void visit(const StatLoop::While& aStat) override;
	void visit(const StatObject& aStat) override;
	void visit(const StatOutput& aStat) override;
	void visit(const StatOutput::Comma& aStat) override;
	void visit(const StatOutput::Concat& aStat) override;
	void visit(const StatOutput::End& aStat) override;
	void visit(const StatOutput::Value& aStat) override;
	void visit(const StatProgram& aStat) override;
	void visit(const StatProperty& aStat) override;
	void visit(const StatRead& aStat) override;
	void visit(const StatRecord& aStat) override;
	void visit(const StatReturn& aStat) override;
	void visit(const StatScope& aStat) override;
	void visit(const StatWrite& aStat) override;

private:
    std::string in(int extra = 0);
    void printLineNr(const Location& aLocation);
	char end();
	void codeOutput(const std::string& aString);
    void codeOutputEnd();
	void codeInputStart(const StatInput& aStat, const std::string& aString, InputType inputType = InputType::Normal);
	std::string codeExpr(const Expr& aValue);
	void codeDeclIdentifier(bool aExtern,
							const IdentifierPtr& aIdentifier,
							bool initializeVariables,
							const Location& aLocation);
	void codeCaseIsListIf(const StatIfIs::IsList& aStat, const StatIfIs& aIfIsStat, bool firstIf);
	void codeCaseIsListSwitch(const Stat& aStat);
	void codeIfIsSwitch(const StatIfIs& aStat);
	void codeIfIsIfs(const StatIfIs& aStat, const Stat::SPtr& aElsePart);
	void codeCaseIsSwitch(const StatIfIs::Is& aStat);
	void codeOutputStart(const StatOutput& aStat, const std::string& aOutput, bool aDataOutput = false);
	std::string createCodeDeclArgs(const DefinePtr& aDefine, const std::vector<Arg>& aArgs);
	std::string createCodeDecl(const DefinePtr& aDefine, const std::string& aObjectName = "");
	void codeObjectBases(const ObjectPtr& aObject);
	std::string codePropHeader(const ObjectPtr& aObject,
							   bool aAddObjectName,
							   const IdentifierPtr& aId, 
							   Property::PropType aPropType);
	void codeDefaultPropertyImpl(const ObjectPtr& aObject,
								 const IdentifierPtr& propId,
							 	 const Object::PropData& propdata);
	void codeDefaultProperties(const ObjectPtr& aObject);
	void codeDeclProperties(const ObjectPtr& aObject);
	void codeDeclareProperty(const ObjectPtr& aObject,
							 const IdentifierPtr& aId,
							 const Location& aLocation);
	void codeImplObjectVariables(const StatObject& aStat);
	void codeImplObjectNested(const StatObject& aStat, bool inImpl);
  
	void codeDefine(const StatDefine& aStat, const ObjectPtr& aObject = ObjectPtr());


	bool isConstIntScalar(const Expr& aExpr);
	bool isNestedConstIntScalar(const Stat& aStat);
	bool isNestedNonConstIntScalar(const Stat& aStat);

	NateParser*                 mParser = nullptr;
	std::ostream*				mOut;
	int							mIndent = 0;
	std::string                 mCachedOutput;
	bool                        mDataOutput = false;
	InputType                   mCurInputType = InputType::Normal;
	bool                        mLastInputComma = false;
	bool                        mNextInputEnd = false;
	std::string                 mStream;

	int							mPrevLine = 0;
	std::string					mPrevFile;
};

}
