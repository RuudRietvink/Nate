#pragma once

#include "Stat.h"
#include "Expr.h"

class StatExpr : public Stat
{
public:
  StatExpr(const Location& aLocation, const Expr& aExpr);

  const Expr& getExpr() const;

private:
  Expr mExpr;
};

