#pragma once

#include "location.hpp"

#include <string>

struct Location
{
	Location()
	{}

	Location(const yy::location& aLocation, const std::string& aCurFile);

	std::string filename;
	int beginLine = -1;
	int beginColumn = -1;
};

