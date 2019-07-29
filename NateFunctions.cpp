#include "NateFunctions.h"
#include <vector>
#include <inttypes.h>
#include <iostream>
#include <sstream>
#include <iomanip>

std::string toCodeName(const std::string& aName)
{
	std::stringstream buf;
	utf8::iterator<std::string::const_iterator> iter(Core::cbegin(aName));
	utf8::iterator<std::string::const_iterator> end(Core::cend(aName));

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
