#include "StatRead.h"
#include "ICodeVisitor.h"

StatRead::StatRead(const Location& aLocation, InputType aInputType, const IdentifierPtr& aReader, const Expr& aInput, bool aCreateIt)
  : StatInput(aLocation),
    mInputType(aInputType),
    mReader(aReader),
    mInput(aInput),
    mCreateIt(aCreateIt)
{}

const IdentifierPtr& StatRead::getReader() const
{
  return mReader;
}

const InputType& StatRead::getInputType() const
{
  return mInputType;
}

const Expr& StatRead::getInput() const
{
  return mInput;
}

bool StatRead::getCreateIt() const
{
  return mCreateIt;
}

void StatRead::accept(ICodeVisitor* aVisitor) const
{
  aVisitor->visit(*this);
}
