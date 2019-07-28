#pragma once

#include "utf8.h"

#define NOMINMAX  
#include <windows.h>
#include <stringapiset.h>
#include <string>
#include <cstdint>
#include <sstream>

extern std::wstring u16(const std::string& str);
extern bool strtoi64(const char* aString, int64_t& aResult);
extern bool strtodbl(const char* aString, double& aResult);
extern std::string toCodeName(const std::string& aName);
extern std::string escapedMatch(const std::string& aString);
extern std::string replaceAll(const std::string& aString, const std::string& aFrom, const std::string& aTo);
extern std::string toCodeWord(const std::string& aWord);
extern utf8::iterator<std::string::const_iterator> cbegin(const std::string& aString);
extern utf8::iterator<std::string::const_iterator> cend(const std::string& aString);
extern utf8::iterator<std::string::const_iterator> find(const std::string& aString,
																											  uint32_t aChar);

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
