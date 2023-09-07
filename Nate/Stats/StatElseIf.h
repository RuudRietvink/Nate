#pragma once

#include "StatIf.h"

class Expr;

class StatElseIf : public StatIf
{
public:
  StatElseIf(const Location& aLocation, const Expr& aExpr);
  
  void accept(ICodeVisitor* aVisitor) const override;
};

