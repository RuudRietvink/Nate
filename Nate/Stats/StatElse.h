#pragma once

#include "Stat.h"

class StatElse : public Stat
{
public:
  StatElse(const Location& aLocation);
  
  void accept(ICodeVisitor* aVisitor) const override;
};

