#pragma once

#include "StatWithExpr.h"
#include "Identifier.h"

namespace nate
{
class StatAssign : public StatWithExpr
{
public:
  StatAssign(const Location& aLocation, const std::vector<Expr>& aIdentifiers, const Expr& aValue);
  
  void accept(ICodeVisitor* aVisitor) const override;

  const std::vector<Expr>& getIdentifiers() const;

private:
  std::vector<Expr> mIdentifiers;
};

}
