#pragma once

#include "Core/Core.h"

#include <string>
#include <cstdint>
#include <sstream>

extern std::string toCodeName(const std::string& aName);
extern std::string escapedMatch(const std::string& aString);
extern std::string replaceAll(const std::string& aString, const std::string& aFrom, const std::string& aTo);
extern std::string toCodeWord(const std::string& aWord);

template<typename CONTAINER>
std::string join(const CONTAINER& container, const std::string& seperator = ",")
{
	std::stringstream ss;
	ss << container.size() << ':';
	bool first = true;

	for (auto const& item : container)
	{
		if (!first)
		{
			ss << seperator;
		}

		ss << "(" << item << ")";
		first = false;
	}

	return ss.str();
}
