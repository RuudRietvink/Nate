#include "StatObject.h"
#include "ICodeVisitor.h"

StatObject::StatObject(const Location& aLocation, const ObjectPtr& aObject, bool aIsDecl)
  : Stat(aLocation),
    mObject(aObject),
    mIsDecl(aIsDecl)
{}

void StatObject::accept(ICodeVisitor* aVisitor, const std::vector<bool>& aFlags) const
{
  aVisitor->visit(*this, aFlags);
}

const ObjectPtr& StatObject::getObject() const
{
  return mObject;
}

bool StatObject::isDecl() const
{
  return mIsDecl;
}