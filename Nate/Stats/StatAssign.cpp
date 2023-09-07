#include "StatAssign.h"
#include "ICodeVisitor.h"

StatAssign::StatAssign(const Location& aLocation, const std::vector<Expr>& aIdentifiers, const Expr& aValue)
  : Stat(aLocation),
    mIdentifiers(aIdentifiers),
    mValue(aValue)
{}

void StatAssign::accept(ICodeVisitor* aVisitor) const
{
  aVisitor->visit(*this);
}

const std::vector<Expr>& StatAssign::getIdentifiers() const
{
  return mIdentifiers;
}

const Expr& StatAssign::getValue() const
{
  return mValue;
}