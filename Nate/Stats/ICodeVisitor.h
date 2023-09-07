#pragma once

#include "StatProgram.h"
#include "StatDeclareLocal.h"
#include "StatOutput.h"
#include "StatAssign.h"
#include "StatOutput.h"
#include "StatIf.h"
#include "StatElse.h"
#include "StatExpr.h"

class ICodeVisitor
{
public:
  virtual void visit(const StatProgram& aStat) = 0;
  virtual void visit(const StatDeclareLocal& aStat) = 0;
  virtual void visit(const StatAssign& aStat) = 0;
  virtual void visit(const StatExpr& aStat) = 0;
  virtual void visit(const StatIf& aStat) = 0;
  virtual void visit(const StatElse& aStat) = 0;
  virtual void visit(const StatOutput& aStat) = 0;
  virtual void visit(const StatOutputComma& aStat) = 0;
  virtual void visit(const StatOutputConcat& aStat) = 0;
  virtual void visit(const StatOutputEnd& aStat) = 0;
  virtual void visit(const StatOutputExpr& aStat) = 0;
};

