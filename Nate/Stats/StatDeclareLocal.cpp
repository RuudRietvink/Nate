#include "StatDeclareLocal.h"
#include "ICodeVisitor.h"

namespace nate
{
StatDeclareLocal::StatDeclareLocal(const Location& aLocation, const IdentifierPtr& aIdentifier, const Expr& aValue)
  : StatWithExpr(aLocation, aValue),
    mIdentifier(aIdentifier)
{}

void StatDeclareLocal::accept(ICodeVisitor* aVisitor) const
{
  aVisitor->visit(*this);
}

const IdentifierPtr& StatDeclareLocal::getIdentifier() const
{
  return mIdentifier;
}

}