
#include "Code.h"
#include "Identifier.h"
#include "NateFunctions.h"

#include <reflex/matcher.h>
#include <iostream>

Code::Code()
{}

const std::string&      Code::codeStat()  const { return mCodeStat; }
std::string&            Code::codeStat()        { return mCodeStat; }

void Code::addCodeStatWord(const std::string& aWord)
{
	mCodeStat += aWord;
}

void Code::addCodeStatId(const std::string& aId)
{
	mCodeStat += "${" + aId + "}";
}

std::string Code::toCodeWord(const std::string& aWord) const
{
	return aWord;
}

std::tuple<std::string, std::string, Type> Code::evaluate(ExprNodesCIter& aBegin, ExprNodesCIter& aEnd) const
{
	std::string error;
	std::string codeStat = mCodeStat;
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
			codeStat = replaceAll(codeStat, "${" + arg.identifier().name() + "}", 
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

	return std::make_tuple(error, codeStat, codeType);
}

std::ostream& operator<<(std::ostream& aStream, const Code& aValue)
{
	aStream << "Code(";
  operator<<(aStream, static_cast<const Method&>(aValue));
  aStream << ","
		      << aValue.priority() << ","
		      << aValue.codeStat() << ")";
	return aStream;
}