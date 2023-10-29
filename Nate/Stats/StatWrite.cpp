#include "StatWrite.h"
#include "ICodeVisitor.h"

StatWrite::StatWrite(const Location& aLocation, const IdentifierPtr& aWriter, const Expr& aOutput, bool aCreateIt)
  : StatOutput(aLocation),
    mWriter(aWriter),
    mOutput(aOutput),
    mCreateIt(aCreateIt)
{}

const IdentifierPtr& StatWrite::getWriter() const
{
  return mWriter;
}

const Expr& StatWrite::getOutput() const
{
  return mOutput;
}

bool StatWrite::getCreateIt() const
{
  return mCreateIt;
}

void StatWrite::accept(ICodeVisitor* aVisitor, const std::vector<bool>& aFlags) const
{
  aVisitor->visit(*this, aFlags);
}
