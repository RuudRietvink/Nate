#include "StatError.h"
#include "ICodeVisitor.h"

namespace nate
{
StatError::StatError(const Location& aLocation)
  : StatOutput(aLocation)
{}


void StatError::accept(ICodeVisitor* aVisitor) const
{
    aVisitor->visit(*this);
}

}
