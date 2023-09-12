#pragma once

#include "Stat.h"
#include "StatExpr.h"


class StatOutput : public Stat
{
public:
  class Comma : public Stat
  {
  public:
    Comma(const Location& aLocation);

    void accept(ICodeVisitor* aVisitor) const override;
  };

  class Concat : public Stat
  {
  public:
    Concat(const Location& aLocation);

    void accept(ICodeVisitor* aVisitor) const override;
  };

  class End : public Stat
  {
  public:
    End(const Location& aLocation, bool aEndOfLine);

    void accept(ICodeVisitor* aVisitor) const override;

    bool getEndOfLine() const;

  private:
    bool mEndOfLine;
  };

  class Expr : public StatExpr
  {
  public:
    Expr(const Location& aLocation, const ::Expr& aExpr);

    void accept(ICodeVisitor* aVisitor) const override;
  };

  StatOutput(const Location& aLocation);

  void accept(ICodeVisitor* aVisitor) const override;
};
