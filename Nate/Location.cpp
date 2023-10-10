#include "Location.h"

Location::Location(const yy::location& aLocation, const std::string& aCurFile)
	: filename(aLocation.begin.filename != nullptr ? *aLocation.begin.filename : aCurFile),
		beginLine(aLocation.begin.line),
		beginColumn(aLocation.end.column)
{
}