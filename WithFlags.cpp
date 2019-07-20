#include "WithFlags.h"

bool WithFlags::is(size_t aFlags) const
{ 
	return mFlags[aFlags];
}

void WithFlags::setFlag(size_t aFlag, bool aEnable)
{ 
	mFlags.set(aFlag, aEnable);
}