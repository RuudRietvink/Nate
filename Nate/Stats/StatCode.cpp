#include "StatCode.h"
#include "ICodeVisitor.h"

StatCode::StatCode(const Location& aLocation, const std::string& aCode)
  : Stat(aLocation),
    mCode(aCode)
{}

const std::string& StatCode::getCode() const
{
  return mCode;
}

void StatCode::accept(ICodeVisitor* aVisitor, const std::vector<bool>& aFlags) const
{
  aVisitor->visit(*this, aFlags);
}
