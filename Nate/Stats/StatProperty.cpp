#include "StatProperty.h"
#include "ICodeVisitor.h"

StatProperty::StatProperty(const Location& aLocation, const IdentifierPtr& aId, const ObjectPtr& aObject, bool aImpOnly)
  : Stat(aLocation),
    mId(aId),
    mObject(aObject),
    mImpOnly(aImpOnly)
{}

const IdentifierPtr& StatProperty::getId() const
{
  return mId;
}

bool StatProperty::isImpOnly() const
{
  return mImpOnly;
}

void StatProperty::accept(ICodeVisitor* aVisitor) const
{
  aVisitor->visit(*this);
}

const ObjectPtr& StatProperty::getObject() const
{
  return mObject;
}

const PropertyPtr& StatProperty::getProperty(Property::PropType propType) const
{
  return mProperties[static_cast<int>(propType)];
}

void StatProperty::setProperty(Property::PropType propType, const PropertyPtr aProperty)
{
  mProperties[static_cast<int>(propType)] = aProperty;
}
