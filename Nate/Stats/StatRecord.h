#pragma once

#include "Stat.h"
#include "Record.h"

class StatRecord : public Stat
{
public:
  StatRecord(const Location& aLocation, const RecordPtr& aRecord);

  const RecordPtr& getRecord() const;

  void accept(ICodeVisitor* aVisitor, const std::vector<bool>& aFlags) const override;

private:
  RecordPtr mRecord;
};

