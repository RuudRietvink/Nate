#include "StatWithExpr.h"
#include "ICodeVisitor.h"

namespace nate
{
StatWithExpr::StatWithExpr(const Location& aLocation, const Expr& aExpr)
  : Stat(aLocation),
    mExpr(aExpr)
{}

const Expr& StatWithExpr::getExpr() const
{
  return mExpr;
}

}
