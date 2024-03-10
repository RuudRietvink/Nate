#include "StatExpr.h"
#include "ICodeVisitor.h"

namespace nate
{
StatExpr::StatExpr(const Location& aLocation, const Expr& aExpr)
  : StatWithExpr(aLocation, aExpr)
{}

void StatExpr::accept(ICodeVisitor* aVisitor) const
{
  aVisitor->visit(*this);
}

}
