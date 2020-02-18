
#include "Define.h"
#include "Identifier.h"
#include "NateFunctions.h"

#include <reflex/matcher.h>
#include <iostream>
#include <sstream>
#include <algorithm>

Define::Define()
{
	setPriority(65);
}

std::string Define::createCodeDeclArgs(const std::vector<Arg>& aArgs)
{
	std::stringstream buf;
	bool first = true;
	
	buf << "(";

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

	return buf.str();
}

std::string Define::createCodeDecl(const std::string& aObjectName)
{
	std::stringstream buf;
	
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

	buf << pattern() << createCodeDeclArgs(args());
	
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

std::list<DefinePtr>& Defines::get()
{
	return mDefines;
}

void Defines::add(const DefinePtr& aDefine)
{
	mDefines.push_back(aDefine);
}

DefinePtr Defines::getLike(const DefinePtr& aDefine)
{
	auto iter = std::find_if(mDefines.begin(), mDefines.end(),
													 [&aDefine](const DefinePtr& item)
													 { return aDefine != item &&
													   aDefine->pattern() == item->pattern(); });
	return (iter != mDefines.end()) ? *iter : DefinePtr();
}