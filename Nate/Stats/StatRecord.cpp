#include "StatRecord.h"
#include "ICodeVisitor.h"

StatRecord::StatRecord(const Location& aLocation, const RecordPtr& aRecord)
  : Stat(aLocation),
    mRecord(aRecord)
{}

void StatRecord::accept(ICodeVisitor* aVisitor) const
{
  aVisitor->visit(*this);
}

const RecordPtr& StatRecord::getRecord() const
{
  return mRecord;
}
