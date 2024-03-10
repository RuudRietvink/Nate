#include "NateFunctions.h"

#include "core/Core.h"
#include "utf8.h"

#include <vector>
#include <inttypes.h>
#include <iostream>
#include <sstream>
#include <iomanip>

namespace nate
{
std::string unquote(const std::string& aString)
{
	return aString.substr(1, aString.size() - 2);
}

std::string toCodeName(const std::string& aName)
{
	std::stringstream buf;
	Core::Utf8 name(aName);

	//std::cerr << aName << " ";
	for (; name; ++name)
	{
		if (*name == '-')
		{
			buf << '_';
		}
		else if (*name == '\'')
		{
			buf << "__qt_";
		}
		else if (*name > 255)
		{
			buf << "__" << std::hex << *name << "_";
		}
		else
		{
			buf << static_cast<char>(*name);
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
	Core::Utf8 string(aString);
	
	auto inserter = std::back_inserter(number);

	while (string && *string < 255 && std::isdigit(*string))
	{
		inserter = utf8::append(*string++, inserter);
	}

	if (string && *string == '.')
	{
		inserter = utf8::append(*string++, inserter);

		while (string && *string < 255 && std::isdigit(*string))
		{
			inserter = utf8::append(*string++, inserter);
		}

		if (string && *string == 'E')
		{
			inserter = utf8::append(*string++, inserter);

			if (string && (*string == '-' || *string == '+'))
			{
				inserter = utf8::append(*string++, inserter);
			}

			while (string && *string < 255 && std::isdigit(*string))
			{
				inserter = utf8::append(*string++, inserter);
			}
		}
	}
	
	inserter = std::back_inserter(word);
	while (string)
	{
		inserter = utf8::append(*string++, inserter);
	}

	return std::make_tuple(number, word);
}

}
