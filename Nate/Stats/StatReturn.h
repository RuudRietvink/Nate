#pragma once

#include "StatWithExpr.h"
#include "Expr.h"

class StatReturn : public StatWithExpr
{
public:
  StatReturn(const Location& aLocation, const Expr& aExpr);

  void accept(ICodeVisitor* aVisitor) const override;
};

