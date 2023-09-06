#include "StatDeclareLocal.h"


StatDeclareLocal::StatDeclareLocal(const Location& aLocation, const IdentifierPtr& aIdentifier, const Expr& aValue)
  : Stat(aLocation),
    mIdentifier(aIdentifier),
    mValue(aValue)
{}

void StatDeclareLocal::accept(ICodeVisitor* aVisitor) const
{
  aVisitor->visit(*this);
}

const IdentifierPtr& StatDeclareLocal::getIdentifier() const
{
  return mIdentifier;
}

const Expr& StatDeclareLocal::getValue() const
{
  return mValue;
}