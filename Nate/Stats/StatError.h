#pragma once

#include "StatOutput.h"

class StatError : public StatOutput
{
public:
  StatError(const Location& aLocation);

  void accept(ICodeVisitor* aVisitor) const override;
};
