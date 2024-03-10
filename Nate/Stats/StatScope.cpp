#include "StatScope.h"
#include "ICodeVisitor.h"

namespace nate
{
StatScope::StatScope(const Location& aLocation)
  : Stat(aLocation)
{}

void StatScope::accept(ICodeVisitor* aVisitor) const
{
  aVisitor->visit(*this);
}

}
