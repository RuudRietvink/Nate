#pragma once

#include "Stat.h"
#include "Object.h"
#include "Identifier.h"
#include "Property.h"

namespace nate
{
class StatProperty : public Stat
{
public:
  StatProperty(const Location& aLocation, const IdentifierPtr& aId, const ObjectPtr& aObject, bool aImpOnly);
  
  const IdentifierPtr& getId() const;

  const ObjectPtr& getObject() const;

  bool isImpOnly() const;

  const PropertyPtr& getProperty(Property::PropType propType) const;

  void setProperty(Property::PropType propType, const PropertyPtr aProperty);

  void accept(ICodeVisitor* aVisitor) const override;

private:
  IdentifierPtr mId;
  ObjectPtr mObject;
  bool mImpOnly;
  PropertyPtr mProperties[2];
};

}
