#pragma once

#include "location.hpp"
#include "NateParser.tab.h"

#include <string>

struct Location
{
	Location()
	{}

	Location(const yy::parser::location_type& aLocation, const std::string& aCurFile)
		: filename(aLocation.begin.filename != nullptr ? *aLocation.begin.filename : aCurFile),
		  beginLine(aLocation.begin.line),
		  beginColumn(aLocation.end.column)
	{
	}

	std::string filename;
	int beginLine = -1;
	int beginColumn = -1;
};

