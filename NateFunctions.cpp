#include "NateFunctions.h"

#include "core/Core.h"
#include "utf8.h"

#include <vector>
#include <inttypes.h>
#include <iostream>
#include <sstream>
#include <iomanip>

std::string unquote(const std::string& aString)
{
	return aString.substr(1, aString.size() - 2);
}

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

std::tuple<std::string, std::string> fromMonomial(const std::string& aString)
{
	std::string number;
	std::string word;
	
	utf8::iterator<std::string::const_iterator> iter(Core::cbegin(aString));
	utf8::iterator<std::string::const_iterator> end(Core::cend(aString));
	auto inserter = std::back_inserter(number);

	while (iter != end && *iter < 255 && std::isdigit(*iter))
	{
		inserter = utf8::append(*iter++, inserter);
	}

	if (iter != end && *iter == '.')
	{
		inserter = utf8::append(*iter++, inserter);

		while (iter != end && *iter < 255 && std::isdigit(*iter))
		{
			inserter = utf8::append(*iter++, inserter);
		}

		if (iter != end && *iter == 'E')
		{
			inserter = utf8::append(*iter++, inserter);

			if (iter != end && (*iter == '-' || *iter == '+'))
			{
				inserter = utf8::append(*iter++, inserter);
			}

			while (iter != end && *iter < 255 && std::isdigit(*iter))
			{
				inserter = utf8::append(*iter++, inserter);
			}
		}
	}
	
	inserter = std::back_inserter(word);
	while (iter != end)
	{
		inserter = utf8::append(*iter++, inserter);
	}

	return std::make_tuple(number, word);
}