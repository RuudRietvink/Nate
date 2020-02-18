#include "WithFlags.h"

bool WithFlags::is(size_t aFlags) const
{ 
	return mFlags[aFlags];
}

void WithFlags::setFlag(size_t aFlag)
{ 
	mFlags.set(aFlag, true);
}

void WithFlags::clearFlag(size_t aFlag)
{ 
	mFlags.set(aFlag, false);
}

void WithFlags::setFlag(size_t aFlag, bool aEnable)
{ 
	mFlags.set(aFlag, aEnable);
}

void WithFlags::setFlags(const Flags& aFlags)
{
	for (auto flag : aFlags)
	{
		setFlag(flag);
	}
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

void WithFlags::setFlags(const std::bitset<32>& aFlags)
{
	mFlags = aFlags;
}

const std::bitset<32>& WithFlags::getFlags() const
{
	return mFlags;
}