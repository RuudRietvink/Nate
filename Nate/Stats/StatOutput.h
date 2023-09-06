#pragma once

#include "Stat.h"
#include "StatExpr.h"


class StatOutput : public Stat
{
public:
  StatOutput(const Location& aLocation);

  void accept(ICodeVisitor* aVisitor) const override;
};

class StatOutputComma : public Stat
{
public:
  StatOutputComma(const Location& aLocation);

  void accept(ICodeVisitor* aVisitor) const override;
};

class StatOutputConcat : public Stat
{
public:
  StatOutputConcat(const Location& aLocation);

  void accept(ICodeVisitor* aVisitor) const override;
};

class StatOutputEnd : public Stat
{
public:
  StatOutputEnd(const Location& aLocation, bool aEndOfLine);

  void accept(ICodeVisitor* aVisitor) const override;

  bool getEndOfLine() const;

private:
  bool mEndOfLine;
};

class StatOutputExpr : public StatExpr
{
public:
  StatOutputExpr(const Location& aLocation, const Expr& aExpr);

  void accept(ICodeVisitor* aVisitor) const override;
};