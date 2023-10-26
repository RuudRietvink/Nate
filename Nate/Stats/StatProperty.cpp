#include "StatProperty.h"
#include "ICodeVisitor.h"

StatProperty::StatProperty(const Location& aLocation, const IdentifierPtr& aId, const ObjectPtr& aObject, Object::PropType aPropType)
  : Stat(aLocation),
    mId(aId),
    mObject(aObject),
    mPropType(aPropType)
{}

const IdentifierPtr& StatProperty::getId() const
{
  return mId;
}

void StatProperty::accept(ICodeVisitor* aVisitor) const
{
  aVisitor->visit(*this);
}

const ObjectPtr& StatProperty::getObject() const
{
  return mObject;
}

Object::PropType StatProperty::getPropType() const
{
  return mPropType;
}