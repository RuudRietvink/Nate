#include "Scope.h"
#include <iostream>
#include <algorithm>

Scope::Scope(const std::string& aName, ScopeFlag aScopeFlag)
	: mName(aName),
	  mRecords(mTypes),
	  mScopeFlag(aScopeFlag)
{
}

const std::string& Scope::name() const { return mName; }
