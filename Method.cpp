#include "Method.h"

#include "Identifier.h"
#include "NateFunctions.h"

#include <reflex/matcher.h>
#include <iostream>
#include <sstream>

Method::Method()
{}

const Type&             Method::type()      const { return mType; }
const std::string&      Method::signature() const { return mSignature; }
const std::vector<Arg>& Method::args()      const { return mArgs; }
std::vector<Arg>&       Method::args()            { return mArgs; }
Arg&                    Method::curArg()          { return mArgs.back(); }
int                     Method::priority()  const { return mPriority; }

void Method::setType(const Type& aType) { mType = aType; }
void Method::setPriority(int aValue)    { mPriority = aValue; }

void Method::setReturnFlag(const std::string& aFlag)
{
	if (aFlag == "Num")
	{
		setFlag(Num, true);
	}
	else if (aFlag == "Highest")
	{
		setFlag(Highest, true);
	}
	else if (aFlag == "Same")
	{
		setFlag(Same, true);
	}
}

void Method::addArgWord(const std::string& aWord)
{
	mArgs.push_back(Arg(aWord));
	mSignature += aWord + " ";
}

void Method::addArgId(const Identifier& aId)
{
	mArgs.push_back(Arg(aId));
	mSignature += aId.name();
	if (!type().is(Type::Unknown))
	{
		mSignature += " is " + aId.type().name() + " ";
	}
}

bool Method::matches(const std::string& aPattern) const
{
	auto& regex = pattern();
	return mIsRegex 
         ? reflex::Matcher(pattern(), aPattern).matches()
		     : (regex == aPattern);
}

std::string Method::toCodeWord(const std::string& aWord) const
{
	return ::toCodeWord(aWord);
}

const std::string& Method::pattern() const
{
	if (mPattern.empty())
	{
		std::stringstream buf;

		for (auto& arg : mArgs)
		{
			if (arg.isIdentifier())
			{
				buf << "_E_";
			}
			else
			{
				buf << toCodeWord(arg.word());
			}
		}

		mPattern = buf.str();
	}

	return mPattern;
}

std::tuple<std::string, bool> Method::checkArgTypes(ExprNodesCIter& aBegin, ExprNodesCIter& aEnd) const
{
	std::string error;
	bool result = true;
	ExprNodesCIter nodeIter = aBegin;
	Type firstType;

	for (auto const& arg : mArgs)
	{
		if (arg.isIdentifier())
		{
			const Type& nodeType = nodeIter->type();
			const Type& argType = arg.identifier().type();

			if (firstType.is(Type::Unknown))
			{
				firstType = nodeType;
			}

			if (arg.is(Arg::Num) && !nodeType.is(Type::Number))
			{
				error = "Not a number: " + nodeIter->text() + " for " + arg.identifier().name();
				result = false;
			}
			else if (argType.is(Type::Number) && !nodeType.is(Type::Number))
			{
				error = "Not a number: " + nodeIter->text() + " for " + arg.identifier().name();
				result = false;
			}
			else if (arg.is(Arg::Cmp) && !nodeType.is(Type::Comparable))
			{				
				error = "Not a comparible: " + nodeIter->text() + " for " + arg.identifier().name();
				result = false;
			}
			else if (arg.is(Arg::Same) && nodeType.name() != firstType.name())
			{
				error = "Not same type: " + arg.identifier().name() + " of type " + nodeType.name() +
                " must be of type " + firstType.name();
				result = false;
			}
			else if ((!argType.is(Type::Unknown)) && (!argType.isCompatibleWith(nodeType)))
			{
				error = "Not same type: " + arg.identifier().name() + + " of type " + nodeType.name() +
                " must be of type " + argType.name();
				result = false;
			}
		}

		++nodeIter;
	}

	return std::make_tuple(error, result);
}

std::ostream& operator<<(std::ostream& aStream, const Method& aValue)
{
	aStream << "Method(" 
		    << aValue.type() << ","
		    << join(aValue.args())  << "," 
		    << aValue.pattern() << ")";
	return aStream;
}