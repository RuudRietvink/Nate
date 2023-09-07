#include "StatElseIf.h"
#include "ICodeVisitor.h"

StatElseIf::StatElseIf(const Location& aLocation, const Expr& aExpr)
  : StatIf(aLocation, aExpr)
{}

void StatElseIf::accept(ICodeVisitor* aVisitor) const
{
  aVisitor->visit(*this);
}
