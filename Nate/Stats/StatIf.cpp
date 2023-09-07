#include "StatIf.h"
#include "ICodeVisitor.h"

StatIf::StatIf(const Location& aLocation, const Expr& aExpr)
  : Stat(aLocation),
    mExpr(aExpr)
{}

void StatIf::accept(ICodeVisitor* aVisitor) const
{
  aVisitor->visit(*this);
}

const Expr& StatIf::getExpr() const
{
  return mExpr;
}