#include "StatInput.h"
#include "StatExpr.h"
#include "ICodeVisitor.h"

StatInput::StatInput(const Location& aLocation)
  : Stat(aLocation)
{}

void StatInput::accept(ICodeVisitor* aVisitor, const std::vector<bool>& aFlags) const
{
  aVisitor->visit(*this, aFlags);
}

StatInput::Comma::Comma(const Location& aLocation)
  : Stat(aLocation)
{}

void StatInput::Comma::accept(ICodeVisitor* aVisitor, const std::vector<bool>& aFlags) const
{
  aVisitor->visit(*this, aFlags);
}

StatInput::Concat::Concat(const Location& aLocation)
  : Stat(aLocation)
{}

void StatInput::Concat::accept(ICodeVisitor* aVisitor, const std::vector<bool>& aFlags) const
{
  aVisitor->visit(*this, aFlags);
}

StatInput::End::End(const Location& aLocation, bool aEndOfLine)
  : Stat(aLocation),
    mEndOfLine(aEndOfLine)
{}

bool StatInput::End::getEndOfLine() const
{
  return mEndOfLine;
}

void StatInput::End::accept(ICodeVisitor* aVisitor, const std::vector<bool>& aFlags) const
{
  aVisitor->visit(*this, aFlags);
}

StatInput::Value::Value(const Location& aLocation, const Expr& aExpr)
  : StatWithExpr(aLocation, aExpr)
{}

void StatInput::Value::accept(ICodeVisitor* aVisitor, const std::vector<bool>& aFlags) const
{
  aVisitor->visit(*this, aFlags);
}