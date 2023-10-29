#pragma once

#include "StatWithExpr.h"
#include "Expr.h"

class StatIfThen : public StatWithExpr
{
public:
  class ElseIf : public StatWithExpr
  {
  public:
    ElseIf(const Location& aLocation, const Expr& aExpr);
  
    void accept(ICodeVisitor* aVisitor, const std::vector<bool>& aFlags) const override;
  };
  
  class Else : public Stat
  {
  public:
    Else(const Location& aLocation);
  
    void accept(ICodeVisitor* aVisitor, const std::vector<bool>& aFlags) const override;
  };

  StatIfThen(const Location& aLocation, const Expr& aExpr);
  
  void accept(ICodeVisitor* aVisitor, const std::vector<bool>& aFlags) const override;
};

