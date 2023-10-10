#pragma once

#include "Stat.h"
#include "Define.h"

class StatDefine : public Stat
{
public:
  StatDefine(const Location& aLocation, const DefinePtr& aDefine, bool aIsDecl);

  const DefinePtr& getDefine() const;

  bool isDecl() const;

  void accept(ICodeVisitor* aVisitor) const override;

private:
  DefinePtr mDefine;
  bool mIsDecl;
};

