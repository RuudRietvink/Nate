#pragma once

#include "StatWithExpr.h"
#include "Expr.h"

class StatExpr : public StatWithExpr
{
public:
  StatExpr(const Location& aLocation, const Expr& aExpr);

  void accept(ICodeVisitor* aVisitor) const override;
};

