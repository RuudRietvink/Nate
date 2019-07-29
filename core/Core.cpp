#include "Core.h"
#include "../utf8.h"
#include "UpperLower.h"

#define NOMINMAX  
#include <windows.h>
#include <stringapiset.h>
#include <algorithm>
#include <locale>
#include <cctype>
#include <vector>
#include <iostream>

namespace
{
	uint32_t upcase(uint32_t aChar)
	{
		uint32_t result = aChar;
	  auto iter = std::find(lowers.cbegin(), lowers.cend(), aChar);
		if (iter != lowers.cend())
		{
			result = uppers[std::distance(lowers.cbegin(), iter)];
		}

		return result;
	}
}

std::wstring Core::u16(const std::string& str)
{
	std::wstring convertedString;
	int requiredSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, 0, 0);
	if (requiredSize > 0)
	{
		std::vector<wchar_t> buffer(requiredSize);
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &buffer[0], requiredSize);
		convertedString.assign(buffer.begin(), buffer.end() - 1);
	}

	return convertedString;
}

std::string Core::upperCased(const std::string& aString)
{
  std::string result;
	auto inserter = std::back_inserter(result);
  for (auto iter = cbegin(aString); iter != cend(aString); ++iter)
	{
		inserter = utf8::append(upcase(*iter), inserter);
	}
  return result;
}

std::string Core::firstOf(const std::string& aString)
{
	std::string result;

	if (!aString.empty())
	{
		std::string::const_iterator iter = aString.cbegin();
		utf8::next(iter, aString.cend());
		result = std::string(aString.cbegin(), iter);
	}

	return result;
}

std::string Core::restOf(const std::string& aString)
{
	std::string result;

	if (!aString.empty())
	{
		std::string::const_iterator iter = aString.cbegin();
		utf8::next(iter, aString.cend());
		result =  std::string(iter, aString.cend());
	}

	return result;
}

utf8::iterator<std::string::const_iterator> Core::cbegin(const std::string& aString)
{
	return utf8::iterator<std::string::const_iterator>(aString.cbegin(), aString.cbegin(), aString.cend());
}

utf8::iterator<std::string::const_iterator> Core::cend(const std::string& aString)
{
	return utf8::iterator<std::string::const_iterator>(aString.cend(), aString.cbegin(), aString.cend());
}

utf8::iterator<std::string::const_iterator> Core::find(const std::string& aString,
																								       uint32_t aChar)
{
	utf8::iterator<std::string::const_iterator> iter(cbegin(aString));
	utf8::iterator<std::string::const_iterator> end(cend(aString));
		
	for (; iter != end; ++iter)
	{
		if (*iter == aChar)
		{
			return iter;
		}
	}

	return end;
}

bool Core::strtoi32(const char* aString, int32_t& aResult)
{
	char c;
	int64_t temp;
	int scanned = sscanf_s(aString, "%" SCNd64 "%c", &temp, &c, 1);
	bool ok = (scanned == 1);
	if (temp > std::numeric_limits<int32_t>::max() || temp < std::numeric_limits<int32_t>::lowest())
	{
		ok = false;
	}
	else
	{
		aResult = static_cast<int32_t>(temp);
	}
		
	return ok;
}

bool Core::strtoi64(const char* aString, int64_t& aResult)
{
	char c;
	int scanned = sscanf_s(aString, "%" SCNd64 "%c", &aResult, &c, 1);
	return (scanned == 1);
}

bool Core::strtodbl(const char* aString, double& aResult)
{
	char c;
	int scanned = sscanf_s(aString, "%lG%c", &aResult, &c, 1);
	return (scanned == 1);
}

int Core::positionIn(const std::string& aString, uint32_t aChar)
{
	int result = -1;

	auto iter = Core::find(aString, aChar);
	if (iter != Core::cend(aString))
	{
		result = static_cast<int>(std::distance(Core::cbegin(aString), iter));
	}

	return result;
}

bool Core::numberFrom(utf8::iterator<std::string::const_iterator>& aIter,
											const utf8::iterator<std::string::const_iterator>& aEnd,
											const std::string& aDigits,
											int32_t& aNumber)
{
	bool ok = false;
	int pos = 0;
	aNumber = 0;
	std::string buf;

	while (aIter != aEnd && pos >= 0)
	{
		pos = positionIn(aDigits, *aIter);
		if (pos >= 0) 
		{
			buf.push_back('0' + pos);
			++aIter;
		}
	}

	if (!buf.empty())
	{
		ok = strtoi32(buf.c_str(), aNumber);
	}

	return ok;
}

bool Core::numberFrom(const std::string& aString,
											const std::string& aDigits,
											int32_t& aNumber)
{
	auto iter = cbegin(aString);
	return numberFrom(iter, cend(aString), aDigits, aNumber);
}

std::string Core::unSuperscript(const std::string& aString)
{
	std::string result;
	static const std::string superScript = "⁻⁰¹²³⁴⁵⁶⁷⁸⁹ᴬᴮᴰᴱᴳᴴᴵᴶᴷᴸᴹᴺᴼᴾᴿᵀᵁⱽᵂᵃᵇᶜᵈᵉᶠᵍʰⁱʲᵏˡᵐⁿᵒᵖʳˢᵗᵘᵛʷˣʸᶻ";
	static const std::string normlScript = "-0123456789ABDEGHIJKLMNOPRTUVWabcdefghijklmnoprstuvwxyz";
	
	utf8::iterator<std::string::const_iterator> iter(cbegin(aString));
	utf8::iterator<std::string::const_iterator> end(cend(aString));
		
	for (; iter != end; ++iter)
	{
		auto pos = positionIn(superScript, *iter);
		if (pos >= 0)
		{
			result.push_back(normlScript[pos]);
		}
	}

	std::cerr << aString << " -> " << result << std::endl; 
	return result;
}