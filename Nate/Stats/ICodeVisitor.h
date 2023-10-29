#pragma once

#include "StatProgram.h"
#include "StatDeclareLocal.h"
#include "StatAssign.h"
#include "StatExpr.h"
#include "StatCode.h"
#include "StatIfThen.h"
#include "StatIfIs.h"
#include "StatLoop.h"
#include "StatData.h"
#include "StatOutput.h"
#include "StatError.h"
#include "StatWrite.h"
#include "StatInput.h"
#include "StatRead.h"
#include "StatDefine.h"
#include "StatRecord.h"
#include "StatReturn.h"
#include "StatObject.h"

class ICodeVisitor
{
public:
  virtual void visit(const StatProgram& aStat, const std::vector<bool>& aFlags) = 0;
  virtual void visit(const StatDeclareLocal& aStat, const std::vector<bool>& aFlags) = 0;
  virtual void visit(const StatAssign& aStat, const std::vector<bool>& aFlags) = 0;
  virtual void visit(const StatExpr& aStat, const std::vector<bool>& aFlags) = 0;
  virtual void visit(const StatCode& aStat, const std::vector<bool>& aFlags) = 0;
  virtual void visit(const StatIfThen& aStat, const std::vector<bool>& aFlags) = 0;
  virtual void visit(const StatIfThen::ElseIf& aStat, const std::vector<bool>& aFlags) = 0;
  virtual void visit(const StatIfThen::Else& aStat, const std::vector<bool>& aFlags) = 0;
  virtual void visit(const StatIfIs& aStat, const std::vector<bool>& aFlags) = 0;
  virtual void visit(const StatLoop& aStat, const std::vector<bool>& aFlags) = 0;
  virtual void visit(const StatLoop::While& aStat, const std::vector<bool>& aFlags) = 0;
  virtual void visit(const StatLoop::ForStep& aStat, const std::vector<bool>& aFlags) = 0;
  virtual void visit(const StatLoop::ForRange& aStat, const std::vector<bool>& aFlags) = 0;
  virtual void visit(const StatData& aStat, const std::vector<bool>& aFlags) = 0;
  virtual void visit(const StatOutput& aStat, const std::vector<bool>& aFlags) = 0;
  virtual void visit(const StatOutput::Comma& aStat, const std::vector<bool>& aFlags) = 0;
  virtual void visit(const StatOutput::Concat& aStat, const std::vector<bool>& aFlags) = 0;
  virtual void visit(const StatOutput::End& aStat, const std::vector<bool>& aFlags) = 0;
  virtual void visit(const StatOutput::Value& aStat, const std::vector<bool>& aFlags) = 0;
  virtual void visit(const StatError& aStat, const std::vector<bool>& aFlags) = 0;
  virtual void visit(const StatWrite& aStat, const std::vector<bool>& aFlags) = 0;
  virtual void visit(const StatRead& aStat, const std::vector<bool>& aFlags) = 0;
  virtual void visit(const StatInput& aStat, const std::vector<bool>& aFlags) = 0;
  virtual void visit(const StatInput::Comma& aStat, const std::vector<bool>& aFlags) = 0;
  virtual void visit(const StatInput::Concat& aStat, const std::vector<bool>& aFlags) = 0;
  virtual void visit(const StatInput::End& aStat, const std::vector<bool>& aFlags) = 0;
  virtual void visit(const StatInput::Value& aStat, const std::vector<bool>& aFlags) = 0;
  virtual void visit(const StatDefine& aStat, const std::vector<bool>& aFlags) = 0;
  virtual void visit(const StatRecord& aStat, const std::vector<bool>& aFlags) = 0;
  virtual void visit(const StatReturn& aStat, const std::vector<bool>& aFlags) = 0;
  virtual void visit(const StatObject& aStat, const std::vector<bool>& aFlags) = 0;
};

