#pragma once

#include "Stat.h"
#include "Expr.h"

namespace nate
{
class StatWithExpr : public Stat
{
public:
  StatWithExpr(const Location& aLocation, const Expr& aExpr);

  const Expr& getExpr() const;

private:
  Expr mExpr;
};

}
