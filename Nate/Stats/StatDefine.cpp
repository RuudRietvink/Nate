#include "StatDefine.h"
#include "ICodeVisitor.h"

StatDefine::StatDefine(const Location& aLocation, const DefinePtr& aDefine, bool aImpOnly, bool aIsDecl)
  : Stat(aLocation),
    mDefine(aDefine),
    mImpOnly(aImpOnly),
    mIsDecl(aIsDecl)
{}

void StatDefine::accept(ICodeVisitor* aVisitor, const std::vector<bool>& aFlags) const
{
  aVisitor->visit(*this, aFlags);
}

const DefinePtr& StatDefine::getDefine() const
{
  return mDefine;
}

bool StatDefine::isImpOnly() const
{
  return mImpOnly;
}

bool StatDefine::isDecl() const
{
  return mIsDecl;
}
