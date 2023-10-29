#pragma once

#include "Location.h"

#include <list>
#include <memory>
#include <vector>

class ICodeVisitor;

class Stat
{
public:
  using SPtr = std::shared_ptr<Stat>;

  using List = std::list<SPtr>;
  Stat(const Location& aLocation);

  virtual void accept(ICodeVisitor* aVisitor, const std::vector<bool>& aFlags = { false }) const = 0;

  SPtr addStat(const SPtr& aStat);

  const Location& getLocation() const;

  void setLocation(const Location& aLocation);

  const List& getCompound() const;

protected:
  List mCompound;

private:
  Location  mLocation;
};

