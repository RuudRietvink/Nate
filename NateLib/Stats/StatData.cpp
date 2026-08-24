#include "StatData.h"
#include "ICodeVisitor.h"

namespace nate
{
StatData::StatData(const Location& aLocation, const IdentifierPtr& anId)
  : StatOutput(aLocation),
    mId(anId)
{}

const IdentifierPtr& StatData::getId() const
{
    return mId;
}

void StatData::accept(ICodeVisitor* aVisitor) const
{
    aVisitor->visit(*this);
}

}
