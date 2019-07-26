#include "Method.h"

#include "Identifier.h"
#include "Record.h"
#include "NateFunctions.h"

#include <reflex/matcher.h>
#include <iostream>
#include <sstream>

Method::Method()
{}

const TypePtr&          Method::type()      const { return mType; }
const std::string&      Method::signature() const { return mSignature; }
const std::vector<Arg>& Method::args()      const { return mArgs; }
std::vector<Arg>&       Method::args()            { return mArgs; }
Arg&                    Method::curArg()          { return mArgs.back(); }
int                     Method::priority()  const { return mPriority; }

const std::string&      Method::code()			const { return mCode; }
std::string&            Method::code()						{ return mCode; }

void Method::setType(const TypePtr& aType) { mType = aType; }
void Method::setPriority(int aValue)       { mPriority = aValue; }

void Method::setReturnFlag(const std::string& aFlag)
{
	if (aFlag == "num")
	{
		setFlag(Num, true);
	}
	else if (aFlag == "highest")
	{
		setFlag(Highest, true);
	}
	else if (aFlag == "same")
	{
		setFlag(Same, true);
	}
	else if (aFlag == "none")
	{
		setFlag(None, true);
	}
}

void Method::addArgWord(const std::string& aWord)
{
	mArgs.push_back(Arg(aWord));
	mSignature += aWord + " ";
}

void Method::addArgId(const IdentifierPtr& aId)
{
	mArgs.push_back(Arg(aId));
	mSignature += aId->name();
	if (type())
	{
		mSignature += " is " + aId->type()->name() + " ";
	}
}

bool Method::matches(ExprNodesCIter& aBegin, ExprNodesCIter& aEnd) const
{
	auto nodeIter = aBegin;

	if (mArgs.size() != static_cast<size_t>(std::distance(aBegin, aEnd)))
	{
		return false;
	}

	for (const auto& arg : mArgs)
	{
		if (arg.isIdentifier() != (!nodeIter->is(ExprNode::Word)))
		{
			return false;
		}
		if (!arg.isIdentifier() && arg.word() != nodeIter->text())
		{
			return false;
		}

		++nodeIter;
	}

	return true;
}

std::tuple<std::string, std::string, TypePtr> Method::evaluate(ExprNodesCIter& aBegin, ExprNodesCIter& aEnd) const
{
	std::string error;
	std::string resultCode = code();
	ExprNodesCIter nodeIter = aBegin;
	TypePtr codeType(type());
	TypePtr firstType;

	for (auto const& arg : args())
	{
		if (arg.isIdentifier())
		{
			const TypePtr& nodeType = nodeIter->type();

			if (!firstType) 
			{
				firstType = nodeType;
			}

      ExprNode node = *nodeIter;
			TypePtr argType = arg.identifier()->type();
      node.castToType(argType->is(Arg::Same) ? firstType : argType);
			resultCode = replaceAll(resultCode, "${" + arg.identifier()->name() + "}", 
                              "(" + node.code() + ")");

			if (arg.is(Arg::Num) && !type() && (!codeType || nodeType->isBiggerThan(codeType)))
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

	return std::make_tuple(error, resultCode, codeType);
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
				buf << toCodeWord(arg.word()) << "_";
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
	TypePtr firstType;
	Record* owner = nullptr;

	for (auto const& arg : mArgs)
	{
		if (arg.isIdentifier())
		{
			const TypePtr& nodeType = nodeIter->type();
			const TypePtr& argType = arg.identifier()->type();

			if (!firstType)
			{
				firstType = nodeType;
			}

			if (arg.is(Arg::Num) && !nodeType->is(Type::Number))
			{
				error = "Not a number: " + nodeIter->text() + " for " + arg.identifier()->name();
				result = false;
			}
			else if (argType->is(Type::Number) && !nodeType->is(Type::Number))
			{
				error = "Not a number: " + nodeIter->text() + " for " + arg.identifier()->name();
				result = false;
			}
			else if (arg.is(Arg::Cmp) && !nodeType->is(Type::Comparable))
			{				
				error = "Not a comparible: " + nodeIter->text() + " for " + arg.identifier()->name();
				result = false;
			}
			else if (arg.is(Arg::Same) && !firstType->isCompatibleWith(nodeType))
			{
				error = "Not same type: " + arg.identifier()->name() + " of type " + nodeType->name() +
                " must be of type " + firstType->name();
				result = false;
			}
			else if (arg.is(Arg::Owner))
			{
				if (!nodeType->is(Type::Record))
				{
					error = "Not a record: " + nodeType->name();
					result = false;
				}
				else
				{
				}
			}
			else if (arg.is(Arg::Prop) && !nodeIter->is(ExprNode::Word))
			{
				error = "Not a record: " + nodeType->name();
				result = false;
			}
			else if (argType && !argType->is(Type::Unknown) && !argType->isCompatibleWith(nodeType))
			{
				error = "Not same type: " + arg.identifier()->name() + + " of type " + nodeType->name() +
                " must be of type " + argType->name();
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
		    << aValue.code() << ","
		    << join(aValue.args())  << ","
		    << aValue.pattern();
	
	if (aValue.is(Method::Highest)) aStream << ",Highest";
	if (aValue.is(Method::Num)) aStream << ",Num";
	if (aValue.is(Method::Same)) aStream << ",Same";
	if (aValue.is(Method::RightLeft)) aStream << ",RightLeft";
	if (aValue.is(Method::Last)) aStream << ",Last";

	aStream << ")";
	return aStream;
}