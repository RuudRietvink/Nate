#pragma once

#include "Stat.h"
#include "StatExpr.h"
#include "Identifier.h"
#include "Expr.h"

class StatDeclareLocal : public Stat
{
public:
  StatDeclareLocal(const Location& aLocation, const IdentifierPtr& aIdentifier, const Expr& aValue);
  
  void accept(ICodeVisitor* aVisitor) const override;

  const IdentifierPtr& getIdentifier() const;

  const Expr& getValue() const;

private:
  IdentifierPtr mIdentifier;
  Expr          mValue;  
};

