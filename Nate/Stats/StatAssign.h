#pragma once

#include "Stat.h"
#include "StatExpr.h"
#include "Identifier.h"

class StatAssign : public Stat
{
public:
  StatAssign(const Location& aLocation, const std::vector<Expr>& aIdentifiers, const Expr& aValue);
  
  void accept(ICodeVisitor* aVisitor) const override;

  const std::vector<Expr>& getIdentifiers() const;

  const Expr& getValue() const;

private:
  std::vector<Expr> mIdentifiers;
  Expr              mValue;  
};

