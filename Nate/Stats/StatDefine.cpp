#include "StatDefine.h"
#include "ICodeVisitor.h"

StatDefine::StatDefine(const Location& aLocation, const DefinePtr& aDefine, bool aIsDecl)
  : Stat(aLocation),
    mDefine(aDefine),
    mIsDecl(aIsDecl)
{}

void StatDefine::accept(ICodeVisitor* aVisitor) const
{
  aVisitor->visit(*this);
}

const DefinePtr& StatDefine::getDefine() const
{
  return mDefine;
}

bool StatDefine::isDecl() const
{
  return mIsDecl;
}