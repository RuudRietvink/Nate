#include "StatLoop.h"
#include "ICodeVisitor.h"

StatLoop::StatLoop(const Location& aLocation)
  : Stat(aLocation)
{}

void StatLoop::accept(ICodeVisitor* aVisitor) const
{
  aVisitor->visit(*this);
}

StatLoop::While::While(const Location& aLocation, const Expr& aExpr)
  : StatExpr(aLocation, aExpr)
{}

void StatLoop::While::accept(ICodeVisitor* aVisitor) const
{
  aVisitor->visit(*this);
}

StatLoop::ForStep::ForStep(const Location& aLocation, const IdentifierPtr& aId, 
						               bool aDownTo,
						               const Expr& aStart, const Expr& aEnd, const Expr& aStep)
  : Stat(aLocation),
    mId(aId),
    mDownTo(aDownTo),
    mStart(aStart),
    mEnd(aEnd),
    mStep(aStep)
{}

const IdentifierPtr& StatLoop::ForStep::getId() const
{
  return mId;
}

bool StatLoop::ForStep::getDownTo() const
{
  return mDownTo;
}

const Expr& StatLoop::ForStep::getStart() const
{
  return mStart;
}

const Expr& StatLoop::ForStep::getEnd() const
{
  return mEnd;
}

const Expr& StatLoop::ForStep::getStep() const
{
  return mStep;
}

void StatLoop::ForStep::accept(ICodeVisitor* aVisitor) const
{
  aVisitor->visit(*this);
}

StatLoop::ForRange::ForRange(const Location& aLocation, const IdentifierPtr& aId, const Expr& aRange)
  : Stat(aLocation),
    mId(aId),
    mRange(aRange)
{}

const IdentifierPtr& StatLoop::ForRange::getId() const
{
  return mId;
}

const Expr& StatLoop::ForRange::getRange() const
{
  return mRange;
}

void StatLoop::ForRange::accept(ICodeVisitor* aVisitor) const
{
  aVisitor->visit(*this);
}