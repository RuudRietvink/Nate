#include "StatExpr.h"
#include "ICodeVisitor.h"

StatExpr::StatExpr(const Location& aLocation, const Expr& aExpr)
  : Stat(aLocation),
    mExpr(aExpr)
{}

const Expr& StatExpr::getExpr() const
{
  return mExpr;
}