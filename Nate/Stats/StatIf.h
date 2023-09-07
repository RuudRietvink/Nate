#pragma once

#include "Stat.h"
#include "Expr.h"

class StatIf : public Stat
{
public:
  StatIf(const Location& aLocation, const Expr& aExpr);
  
  void accept(ICodeVisitor* aVisitor) const override;

  const Expr& getExpr() const;

private:
  Expr mExpr;
};

