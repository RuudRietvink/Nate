#pragma once

#include "Stat.h"
#include "Object.h"
#include "Identifier.h"

class StatProperty : public Stat
{
public:
  StatProperty(const Location& aLocation, const IdentifierPtr& aId, const ObjectPtr& aObject, Object::PropType aPropType);
  
  const IdentifierPtr& getId() const;

  const ObjectPtr& getObject() const;

  Object::PropType getPropType() const;

  void accept(ICodeVisitor* aVisitor) const override;

private:
  IdentifierPtr mId;
  ObjectPtr mObject;
  Object::PropType mPropType;
};

