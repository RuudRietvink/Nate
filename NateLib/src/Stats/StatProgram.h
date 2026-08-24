#pragma once

#include "Stat.h"

namespace nate
{
class StatProgram : public Stat
{
public:
    StatProgram(const Location& aLocation);

    void accept(ICodeVisitor* aVisitor) const override;
};

}
