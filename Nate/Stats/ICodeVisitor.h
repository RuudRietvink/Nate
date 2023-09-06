#pragma once

class StatProgram;
class StatDeclareLocal;
class StatAssign;
class StatExpr;
class StatOutput;
class StatOutputComma;
class StatOutputConcat;
class StatOutputEnd;
class StatOutputExpr;

class ICodeVisitor
{
public:
  virtual void visit(const StatProgram& aStat) = 0;
  virtual void visit(const StatDeclareLocal& aStat) = 0;
  virtual void visit(const StatAssign& aStat) = 0;
  virtual void visit(const StatExpr& aStat) = 0;
  virtual void visit(const StatOutput& aStat) = 0;
  virtual void visit(const StatOutputComma& aStat) = 0;
  virtual void visit(const StatOutputConcat& aStat) = 0;
  virtual void visit(const StatOutputEnd& aStat) = 0;
  virtual void visit(const StatOutputExpr& aStat) = 0;
};

