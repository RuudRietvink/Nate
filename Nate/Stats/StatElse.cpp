#include "StatElse.h"
#include "ICodeVisitor.h"

StatElse::StatElse(const Location& aLocation)
  : Stat(aLocation)
{}

void StatElse::accept(ICodeVisitor* aVisitor) const
{
  aVisitor->visit(*this);
}
