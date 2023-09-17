#pragma once

#include "Stat.h"

class StatCode: public Stat
{
public:
  StatCode(const Location& aLocation, const std::string& aCode);

  const std::string& getCode() const;

  void accept(ICodeVisitor* aVisitor) const override;

private:
  std::string mCode;
};

