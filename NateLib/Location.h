#pragma once

#include "location.hpp"

#include <string>

namespace nate
{
struct Location
{
	Location()
	{}

	Location(const nate::location& aLocation, const std::string& aCurFile);

	std::string filename;
	int beginLine = -1;
	int beginColumn = -1;
};

}
