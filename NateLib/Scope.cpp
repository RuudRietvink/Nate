#include "Scope.h"
#include <iostream>
#include <algorithm>

namespace nate
{
Scope::Scope(const std::string& aName, ScopeFlag aScopeFlag)
  : mName(aName),
	mRecords(mTypes),
	mScopeFlag(aScopeFlag)
{
}

const std::string& Scope::name() const { return mName; }

}
