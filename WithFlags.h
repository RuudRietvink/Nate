#pragma once

#include <bitset>
#include <vector>

using Flags = std::vector<int>;

class WithFlags
{
public:
	bool is(size_t aFlags) const;
	void setFlag(size_t aFlag);
	void clearFlag(size_t aFlag);
	void setFlag(size_t aFlag, bool aEnable);
	void setFlags(const Flags& aFlags);

private:
	std::bitset<32> mFlags;
};

