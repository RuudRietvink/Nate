#include "StatOutput.h"
#include "StatExpr.h"


StatOutput::StatOutput(const Location& aLocation)
  : Stat(aLocation)
{}

void StatOutput::accept(ICodeVisitor* aVisitor) const
{
  aVisitor->visit(*this);
}

StatOutputComma::StatOutputComma(const Location& aLocation)
  : Stat(aLocation)
{}

void StatOutputComma::accept(ICodeVisitor* aVisitor) const
{
  aVisitor->visit(*this);
}

StatOutputConcat::StatOutputConcat(const Location& aLocation)
  : Stat(aLocation)
{}

void StatOutputConcat::accept(ICodeVisitor* aVisitor) const
{
  aVisitor->visit(*this);
}

StatOutputEnd::StatOutputEnd(const Location& aLocation, bool aEndOfLine)
  : Stat(aLocation),
    mEndOfLine(aEndOfLine)
{}

bool StatOutputEnd::getEndOfLine() const
{
  return mEndOfLine;
}

void StatOutputEnd::accept(ICodeVisitor* aVisitor) const
{
  aVisitor->visit(*this);
}

StatOutputExpr::StatOutputExpr(const Location& aLocation, const Expr& aExpr)
  : StatExpr(aLocation, aExpr)
{}

void StatOutputExpr::accept(ICodeVisitor* aVisitor) const
{
  aVisitor->visit(*this);
}