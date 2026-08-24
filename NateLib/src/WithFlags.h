#pragma once

#include "core/cpp/Core.h"

#include <vector>
#include <map>

namespace nate
{
using Flags = std::map<size_t, bool>;

class WithFlags
{
public:
	virtual bool is(size_t aFlag) const;
	virtual std::string setFlagString(const std::string& aFlag) { return ""; };
	void setFlag(size_t aFlag);
	void clearFlag(size_t aFlag);
	void setFlag(size_t aFlag, bool aEnable);
	void setFlags(const Flags& aFlags);
	std::string setFlagStrings(const std::vector<std::string>& aFlags);
	const Flags& getFlags() const;
	
protected:
	RULES_OF_X(WithFlags)

private:
	Flags mFlags;
};

}
