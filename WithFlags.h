#pragma once

#include <bitset>
#include <vector>

using Flags = std::vector<int>;

class WithFlags
{
public:
	virtual bool is(size_t aFlags) const;
	virtual std::string setFlagString(const std::string& aFlag) { return ""; };
	void setFlag(size_t aFlag);
	void clearFlag(size_t aFlag);
	void setFlag(size_t aFlag, bool aEnable);
	void setFlags(const Flags& aFlags);
	std::string setFlagStrings(const std::vector<std::string>& aFlags);

protected:
	void setFlags(const std::bitset<32>& aFlags);
	const std::bitset<32>& getFlags() const;

private:
	std::bitset<32> mFlags;
};

