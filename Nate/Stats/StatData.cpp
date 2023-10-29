#include "StatData.h"
#include "ICodeVisitor.h"

StatData::StatData(const Location& aLocation, const IdentifierPtr& anId)
  : StatOutput(aLocation),
    mId(anId)
{}

const IdentifierPtr& StatData::getId() const
{
  return mId;
}

void StatData::accept(ICodeVisitor* aVisitor, const std::vector<bool>& aFlags) const
{
  aVisitor->visit(*this, aFlags);
}
