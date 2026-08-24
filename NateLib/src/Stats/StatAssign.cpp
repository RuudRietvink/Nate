#include "StatAssign.h"
#include "ICodeVisitor.h"

namespace nate
{
StatAssign::StatAssign(const Location& aLocation, const std::vector<Expr>& aIdentifiers, const Expr& aValue)
  : StatWithExpr(aLocation, aValue),
    mIdentifiers(aIdentifiers)
{}

void StatAssign::accept(ICodeVisitor* aVisitor) const
{
    aVisitor->visit(*this);
}

const std::vector<Expr>& StatAssign::getIdentifiers() const
{
    return mIdentifiers;
}

}
