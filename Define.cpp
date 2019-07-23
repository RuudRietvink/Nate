
#include "Define.h"
#include "Identifier.h"
#include "NateFunctions.h"

#include <reflex/matcher.h>
#include <iostream>
#include <sstream>

Define::Define()
{
	setPriority(150);
}

const std::string&      Define::codeCall()  const { return mCodeCall; }
std::string&            Define::codeCall()        { return mCodeCall; }

std::string Define::createCodeDecl()
{
	std::stringstream buf;
	bool first = true;

	if (is(None))
	{
		buf << "void ";
	}
	else
	{
		buf << type().codeType() << " ";
	}

	buf << pattern() << "(";

	for (auto const& arg : args())
	{
		if (arg.isIdentifier())
		{
			if (!first)
			{
				buf << ", ";
			}

			first = false;

			if (arg.identifier().type().is(Type::String) && !arg.is(Arg::Out))
			{
				buf << "const ";
			}

			buf << arg.identifier().type().codeType();

			if (arg.is(Arg::Out))
			{
				buf << "&";
			}

			buf << " " << arg.identifier().codeName();
		}
	}

	buf << ")";

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
			if (!first)
			{
				buf << ", ";
			}
			first = false;

			buf << "${" << arg.identifier().name() << "}";
		}
	}
		
	buf << ")";

	mCodeCall = buf.str(); 
}

std::tuple<std::string, std::string, Type> Define::evaluate(ExprNodesCIter& aBegin, ExprNodesCIter& aEnd) const
{
	std::string error;
	std::string codeCall = mCodeCall;
	ExprNodesCIter nodeIter = aBegin;
	Type codeType(type());
	Type firstType;

	for (auto const& arg : args())
	{
		if (arg.isIdentifier())
		{
			const Type& nodeType = nodeIter->type();

			if (firstType.is(Type::Unknown))
			{
				firstType = nodeType;
			}

      ExprNode node = *nodeIter;
      node.castToType(arg.identifier().type());
			codeCall = replaceAll(codeCall, "${" + arg.identifier().name() + "}", 
                            "(" + node.code() + ")");

			if (arg.is(Arg::Num) && type().is(Type::Unknown) && nodeType.isBiggerThan(codeType))
			{
				codeType = nodeType;
				//std::cerr << arg.identifier().name() << " " << codeType << std::endl;
			}
		}

		++nodeIter;
	}

	if (is(Same))
	{
		codeType = firstType;
	}

	return std::make_tuple(error, codeCall, codeType);
}

std::ostream& operator<<(std::ostream& aStream, const Define& aValue)
{
	aStream << "Define(";
  operator<<(aStream, static_cast<const Method&>(aValue));
  aStream << ","
		      << aValue.codeCall() << ")";
	return aStream;
}