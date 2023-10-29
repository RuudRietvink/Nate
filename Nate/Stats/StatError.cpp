#include "StatError.h"
#include "ICodeVisitor.h"

StatError::StatError(const Location& aLocation)
  : StatOutput(aLocation)
{}


void StatError::accept(ICodeVisitor* aVisitor, const std::vector<bool>& aFlags) const
{
  aVisitor->visit(*this, aFlags);
}
