#pragma once

#include "StatAssign.h"
#include "StatCode.h"
#include "StatData.h"
#include "StatDeclareLocal.h"
#include "StatDefine.h"
#include "StatError.h"
#include "StatExpr.h"
#include "StatIfThen.h"
#include "StatIfIs.h"
#include "StatInput.h"
#include "StatLoop.h"
#include "StatObject.h"
#include "StatOutput.h"
#include "StatProgram.h"
#include "StatProperty.h"
#include "StatRead.h"
#include "StatRecord.h"
#include "StatReturn.h"
#include "StatScope.h"
#include "StatWrite.h"

namespace nate
{
class ICodeVisitor
{
public:
    virtual void visit(const StatAssign& aStat) = 0;
    virtual void visit(const StatCode& aStat) = 0;
    virtual void visit(const StatData& aStat) = 0;
    virtual void visit(const StatDeclareLocal& aStat) = 0;
    virtual void visit(const StatDefine& aStat) = 0;
    virtual void visit(const StatError& aStat) = 0;
    virtual void visit(const StatExpr& aStat) = 0;
    virtual void visit(const StatIfIs& aStat) = 0;
    virtual void visit(const StatIfIs::Else& aStat) = 0;
    virtual void visit(const StatIfThen& aStat) = 0;
    virtual void visit(const StatIfThen::ElseIf& aStat) = 0;
    virtual void visit(const StatIfThen::Else& aStat) = 0;
    virtual void visit(const StatInput& aStat) = 0;
    virtual void visit(const StatInput::Comma& aStat) = 0;
    virtual void visit(const StatInput::Concat& aStat) = 0;
    virtual void visit(const StatInput::End& aStat) = 0;
    virtual void visit(const StatInput::Value& aStat) = 0;
    virtual void visit(const StatLoop& aStat) = 0;
    virtual void visit(const StatLoop::ForRange& aStat) = 0;
    virtual void visit(const StatLoop::ForStep& aStat) = 0;
    virtual void visit(const StatLoop::While& aStat) = 0;
    virtual void visit(const StatObject& aStat) = 0;
    virtual void visit(const StatOutput& aStat) = 0;
    virtual void visit(const StatOutput::Comma& aStat) = 0;
    virtual void visit(const StatOutput::Concat& aStat) = 0;
    virtual void visit(const StatOutput::End& aStat) = 0;
    virtual void visit(const StatOutput::Value& aStat) = 0;
    virtual void visit(const StatProgram& aStat) = 0;
    virtual void visit(const StatProperty& aStat) = 0;
    virtual void visit(const StatRead& aStat) = 0;
    virtual void visit(const StatRecord& aStat) = 0;
    virtual void visit(const StatReturn& aStat) = 0;
    virtual void visit(const StatScope& aStat) = 0;
    virtual void visit(const StatWrite& aStat) = 0;
};

}
