#pragma once

#include "StatWithExpr.h"
#include "Expr.h"

class StatIfIs : public StatWithExpr
{
public:
  class IsList : public Stat
  {
  public:
    IsList(const Location& aLocation);
  
    void accept(ICodeVisitor* aVisitor, const std::vector<bool>& aFlags) const override;
  };

  class Is : public StatWithExpr
  {
  public:
    Is(const Location& aLocation, const Expr& aExpr);
  
    void accept(ICodeVisitor* aVisitor, const std::vector<bool>& aFlags) const override;
  };

  StatIfIs(const Location& aLocation, const IdentifierPtr& aId, const Expr& aExpr);
  
  void accept(ICodeVisitor* aVisitor, const std::vector<bool>& aFlags) const override;
  
  const IdentifierPtr& getId() const;

private:
  IdentifierPtr mId; 
};

