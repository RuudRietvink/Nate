#pragma once

#include <string>

namespace nate
{
class location;

struct Location
{
	Location()
	{}

	Location(const location& aLocation, const std::string& aCurFile);

	std::string filename;
	int beginLine = -1;
	int beginColumn = -1;
};

}
