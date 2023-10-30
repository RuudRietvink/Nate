#pragma once

#include "Stat.h"
#include "StatWithExpr.h"

class StatInput : public Stat
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

  class Value : public StatWithExpr
  {
  public:
    Value(const Location& aLocation, const Expr& aExpr);

    void accept(ICodeVisitor* aVisitor) const override;
  };

  StatInput(const Location& aLocation);

  void accept(ICodeVisitor* aVisitor) const override;
};
