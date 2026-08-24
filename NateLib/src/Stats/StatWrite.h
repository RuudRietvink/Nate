#pragma once

#include "StatOutput.h"

namespace nate
{
class StatWrite : public StatOutput
{
public:
    StatWrite(const Location& aLocation, const IdentifierPtr& aWriter, const Expr& aOutput, bool aCreateIt);

    void accept(ICodeVisitor* aVisitor) const override;

    const IdentifierPtr& getWriter() const;
  
    const Expr& getOutput() const;

    bool getCreateIt() const;

private:
    IdentifierPtr mWriter;
    Expr mOutput;
    bool mCreateIt;
};

}
