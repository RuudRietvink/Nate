#include "NateFunctions.h"
#include <vector>
#include <inttypes.h>
#include <iostream>

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
	std::string result;
	for (auto const& kar : aName)
	{
		if (kar == '-')
		{
			result += '_';
		}
		else
		{
			result += kar;
		}
	}

	return result;
}

extern std::string escapedMatch(const std::string& aString)
{
	std::string result;

	for (auto const& kar : aString)
	{
		if (strchr(".\\?*+|()[]{}^$\"/", kar) != nullptr)
		{
			result += '\\';
		}

		result += kar;
	}

	return result;
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