#pragma once

#include "Stat.h"
#include "Record.h"

namespace nate
{
class StatRecord : public Stat
{
public:
  StatRecord(const Location& aLocation, const RecordPtr& aRecord);

  const RecordPtr& getRecord() const;

  void accept(ICodeVisitor* aVisitor) const override;

private:
  RecordPtr mRecord;
};

}
