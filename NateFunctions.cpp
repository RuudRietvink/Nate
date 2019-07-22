#include "NateFunctions.h"
#include <vector>
#include <inttypes.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "utf8.h"

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
	utf8::iterator<std::string::const_iterator> iter(aName.cbegin(), aName.cbegin(), aName.cend());
	utf8::iterator<std::string::const_iterator> end(aName.cend(), aName.cbegin(), aName.cend());

	//std::cerr << aName << " ";
	for (; iter != end; ++iter)
	{
		if (*iter == '-')
		{
			buf << '_';
		}
		else if (*iter > 255)
		{
			buf << "U" << std::hex << *iter;
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