#include "StatReturn.h"
#include "ICodeVisitor.h"

StatReturn::StatReturn(const Location& aLocation, const Expr& aExpr)
  : StatWithExpr(aLocation, aExpr)
{}

void StatReturn::accept(ICodeVisitor* aVisitor) const
{
  aVisitor->visit(*this);
}
