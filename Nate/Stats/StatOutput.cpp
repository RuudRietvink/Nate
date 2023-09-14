#include "StatOutput.h"
#include "StatExpr.h"
#include "ICodeVisitor.h"

StatOutput::StatOutput(const Location& aLocation)
  : Stat(aLocation)
{}

void StatOutput::accept(ICodeVisitor* aVisitor) const
{
  aVisitor->visit(*this);
}

StatOutput::Comma::Comma(const Location& aLocation)
  : Stat(aLocation)
{}

void StatOutput::Comma::accept(ICodeVisitor* aVisitor) const
{
  aVisitor->visit(*this);
}

StatOutput::Concat::Concat(const Location& aLocation)
  : Stat(aLocation)
{}

void StatOutput::Concat::accept(ICodeVisitor* aVisitor) const
{
  aVisitor->visit(*this);
}

StatOutput::End::End(const Location& aLocation, bool aEndOfLine)
  : Stat(aLocation),
    mEndOfLine(aEndOfLine)
{}

bool StatOutput::End::getEndOfLine() const
{
  return mEndOfLine;
}

void StatOutput::End::accept(ICodeVisitor* aVisitor) const
{
  aVisitor->visit(*this);
}

StatOutput::Expr::Expr(const Location& aLocation, const ::Expr& aExpr)
  : StatWithExpr(aLocation, aExpr)
{}

void StatOutput::Expr::accept(ICodeVisitor* aVisitor) const
{
  aVisitor->visit(*this);
}