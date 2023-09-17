#pragma once

#include "StatProgram.h"
#include "StatDeclareLocal.h"
#include "StatAssign.h"
#include "StatExpr.h"
#include "StatCode.h"
#include "StatIfThen.h"
#include "StatIfIs.h"
#include "StatLoop.h"
#include "StatOutput.h"
#include "StatWrite.h"

class ICodeVisitor
{
public:
  virtual void visit(const StatProgram& aStat) = 0;
  virtual void visit(const StatDeclareLocal& aStat) = 0;
  virtual void visit(const StatAssign& aStat) = 0;
  virtual void visit(const StatExpr& aStat) = 0;
  virtual void visit(const StatCode& aStat) = 0;
  virtual void visit(const StatIfThen& aStat) = 0;
  virtual void visit(const StatIfThen::ElseIf& aStat) = 0;
  virtual void visit(const StatIfThen::Else& aStat) = 0;
  virtual void visit(const StatIfIs& aStat) = 0;
  virtual void visit(const StatLoop& aStat) = 0;
  virtual void visit(const StatLoop::While& aStat) = 0;
  virtual void visit(const StatLoop::ForStep& aStat) = 0;
  virtual void visit(const StatLoop::ForRange& aStat) = 0;
  virtual void visit(const StatOutput& aStat) = 0;
  virtual void visit(const StatOutput::Comma& aStat) = 0;
  virtual void visit(const StatOutput::Concat& aStat) = 0;
  virtual void visit(const StatOutput::End& aStat) = 0;
  virtual void visit(const StatOutput::Value& aStat) = 0;
  virtual void visit(const StatWrite& aStat) = 0;
};

