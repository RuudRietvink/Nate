#pragma once

#include "Stat.h"
#include "Define.h"

namespace nate
{
class StatDefine : public Stat
{
public:
    StatDefine(const Location& aLocation, const DefinePtr& aDefine, bool aImpOnly, bool aIsDecl);

    const DefinePtr& getDefine() const;

    bool isImpOnly() const;

    bool isDecl() const;

    void accept(ICodeVisitor* aVisitor) const override;

private:
    DefinePtr mDefine;
    bool mImpOnly;
    bool mIsDecl;
};

}
