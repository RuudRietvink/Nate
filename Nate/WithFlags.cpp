#include "WithFlags.h"

namespace nate
{
bool WithFlags::is(size_t aFlag) const
{ 
	auto iter = mFlags.find(aFlag);
	return iter != mFlags.cend() ? iter->second : false;
}

void WithFlags::setFlag(size_t aFlag)
{ 
	mFlags[aFlag] = true;
}

void WithFlags::clearFlag(size_t aFlag)
{ 
	mFlags[aFlag] = false;
}

void WithFlags::setFlag(size_t aFlag, bool aEnable)
{ 
	mFlags[aFlag] = aEnable;
}
	
std::string WithFlags::setFlagStrings(const std::vector<std::string>& aFlags)
{
	std::string result;

	for (auto& flag : aFlags)
	{
		result.append(setFlagString(flag));
	}

	return result;
}

void WithFlags::setFlags(const Flags& aFlags)
{
	mFlags = aFlags;
}

const Flags& WithFlags::getFlags() const
{
	return mFlags;
}

}
