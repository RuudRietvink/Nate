
#include "Define.h"
#include "Identifier.h"
#include "NateFunctions.h"

#include <reflex/matcher.h>
#include <iostream>
#include <sstream>

Define::Define()
{
	setPriority(65);
}

std::string Define::createCodeDecl(const std::string& aObjectName)
{
	std::stringstream buf;
	bool first = true;
	
	if (is(None))
	{
		buf << "void ";
	}
	else
	{
		buf << type()->codeType() << " ";
	}

	if (!aObjectName.empty())
	{
		buf << aObjectName << "::";
	}

	buf << pattern() << "(";

	for (auto const& arg : args())
	{
		if (arg.isIdentifier() && !arg.identifier()->isObjectMe())
		{
			if (!first)
			{
				buf << ", ";
			}

			first = false;

			if (arg.identifier()->type()->is(Type::NeedsRef) && !arg.is(Arg::Out))
			{
				buf << "const ";
			}

			buf << arg.identifier()->type()->codeType();

			if (arg.identifier()->type()->is(Type::NeedsRef) || arg.is(Arg::Out))
			{
				buf << "&";
			}

			buf << " " << arg.identifier()->codeName();
		}
	}

	buf << ")";

	if (is(ConstMethod))
	{
		buf << " const";
	}

	return buf.str();
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