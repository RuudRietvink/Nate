#include "Core.h"

#include <algorithm>
#include <locale>
#include <cctype>

namespace
{
	typedef std::string::value_type char_t;
	char_t upcase(char_t ch)
	{
	  return std::use_facet<std::ctype< char_t>>(std::locale()).toupper(ch);
	}
}

std::string Core::upperCased(const std::string& aString)
{
 std::string result;
 std::transform(aString.begin(), aString.end(), std::back_inserter(result), upcase);
 return result;
}

std::string Core::firstOf(const std::string& aString)
{
 return aString.empty() ? "" : aString.substr(0, 1);
}

std::string Core::restOf(const std::string& aString)
{
 return aString.size() <= 1 ? "" : aString.substr(1);
}