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
  
    void accept(ICodeVisitor* aVisitor) const override;
  };

  class Is : public StatWithExpr
  {
  public:
    Is(const Location& aLocation, const Expr& aExpr);
  
    void accept(ICodeVisitor* aVisitor) const override;
  };
  
  class Else : public Stat
  {
  public:
    Else(const Location& aLocation);
  
    void accept(ICodeVisitor* aVisitor) const override;
  };

  StatIfIs(const Location& aLocation, const IdentifierPtr& aId, const Expr& aExpr);
  
  void accept(ICodeVisitor* aVisitor) const override;
  
  const IdentifierPtr& getId() const;

private:
  IdentifierPtr mId; 
};

