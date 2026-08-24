#include "StatIfIs.h"
#include "ICodeVisitor.h"

namespace nate
{
StatIfIs::StatIfIs(const Location& aLocation, const IdentifierPtr& aId, const Expr& aExpr)
  : StatWithExpr(aLocation, aExpr),
    mId(aId)
{}

void StatIfIs::accept(ICodeVisitor* aVisitor) const
{
    aVisitor->visit(*this);
}

const IdentifierPtr& StatIfIs::getId() const
{
    return mId;
}

StatIfIs::IsList::IsList(const Location& aLocation)
  : Stat(aLocation)
{}

void StatIfIs::IsList::accept(ICodeVisitor* /*aVisitor*/) const
{
}

StatIfIs::Else::Else(const Location& aLocation)
  : Stat(aLocation)
{}

void StatIfIs::Else::accept(ICodeVisitor* aVisitor) const
{
    aVisitor->visit(*this);
}

StatIfIs::Is::Is(const Location& aLocation, const Expr& aExpr)
  : StatWithExpr(aLocation, aExpr)
{}

void StatIfIs::Is::accept(ICodeVisitor* /*aVisitor*/) const
{
}

}
