#include "StatOutput.h"
#include "StatExpr.h"
#include "ICodeVisitor.h"

StatOutput::StatOutput(const Location& aLocation)
  : Stat(aLocation)
{}

void StatOutput::accept(ICodeVisitor* aVisitor, const std::vector<bool>& aFlags) const
{
  aVisitor->visit(*this, aFlags);
}

StatOutput::Comma::Comma(const Location& aLocation)
  : Stat(aLocation)
{}

void StatOutput::Comma::accept(ICodeVisitor* aVisitor, const std::vector<bool>& aFlags) const
{
  aVisitor->visit(*this, aFlags);
}

StatOutput::Concat::Concat(const Location& aLocation)
  : Stat(aLocation)
{}

void StatOutput::Concat::accept(ICodeVisitor* aVisitor, const std::vector<bool>& aFlags) const
{
  aVisitor->visit(*this, aFlags);
}

StatOutput::End::End(const Location& aLocation, bool aEndOfLine)
  : Stat(aLocation),
    mEndOfLine(aEndOfLine)
{}

bool StatOutput::End::getEndOfLine() const
{
  return mEndOfLine;
}

void StatOutput::End::accept(ICodeVisitor* aVisitor, const std::vector<bool>& aFlags) const
{
  aVisitor->visit(*this, aFlags);
}

StatOutput::Value::Value(const Location& aLocation, const Expr& aExpr)
  : StatWithExpr(aLocation, aExpr)
{}

void StatOutput::Value::accept(ICodeVisitor* aVisitor, const std::vector<bool>& aFlags) const
{
  aVisitor->visit(*this, aFlags);
}