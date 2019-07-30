#pragma once

#include "../utf8.h"
#include "Fraction.h"
#include <string>
#include <codecvt>

class Core
{
public:
	static std::wstring u16(const std::string& str);
	static utf8::iterator<std::string::const_iterator> cbegin(const std::string& aString);
	static utf8::iterator<std::string::const_iterator> cend(const std::string& aString);
	static utf8::iterator<std::string::const_iterator> find(const std::string& aString,
																											    uint32_t aChar);

	static std::string upperCased(const std::string& aString);
	static std::string firstOf(const std::string& aString);
	static std::string restOf(const std::string& aString);

	static bool strtoi32(const char* aString, int32_t& aResult);
	static bool strtoi64(const char* aString, int64_t& aResult);
	static bool strtodbl(const char* aString, double& aResult);
	static int positionIn(const std::string& aString, uint32_t aChar);
	static bool numberFrom(utf8::iterator<std::string::const_iterator>& aIter,
											 	 const utf8::iterator<std::string::const_iterator>& aEnd,
												 const std::string& aDigits,
												 int32_t& aNumber);
	static bool numberFrom(const std::string& aString,
												 const std::string& aDigits,
												 int32_t& aNumber);
	static std::string unSuperscript(const std::string& aString);
};

