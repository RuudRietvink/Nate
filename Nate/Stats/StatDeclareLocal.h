#pragma once

#include "StatWithExpr.h"
#include "Identifier.h"
#include "Expr.h"

namespace nate
{
class StatDeclareLocal : public StatWithExpr
{
public:
  StatDeclareLocal(const Location& aLocation, const IdentifierPtr& aIdentifier, const Expr& aValue);
  
  void accept(ICodeVisitor* aVisitor) const override;

  const IdentifierPtr& getIdentifier() const;

private:
  IdentifierPtr mIdentifier;
};

}
