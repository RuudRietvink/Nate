#pragma once

#include <bitset>

class WithFlags
{
public:
	bool is(size_t aFlags) const;
	void setFlag(size_t aFlag, bool aEnable);

private:
	std::bitset<32> mFlags;
};

