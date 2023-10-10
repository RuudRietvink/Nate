#include "StatExpr.h"
#include "ICodeVisitor.h"

StatExpr::StatExpr(const Location& aLocation, const Expr& aExpr)
  : StatWithExpr(aLocation, aExpr)
{}

void StatExpr::accept(ICodeVisitor* aVisitor) const
{
  aVisitor->visit(*this);
}
