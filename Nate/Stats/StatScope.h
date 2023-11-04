#pragma once

#include "Stat.h"

class StatScope : public Stat
{
public:
  StatScope(const Location& aLocation);

  void accept(ICodeVisitor* aVisitor) const override;
};
