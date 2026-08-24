#include "StatInput.h"
#include "StatExpr.h"
#include "ICodeVisitor.h"

namespace nate
{
StatInput::StatInput(const Location& aLocation)
  : Stat(aLocation)
{}

void StatInput::accept(ICodeVisitor* aVisitor) const
{
    aVisitor->visit(*this);
}

StatInput::Comma::Comma(const Location& aLocation)
  : Stat(aLocation)
{}

void StatInput::Comma::accept(ICodeVisitor* aVisitor) const
{
    aVisitor->visit(*this);
}

StatInput::Concat::Concat(const Location& aLocation)
  : Stat(aLocation)
{}

void StatInput::Concat::accept(ICodeVisitor* aVisitor) const
{
    aVisitor->visit(*this);
}

StatInput::End::End(const Location& aLocation, bool aEndOfLine)
  : Stat(aLocation),
    mEndOfLine(aEndOfLine)
{}

bool StatInput::End::getEndOfLine() const
{
    return mEndOfLine;
}

void StatInput::End::accept(ICodeVisitor* aVisitor) const
{
    aVisitor->visit(*this);
}

StatInput::Value::Value(const Location& aLocation, const Expr& aExpr)
  : StatWithExpr(aLocation, aExpr)
{}

void StatInput::Value::accept(ICodeVisitor* aVisitor) const
{
    aVisitor->visit(*this);
}
}
