#pragma once

#include "StatInput.h"
#include "NateData.h"

namespace nate
{
class StatRead : public StatInput
{
public:
    StatRead(const Location& aLocation, InputType aInputType, const IdentifierPtr& aReader, const Expr& aInput, bool aCreateIt);

    void accept(ICodeVisitor* aVisitor) const override;
  
    const IdentifierPtr& getReader() const;

    const InputType& getInputType() const;
  
    const Expr& getInput() const;

    bool getCreateIt() const;

private:
    InputType mInputType;
    IdentifierPtr mReader;
    Expr mInput;
    bool mCreateIt;
};

}
