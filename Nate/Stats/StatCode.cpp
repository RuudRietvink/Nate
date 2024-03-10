#include "StatCode.h"
#include "ICodeVisitor.h"

namespace nate
{
StatCode::StatCode(const Location& aLocation, const std::string& aCode)
  : Stat(aLocation),
    mCode(aCode)
{}

const std::string& StatCode::getCode() const
{
  return mCode;
}

void StatCode::accept(ICodeVisitor* aVisitor) const
{
  aVisitor->visit(*this);
}

}
