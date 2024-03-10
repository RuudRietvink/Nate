
#include "Define.h"
#include "Identifier.h"
#include "NateFunctions.h"

#include <reflex/matcher.h>
#include <iostream>
#include <sstream>
#include <algorithm>

namespace nate
{
Define::Define()
{
	setPriority(65);
}

void Define::createCodeCall()
{
	std::stringstream buf;
	bool first = true;

	buf << pattern() << "(";

	for (auto const& arg : args())
	{
		if (arg.isIdentifier())
		{
			if (!arg.identifier()->isObjectMe())
			{
				if (!first)
				{
					buf << ", ";
				}
				first = false;

				buf << "${" << arg.identifier()->name() << "}";
			}
		}
	}
		
	buf << ")";

	code() = buf.str(); 
}

std::ostream& operator<<(std::ostream& aStream, const Define& aValue)
{
	aStream << "Define(";
  operator<<(aStream, static_cast<const Method&>(aValue));
	return aStream;
}

std::list<DefinePtr>& Defines::get()
{
	return mDefines;
}

DefinePtr Defines::add(const DefinePtr& aDefine)
{
	mDefines.push_back(aDefine);
	return aDefine;
}

DefinePtr Defines::getLike(const DefinePtr& aDefine)
{
	auto iter = std::find_if(mDefines.begin(), mDefines.end(),
													 [&aDefine](const DefinePtr& item)
													 { 
														 return aDefine != item &&
													   aDefine->pattern() == item->pattern();
													 });

	return (iter != mDefines.end()) ? *iter : DefinePtr();
}
}
