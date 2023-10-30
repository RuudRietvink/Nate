#pragma once

#include "Stat.h"

class StatProgram : public Stat
{
public:
  StatProgram(const Location& aLocation);

  void accept(ICodeVisitor* aVisitor) const override;
};

