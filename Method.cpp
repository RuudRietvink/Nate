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
	else if (aFlag == "last")
	{
		setFlag(Last, true);
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

bool Method::matches(const ExprNodesCIter& aBegin, const ExprNodesCIter& aEnd) const
{
	auto nodeIter = aBegin;
	auto exp = Expr();
	exp.addNodes(aBegin, aEnd);
	//std::cerr << "//// " << pattern() << " " << exp.text() << " ";

	if (mArgs.size() != static_cast<size_t>(std::distance(aBegin, aEnd)))
	{
		//std::cerr <<  "diff size "  << mArgs.size() << " " << static_cast<size_t>(std::distance(aBegin, aEnd)) << std::endl;
		return false;
	}

	for (const auto& arg : mArgs)
	{
		if (arg.isIdentifier() != (!nodeIter->is(ExprNode::Word)))
		{
			//std::cerr << "not id " << arg.isIdentifier() << " " << !nodeIter->is(ExprNode::Word) << std::endl;
			return false;
		}
		if (!arg.isIdentifier() && arg.word() != nodeIter->text())
		{
			//std::cerr << "not word " << arg.word() << " " << nodeIter->text() << std::endl;
			return false;
		}

		++nodeIter;
	}

	//std::cerr << "matches " << std::endl;

	return true;
}

std::tuple<std::string, std::string, TypePtr, Flags> 
Method::evaluate(const ExprNodesCIter& aBegin, const ExprNodesCIter& aEnd) const
{
	std::string error;
	std::string resultCode = code();
	TypePtr codeType(type());
	TypePtr firstType;
	TypePtr lastType;
	Record* owner = nullptr;
	Flags nodeFlags;
	bool isConst = is(ConstExpr);

	ExprNodesCIter nodeIter = aBegin;
	for (auto arg = args().cbegin(); arg != args().cend() && owner == nullptr; ++arg, ++nodeIter)
	{
		if (arg->isIdentifier())
		{
			if (arg->is(Arg::Owner))
			{
				owner = dynamic_cast<Record*>(nodeIter->type().get());
			}
		}
	}
	
	nodeIter = aBegin;
	for (auto const& arg : mArgs)
	{
		if (arg.isIdentifier())
		{
			const TypePtr& argType = arg.identifier()->type();
			TypePtr nodeType = nodeIter->type();

			if (arg.is(Arg::Prop) && owner != nullptr)
			{
				auto identifier = owner->getIdentifier(nodeIter->text());
				if (identifier)
				{
					nodeType = identifier->type();
					nodeFlags.push_back(ExprNode::Output);
				}
			}

			if (!firstType) 
			{
				firstType = nodeType;
			}

      ExprNode node = *nodeIter;
      node.castToType(arg.is(Arg::Same) ? firstType : argType);
			std::string code = (arg.is(Arg::Prop) || node.is(ExprNode::Literal))
													? node.code() 
													: "(" + node.code() + ")";
			resultCode = Core::replaceAll(resultCode, "${" + arg.identifier()->name() + "}", code);

			if (arg.is(Arg::Num) && !type() && (!codeType || nodeType->isBiggerThan(codeType)))
			{
				codeType = nodeType;
			}

			lastType = nodeType;
			if (!node.is(ExprNode::ConstExpr))
			{
				isConst = false;
			}
		}

		++nodeIter;
	}

	if (is(Same))
	{
		codeType = firstType;
	}
	else if (is(Last))
	{
		codeType = lastType;
	}

	if (isConst)
	{
	  nodeFlags.push_back(ExprNode::ConstExpr);
	}

	return std::make_tuple(error, resultCode, codeType, nodeFlags);
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

std::tuple<std::string, bool> 
Method::checkArgTypes(const ExprNodesCIter& aBegin, const ExprNodesCIter& aEnd) const
{
	std::string error;
	bool result = true;
	TypePtr firstType;
	Record* owner = nullptr;
	
	ExprNodesCIter nodeIter = aBegin;
	for (auto arg = args().cbegin(); arg != args().cend() && owner == nullptr; ++arg, ++nodeIter)
	{
		if (arg->isIdentifier())
		{
			if (arg->is(Arg::Owner))
			{
				const TypePtr& nodeType = nodeIter->type();
				if (!nodeType->is(Type::Record))
				{
					error = "Not a record: " + nodeType->name();
					result = false;
				}
				else
				{
					owner = dynamic_cast<Record*>(nodeType.get());
					if (owner == nullptr)
					{
						error = "Not a record: " + nodeType->name();
						result = false;
					}
				}
			}
		}
	}

	nodeIter = aBegin;
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
				// above code
			}
			else if (arg.is(Arg::Prop))
			{
				if (owner == nullptr)
				{
					error = "No record specified for property: " + nodeIter->text();
					result = false;
				}
				else if (!owner->getIdentifier(nodeIter->text()))
				{
					error = "Not a property of '" + owner->name() + "': " + nodeIter->text();
					result = false;
				}
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
	if (aValue.is(Method::None)) aStream << ",None";
	if (aValue.is(Method::RightLeft)) aStream << ",RightLeft";
	if (aValue.is(Method::Last)) aStream << ",Last";
	if (aValue.is(Method::LeftMonomial)) aStream << ",LeftMonomial";
	if (aValue.is(Method::Unary)) aStream << ",Unary";

	aStream << ")";
	return aStream;
}