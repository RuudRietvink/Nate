#pragma once

#include "StatWithExpr.h"
#include "Expr.h"

namespace nate
{
class StatIfThen : public StatWithExpr
{
public:
    class ElseIf : public StatWithExpr
    {
    public:
        ElseIf(const Location& aLocation, const Expr& aExpr);
  
        void accept(ICodeVisitor* aVisitor) const override;
    };
  
    class Else : public Stat
    {
    public:
        Else(const Location& aLocation);
  
        void accept(ICodeVisitor* aVisitor) const override;
    };

    StatIfThen(const Location& aLocation, const Expr& aExpr);
  
    void accept(ICodeVisitor* aVisitor) const override;
};

}
