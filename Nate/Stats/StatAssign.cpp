#include "StatAssign.h"
#include "ICodeVisitor.h"

StatAssign::StatAssign(const Location& aLocation, const std::vector<Expr>& aIdentifiers, const Expr& aValue)
  : StatWithExpr(aLocation, aValue),
    mIdentifiers(aIdentifiers)
{}

void StatAssign::accept(ICodeVisitor* aVisitor, const std::vector<bool>& aFlags) const
{
  aVisitor->visit(*this, aFlags);
}

const std::vector<Expr>& StatAssign::getIdentifiers() const
{
  return mIdentifiers;
}
