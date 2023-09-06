#include "Stat.h"

Stat::Stat(const Location& aLocation)
  : mLocation(aLocation)
{
}

Stat::SPtr Stat::addStat(const SPtr& aStat)
{
  mCompound.push_back(aStat);
  return aStat;
}

const Location& Stat::getLocation() const
{
  return mLocation;
}
  
void Stat::setLocation(const Location& aLocation)
{
  mLocation = aLocation;
}

const Stat::List& Stat::getCompound() const
{
  return mCompound;
}