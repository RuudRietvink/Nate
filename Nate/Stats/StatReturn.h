#pragma once

#include "StatWithExpr.h"
#include "Expr.h"

namespace nate
{
class StatReturn : public StatWithExpr
{
public:
  StatReturn(const Location& aLocation, const Expr& aExpr);

  void accept(ICodeVisitor* aVisitor) const override;
};

}
