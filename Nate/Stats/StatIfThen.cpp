#include "StatIfThen.h"
#include "ICodeVisitor.h"

StatIfThen::StatIfThen(const Location& aLocation, const Expr& aExpr)
  : StatWithExpr(aLocation, aExpr)
{}

void StatIfThen::accept(ICodeVisitor* aVisitor, const std::vector<bool>& aFlags) const
{
  aVisitor->visit(*this, aFlags);
}


StatIfThen::ElseIf::ElseIf(const Location& aLocation, const Expr& aExpr)
  : StatWithExpr(aLocation, aExpr)
{}

void StatIfThen::ElseIf::accept(ICodeVisitor* aVisitor, const std::vector<bool>& aFlags) const
{
  aVisitor->visit(*this, aFlags);
}


StatIfThen::Else::Else(const Location& aLocation)
  : Stat(aLocation)
{}

void StatIfThen::Else::accept(ICodeVisitor* aVisitor, const std::vector<bool>& aFlags) const
{
  aVisitor->visit(*this, aFlags);
}
