
#include "Code.h"
#include "Identifier.h"
#include "NateFunctions.h"

#include <reflex/matcher.h>
#include <iostream>

Code::Code()
{}

int                         Code::priority()  const { return mPriority; }
const Type&                 Code::type()      const { return mType; }
const std::string&          Code::codeStat()  const { return mCodeStat; }
std::string&                Code::codeStat()        { return mCodeStat; }
const std::string&          Code::signature() const { return mSignature; }
const std::vector<CodeArg>& Code::codeArgs()  const { return mCodeArgs; }
std::vector<CodeArg>&       Code::codeArgs()        { return mCodeArgs; }
CodeArg&                    Code::curCodeArg()      { return mCodeArgs.back(); }

void Code::setPriority(int aValue)    { mPriority = aValue; }
void Code::setType(const Type& aType) { mType = aType; }

void Code::setReturnFlag(const std::string& aFlag)
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

void Code::addCodeArgWord(const std::string& aWord)
{
	mCodeArgs.push_back(CodeArg(aWord));
	mSignature += aWord + " ";
}

void Code::addCodeArgId(const Identifier& aId)
{
	mCodeArgs.push_back(CodeArg(aId));
	mSignature += aId.name();
	if (!type().is(Type::Unknown))
	{
		mSignature += " is " + aId.type().name() + " ";
	}
}

void Code::addCodeStatWord(const std::string& aWord)
{
	mCodeStat += aWord;
}

void Code::addCodeStatId(const std::string& aId)
{
	mCodeStat += "${" + aId + "}";
}

bool Code::matches(const std::string& aPattern) const
{
	auto& regex = pattern();
	return mIsRegex 
         ? reflex::Matcher(pattern(), aPattern).matches()
		     : (regex == aPattern);
}

const std::string& Code::pattern() const
{
	if (mPattern.empty())
	{
		for (auto& codeArg : mCodeArgs)
		{
			if (codeArg.isIdentifier())
			{
				mPattern += "%E%";
			}
			else
			{
				mPattern += codeArg.word();
			}
		}
	}

	return mPattern;
}

std::tuple<std::string, bool> Code::checkArgTypes(ExprNodesCIter& aBegin, ExprNodesCIter& aEnd) const
{
	std::string error;
	bool result = true;
	ExprNodesCIter nodeIter = aBegin;
	Type firstType;

	for (auto const& arg : mCodeArgs)
	{
		if (arg.isIdentifier())
		{
			const Type& nodeType = nodeIter->type();
			const Type& argType = arg.identifier().type();

			if (firstType.is(Type::Unknown))
			{
				firstType = nodeType;
			}

			if (arg.is(CodeArg::Num) && !nodeType.is(Type::Number))
			{
				error = "Not a number: " + nodeIter->text() + " for " + arg.identifier().name();
				result = false;
			}
			else if (argType.is(Type::Number) && !nodeType.is(Type::Number))
			{
				error = "Not a number: " + nodeIter->text() + " for " + arg.identifier().name();
				result = false;
			}
			else if (arg.is(CodeArg::Cmp) && !nodeType.is(Type::Comparable))
			{				
				error = "Not a comparible: " + nodeIter->text() + " for " + arg.identifier().name();
				result = false;
			}
			else if (arg.is(CodeArg::Same) && nodeType.name() != firstType.name())
			{
				error = "Not same type: " + arg.identifier().name() + " must be of type " + firstType.name();
				result = false;
			}
			else if ((!argType.is(Type::Unknown)) && (!argType.isCompatibleWith(nodeType)))
			{
				error = "Not same type: " + arg.identifier().name() + " must be of type " + nodeType.name();
				result = false;
			}
		}

		++nodeIter;
	}

	return std::make_tuple(error, result);
}

std::tuple<std::string, std::string, Type> Code::evaluate(ExprNodesCIter& aBegin, ExprNodesCIter& aEnd) const
{
	std::string error;
	std::string codeStat = mCodeStat;
	ExprNodesCIter nodeIter = aBegin;
	Type codeType(type());
	Type firstType;

	for (auto const& arg : mCodeArgs)
	{
		if (arg.isIdentifier())
		{
			const Type& nodeType = nodeIter->type();

			if (firstType.is(Type::Unknown))
			{
				firstType = nodeType;
			}

			codeStat = replaceAll(codeStat, "${" + arg.identifier().name() + "}", "(" + nodeIter->code() + ")");

			if (arg.is(CodeArg::Num) && type().is(Type::Unknown) && nodeType.isBiggerThan(codeType))
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
	aStream << "Code(" 
		    << aValue.type() << ","
		    << join(aValue.codeArgs())  << "," 
		    << aValue.codeStat() << "," 
		    << aValue.pattern() << ")";
	return aStream;
}