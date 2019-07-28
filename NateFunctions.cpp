#include "NateFunctions.h"
#include <vector>
#include <inttypes.h>
#include <iostream>
#include <sstream>
#include <iomanip>

std::wstring u16(const std::string& str)
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

bool strtoi64(const char* aString, int64_t& aResult)
{
	char c;
	int scanned = sscanf_s(aString, "%" SCNd64 "%c", &aResult, &c, 1);
	return (scanned == 1);
}

bool strtodbl(const char* aString, double& aResult)
{
	char c;
	int scanned = sscanf_s(aString, "%lG%c", &aResult, &c, 1);
	return (scanned == 1);
}

std::string toCodeName(const std::string& aName)
{
	std::stringstream buf;
	utf8::iterator<std::string::const_iterator> iter(cbegin(aName));
	utf8::iterator<std::string::const_iterator> end(cend(aName));

	//std::cerr << aName << " ";
	for (; iter != end; ++iter)
	{
		if (*iter == '-')
		{
			buf << '_';
		}
		else if (*iter > 255)
		{
			buf << "__" << std::hex << *iter << "_";
		}
		else
		{
			buf << static_cast<char>(*iter);
		}
	}

	//std::cerr << buf.str() << std::endl;
	return buf.str();
}

std::string toCodeWord(const std::string& aWord)
{
	return toCodeName(aWord);
}

extern std::string escapedMatch(const std::string& aString)
{
	std::stringstream buf;

	for (auto const& kar : aString)
	{
		if (strchr(".\\?*+|()[]{}^$\"/", kar) != nullptr)
		{
			buf << '\\';
		}

		buf << kar;
	}

	return buf.str();
}

std::string replaceAll(const std::string& aString, const std::string& aFrom, const std::string& aTo)
{
  std::string result = aString;

  if (!aFrom.empty())
  {
    size_t start = 0;
    while ((start = result.find(aFrom, start)) != std::string::npos)
    {
      result.replace(start, aFrom.length(), aTo);
      start += aTo.length();
    }
  }

  return result;
}

utf8::iterator<std::string::const_iterator> cbegin(const std::string& aString)
{
	return utf8::iterator<std::string::const_iterator>(aString.cbegin(), aString.cbegin(), aString.cend());
}

utf8::iterator<std::string::const_iterator> cend(const std::string& aString)
{
	return utf8::iterator<std::string::const_iterator>(aString.cend(), aString.cbegin(), aString.cend());
}

utf8::iterator<std::string::const_iterator> find(const std::string& aString,
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