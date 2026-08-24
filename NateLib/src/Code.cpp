
#include "Code.h"
#include "Identifier.h"
#include "NateFunctions.h"

#include <reflex/matcher.h>
#include <iostream>

namespace nate
{
Code::Code()
{}

void Code::addCodeStatWord(const std::string& aWord)
{
	code() += aWord;
}

void Code::addCodeStatId(const std::string& aId)
{
	code() += "${" + aId + "}";
}

std::string Code::toCodeWord(const std::string& aWord) const
{
	return aWord;
}

bool Code::equals(const Code& aCode) const
{
	return pattern() == aCode.pattern();
}

std::ostream& operator<<(std::ostream& aStream, const Code& aValue)
{
	aStream << "Code(";
	operator<<(aStream, static_cast<const Method&>(aValue));
	aStream << ","
		    << aValue.priority() << ")";
	return aStream;
}
}
