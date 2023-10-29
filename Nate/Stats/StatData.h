#pragma once

#include "StatOutput.h"

class StatData : public StatOutput
{
public:
  StatData(const Location& aLocation, const IdentifierPtr& anId);

  void accept(ICodeVisitor* aVisitor, const std::vector<bool>& aFlags) const override;

  const IdentifierPtr& getId() const;
  
private:
  IdentifierPtr mId;
};
