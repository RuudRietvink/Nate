#include "Scope.h"
#include <iostream>
#include <algorithm>

Scope::Scope(const std::string& aName)
	: mName(aName),
	  mRecords(mTypes)
{
}

const std::string& Scope::name() const { return mName; }
