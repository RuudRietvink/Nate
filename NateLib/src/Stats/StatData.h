#pragma once

#include "StatOutput.h"

namespace nate
{
class StatData : public StatOutput
{
public:
    StatData(const Location& aLocation, const IdentifierPtr& anId);

    void accept(ICodeVisitor* aVisitor) const override;

    const IdentifierPtr& getId() const;
  
private:
    IdentifierPtr mId;
};

}
