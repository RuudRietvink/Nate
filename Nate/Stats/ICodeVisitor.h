#pragma once

#include "StatProgram.h"
#include "StatDeclareLocal.h"
#include "StatOutput.h"
#include "StatAssign.h"
#include "StatOutput.h"
#include "StatIf.h"
#include "StatElseIf.h"
#include "StatElse.h"
#include "StatExpr.h"
#include "StatLoop.h"

class ICodeVisitor
{
public:
  virtual void visit(const StatProgram& aStat) = 0;
  virtual void visit(const StatDeclareLocal& aStat) = 0;
  virtual void visit(const StatAssign& aStat) = 0;
  virtual void visit(const StatExpr& aStat) = 0;
  virtual void visit(const StatIf& aStat) = 0;
  virtual void visit(const StatElseIf& aStat) = 0;
  virtual void visit(const StatElse& aStat) = 0;
  virtual void visit(const StatLoop& aStat) = 0;
  virtual void visit(const StatLoop::While& aStat) = 0;
  virtual void visit(const StatLoop::ForStep& aStat) = 0;
  virtual void visit(const StatLoop::ForRange& aStat) = 0;
  virtual void visit(const StatOutput& aStat) = 0;
  virtual void visit(const StatOutput::Comma& aStat) = 0;
  virtual void visit(const StatOutput::Concat& aStat) = 0;
  virtual void visit(const StatOutput::End& aStat) = 0;
  virtual void visit(const StatOutput::Expr& aStat) = 0;
};

