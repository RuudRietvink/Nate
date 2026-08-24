#pragma once

#include "Stat.h"

namespace nate
{
class StatScope : public Stat
{
public:
    StatScope(const Location& aLocation);

    void accept(ICodeVisitor* aVisitor) const override;
};

}
