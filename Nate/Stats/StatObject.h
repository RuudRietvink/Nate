#pragma once

#include "Stat.h"
#include "Object.h"

class StatObject : public Stat
{
public:
  StatObject(const Location& aLocation, const ObjectPtr& aObject, bool aIsDecl);

  const ObjectPtr& getObject() const;

  bool isDecl() const;

  void accept(ICodeVisitor* aVisitor, const std::vector<bool>& aFlags) const override;

private:
  ObjectPtr mObject;
  bool mIsDecl;
};

