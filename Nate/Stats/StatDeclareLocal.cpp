#include "StatDeclareLocal.h"
#include "ICodeVisitor.h"

StatDeclareLocal::StatDeclareLocal(const Location& aLocation, const IdentifierPtr& aIdentifier, const Expr& aValue)
  : StatWithExpr(aLocation, aValue),
    mIdentifier(aIdentifier)
{}

void StatDeclareLocal::accept(ICodeVisitor* aVisitor, const std::vector<bool>& aFlags) const
{
  aVisitor->visit(*this, aFlags);
}

const IdentifierPtr& StatDeclareLocal::getIdentifier() const
{
  return mIdentifier;
}
