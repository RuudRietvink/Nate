#include "StatIfThen.h"
#include "ICodeVisitor.h"

namespace nate
{
StatIfThen::StatIfThen(const Location& aLocation, const Expr& aExpr)
  : StatWithExpr(aLocation, aExpr)
{}

void StatIfThen::accept(ICodeVisitor* aVisitor) const
{
    aVisitor->visit(*this);
}


StatIfThen::ElseIf::ElseIf(const Location& aLocation, const Expr& aExpr)
  : StatWithExpr(aLocation, aExpr)
{}

void StatIfThen::ElseIf::accept(ICodeVisitor* aVisitor) const
{
    aVisitor->visit(*this);
}


StatIfThen::Else::Else(const Location& aLocation)
  : Stat(aLocation)
{}

void StatIfThen::Else::accept(ICodeVisitor* aVisitor) const
{
    aVisitor->visit(*this);
}

}
