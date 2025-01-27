#include "Method.h"

#include "Expr.h"
#include "Identifier.h"
#include "Object.h"
#include "Record.h"
#include "NateFunctions.h"
#include "core/Core.h"
#include "NateParser.h"

#include <reflex/matcher.h>
#include <iostream>
#include <sstream>

namespace nate
{
Method::Method()
{}

void Method::copyFrom(const Method& aMethod)
{
	mPriority = aMethod.mPriority;
	mCode = aMethod.mCode;
	mType = aMethod.mType;
	setFlags(aMethod.getFlags());
}

const TypePtr&          Method::type()      const { return mType; }
const std::string&      Method::signature() const { return mSignature; }
const std::vector<Arg>& Method::args()      const { return mArgs; }
std::vector<Arg>&       Method::args()            { return mArgs; }
Arg&                    Method::curArg()          { return mArgs.back(); }
int                     Method::priority()  const { return mPriority; }

const std::string&      Method::code()		const { return mCode; }
std::string&            Method::code()			  { return mCode; }

ObjectPtr               Method::object()    const { return mObject; }
void                    Method::setObject(const ObjectPtr& aObject) { mObject = aObject; }
Method::ArgConstIterator Method::objectArg() const { return mObjectArg;  }

void Method::setType(const TypePtr& aType)        { mType = aType; }
void Method::setPriority(int aValue)              { mPriority = aValue; }

std::string Method::setFlagString(const std::string& aFlag)
{
	std::string errorResult;

	if (aFlag == "highest")
	{
		setFlag(Highest);
	}
	else if (aFlag == "same")
	{
		setFlag(Same);
	}
	else if (aFlag == "me")
	{
		setFlag(Me);
	}
	else if (aFlag == "last")
	{
		setFlag(Last);
	}
	else if (aFlag == "none")
	{
		setFlag(None);
	}
	else if (aFlag == "right")
	{
		setFlag(RightLeft);
	}
	else if (aFlag == "const")
	{
		setFlag(ConstExpr);
	}
	else if (aFlag == "final")
	{
		setFlag(Final);
	}
	else if (aFlag == "ref")
	{
		setFlag(Ref);
	}
    else if (aFlag == "typename")
    {
        setFlag(Typename);
    }
    else if (aFlag == "output")
    {
        setFlag(Output);
    }
	else
	{
		errorResult = "Bad flag: " + aFlag;
	}

	return errorResult;
}

bool Method::isObjectMethod() const
{
	return object() != nullptr;
}

bool Method::isStatic() const
{
	return isObjectMethod() && is(Static);
}

void Method::addArgWord(const std::string& aWord)
{
	mArgs.push_back(Arg(aWord));
	mSignature += aWord + " ";
}

void Method::addArgId(const IdentifierPtr& aId)
{
	mArgs.push_back(Arg(aId));
	mSignature += aId->name() + " ";
	if (type())
	{
		mSignature += "is " + aId->type()->name() + " ";
	}
}

void Method::endDecl()
{
	mObjectArg = args().cend();
	mOwnerArg = args().cend();
	mMemberArg = args().cend();
	mTemplateArg = args().cend();
	mTypenameArg = args().cend();

	for (auto arg = args().cbegin(); arg != args().cend(); ++arg)
	{
		if (arg->isIdentifier())
		{
			if (arg->identifier()->isObjectMe())
			{
				mObjectArg = arg;
				setFlag(ConstMethod, !arg->is(Arg::Out));
			}

			if (arg->is(Arg::Owner))
			{
				mOwnerArg = arg;
			}

			if (arg->is(Arg::Member))
			{
				mMemberArg = arg;
			}

			if (arg->is(Arg::Template))
			{
				mTemplateArg = arg;
			}

			if (arg->is(Arg::Typename))
			{
				mTypenameArg = arg;
			}
		}
	}

	if (mObjectArg == args().cend())
	{
		setFlag(Static);
	}
}

ExprNodesCIter Method::getOwnerNode(const ExprNodesCIter& aNodeIter) const
{
	auto ownerIter = aNodeIter;
	if (mOwnerArg != args().cend())
	{
		auto dist = std::distance(args().cbegin(), mOwnerArg);
		std::advance(ownerIter, dist);
	}

	return ownerIter;
}

Record* Method::getOwnerType(const Type* type) const
{
	auto tempType = const_cast<Type*>(type);
	Record* owner = dynamic_cast<Object*>(tempType);
	if (owner == nullptr)
	{
		owner = dynamic_cast<Record*>(tempType);
	}

	return owner;
}

Record* Method::getOwner(const ExprNodesCIter& aNodeIter) const
{
	Record* owner = nullptr;
	if (mOwnerArg != args().cend())
	{
		auto ownerIter = getOwnerNode(aNodeIter);
		owner = getOwnerType(ownerIter->type().get());
	}

	return owner;
}

TypePtr Method::getTemplateType(const ExprNodesCIter& aNodeIter) const
{
	TypePtr templateType;
	if (mTemplateArg != args().cend())
	{
		auto templateIter = aNodeIter;
		auto dist = std::distance(args().cbegin(), mTemplateArg);
		std::advance(templateIter, dist);
		templateType = templateIter->type();
	}

	return templateType;
}

bool Method::matches(const ExprNodesCIter& aBegin, const ExprNodesCIter& aEnd, int aDebug) const
{
	if (aDebug >= 2)
	{
		auto exp = Expr();
		exp.addNodes(aBegin, aEnd);
		std::cerr << "//// " << pattern() << " " << exp.text() << " ";
	}
	
	ExprNodesCIter nodeIter = aBegin;
	for (const auto& arg : mArgs)
	{
		if (arg.is(Arg::Member))
		{
			Record* owner = getOwner(aBegin);
			if (owner == nullptr || !owner->getIdentifier(nodeIter->text()))
			{
				if (owner == nullptr) 
				{
					if (aDebug >= 3) std::cerr << "No owner of " << nodeIter->text() << std::endl;
				}
				else
				{
					if (aDebug >= 3) std::cerr << "Field is not member of owner: " << owner->name() << ":" << nodeIter->text() << std::endl;
				}
				return false;
			}
		}
		else
		{
			if (arg.isIdentifier())
			{
				if (nodeIter->is(Expr::Word))
				{
					if (aDebug >= 3) std::cerr << "not id " << arg.isIdentifier() << " " << !nodeIter->is(Expr::Word) << std::endl;
					return false;
				}
			}
			else if (arg.word() != nodeIter->text())
			{
				if (aDebug >= 3) std::cerr << "not word " << arg.word() << " " << nodeIter->text() << std::endl;
				return false;
			}
		}

		++nodeIter;
	}
	
	if (aDebug)
	{
		auto exp = Expr();
		exp.addNodes(aBegin, aEnd);
		std::cerr << pattern() << " matches " << exp.text() << std::endl;
	}

	return true;
}

void Method::getTypes(
		const ExprNodesCIter& aBegin,
		Record* aOwner,
		TypePtr& aFirstType,
		TypePtr& aHighestType) const
{
	ExprNodesCIter nodeIter = aBegin;
	for (auto const& arg : mArgs)
	{
		if (arg.isIdentifier())
		{
			TypePtr nodeType = nodeIter->type();

			if (arg.is(Arg::Member) && aOwner != nullptr)
			{
				auto identifier = aOwner->getIdentifier(nodeIter->text());
				if (identifier)
				{
					nodeType = identifier->type();
				}
			}

			if (nodeType)
			{
				if (!aFirstType) 
				{
					aFirstType = nodeType;
				}
			
				if (!aHighestType || nodeType->bitSize() > aHighestType->bitSize())
				{
					aHighestType = nodeType;
				}
			}
		}

		++nodeIter;
	}
}

void Method::handleOwnerMember(
		const ExprNodesCIter& aNodeIter,
		Record* aOwner,
		const ExprNodesCIter& aOwnerNode,
		Method::EvaluateResult& aResult,
		std::string& aNodeCode,
		TypePtr& aNodeType) const
{
	auto identifier = aOwner->getIdentifier(aNodeIter->text());
	if (identifier)
	{
		if (identifier->isProperty())
		{
			aNodeCode = identifier->codeName() + "_get()";
		}
		else
		{
			aNodeCode = identifier->codeName();
		}

		aNodeType = identifier->type();
					
		aResult.flags[Expr::Output] = true;
		if (aOwnerNode->is(Expr::Property) && !identifier->isProperty())
		{
			aResult.flags[Expr::ConstExpr] = true;
		}

		if (identifier->is(Identifier::Const))
		{
			aResult.flags[Expr::ConstExpr] = true;
		}
		if (identifier->isProperty())
		{
			aResult.flags[Expr::Property] = true;
		}
		if (identifier->is(Identifier::ObjectImpl))
		{
			aResult.flags[Expr::ObjectImpl] = true;
		}
	}
}

std::string Method::toCodeWord(const std::string& aWord) const
{
	return nate::toCodeWord(aWord);
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
				buf << "E_";
				if (arg.is(Arg::Out))
				{
					buf << "O_";
				}
				if (arg.identifier()->isObjectMe())
				{
					buf << Identifier::nameMe() << "__";
				}
				else
				{
					buf << toCodeName(arg.identifier()->type()->name()) << "_";
				}
			}
			else
			{
				buf << nate::toCodeWord(arg.word()) << "_";
			}
		}

		mPattern = buf.str();
	}

	return mPattern;
}

Method::MatchResult 
Method::checkArgTypes(NateParser& parser, const ExprNodesCIter& aBegin, const ExprNodesCIter& aEnd, int aDebug) const
{
	MatchResult result;
	TypePtr firstType;
	TypePtr highestType;
	Record* owner = nullptr;
	TypePtr templateType;
	std::ostringstream error;
	
	ExprNodesCIter nodeIter = aBegin;
	for (auto arg = args().cbegin(); arg != args().cend() && owner == nullptr; ++arg, ++nodeIter)
	{
		const TypePtr& nodeType = nodeIter->type();

		if (arg->isIdentifier())
		{
			if (arg->is(Arg::Owner))
			{
				if (!nodeType->is(Type::Record) && !nodeType->is(Type::Object))
				{
					error << "Not a object or record: " << nodeType->name();
					result.matches = false;
				}
				else
				{
					owner = getOwnerType(nodeType.get());
					if (owner == nullptr)
					{
						error << "Not a object/record: " << nodeType->name();
						result.matches = false;
					}
				}
			}

			if (arg->is(Arg::Template))
			{
				templateType = nodeIter->type();
				if (!templateType->typenameType())
				{
					error << "Not a generic: " << templateType->name();
					result.matches = false;
				}
			}
			
			if (nodeType)
			{
				if (!highestType || nodeType->isBiggerThan(highestType))
				{
					highestType = nodeType;
				}
			}
		}
	}

	if (result.matches)
	{
		nodeIter = aBegin;
		for (auto arg = args().cbegin(); arg != args().cend() && result.matches; ++arg, ++nodeIter)
		{
			Type::CompareResult comp = Type::CompareResult::No;

			if (arg->isIdentifier())
			{
				const TypePtr& nodeType = nodeIter->type();
				const TypePtr& argType = arg->identifier()->type();
				bool needExactMatch = arg->is(Arg::Exact);
				if (aDebug > 1) std::cerr << "Arg: " << *arg << std::endl;

				if (!firstType)
				{
					firstType = nodeType;
				}
				
				if (!isCodeMethod() && arg->is(Arg::Out) && nodeIter->is(Expr::Property))
				{				
					error << "Property may not be output: " << nodeIter->text();
					result.matches = false;
				}
				else if (arg->is(Arg::Out) && !nodeIter->is(Expr::Output))
				{				
					error << "Argument must be output: " << nodeIter->text();
					result.matches = false;
				}
				else if (arg->is(Arg::Literal) && !nodeIter->is(Expr::Literal))
				{				
					error << "Argument must be literal: " << nodeIter->text();
					result.matches = false;
				}
				else if (arg->is(Arg::Cmp) && !nodeType->is(Type::Comparable))
				{				
					error << "Not a comparible: " << nodeIter->text() << " for " << arg->identifier()->name();
					result.matches = false;
				}
				else if (arg->is(Arg::Owner))
				{
					// above code
				}
				else if (arg->is(Arg::Member))
				{
					if (owner == nullptr)
					{
						error << "No object/record specified for member: " << nodeIter->text();
						result.matches = false;
					}
					else if (!owner->getIdentifier(nodeIter->text()))
					{
						error << "Not a member of '" << owner->name() << "': " << nodeIter->text();
						result.matches = false;
					}
				}
				else if (arg->is(Arg::Template))
				{
					// above code
				}
				else if (!nodeType)
				{
					error << "0 No type defined for " << nodeIter->text();
					result.matches = false;
				}
				else if (arg->is(Arg::Same) && 
						 (comp = firstType->canBeCastedFrom(nodeType, needExactMatch)) == Type::CompareResult::No)
				{
					error << "1 Not same type: " << arg->identifier()->name() << " of type " << nodeType->name() <<
							 " must be of type " << firstType->name();
					result.matches = false;
				}
				else if (arg->is(Arg::CompHigh) && 
						 (comp = highestType->canBeCastedFrom(nodeType, needExactMatch)) == Type::CompareResult::No)
				{
					error << "2 Not correct type: " << arg->identifier()->name() << " of type " << nodeType->name() <<
							 " must be compatible with type " << highestType->name();
					result.matches = false;
				}
				else if (arg->is(Arg::Typename))
				{
					if (!templateType)
					{
						error << "No generic supplied for : " << nodeType->name();
						result.matches = false;
					}
					else if ((comp = templateType->typenameType()->canBeCastedFrom(nodeType, needExactMatch)) == Type::CompareResult::No)
					{
						error << "3 Not correct type: " << arg->identifier()->name() << " of type " << nodeType->name() <<
								 " must be of type " << templateType->typenameType()->name();
						result.matches = false;
					}
				}
				else if (argType && !argType->is(Type::Unknown) &&
						 (comp = argType->canBeCastedFrom(nodeType, needExactMatch)) == Type::CompareResult::No)
				{
					if (args().size() > 1)
					{
						error << "4 Not correct type: " << arg->identifier()->name() << " of type " << nodeType->name() <<
							     " must be of type " << argType->name();
					}

					result.matches = false;
				}

				if (result.matches)
				{
					if (!arg->is(Arg::Member))
					{
						if (!parser.checkProperty(error, nodeIter))
						{
							result.matches = false;
						}
					}
				}

				if (comp == Type::CompareResult::RequiresCast)
				{
					++result.castCount;
				}
			}
		}
	}
	
	if (!result.matches)
	{
		result.error = error.str();
	}

	if (aDebug) std::cerr << result.matches << " " 
		                  << static_cast<int>(result.castCount) << " "
						  << result.error << std::endl;

	return result;
}

std::ostream& operator<<(std::ostream& aStream, const Method& aValue)
{
	aStream << "Method(" 
		    << (aValue.type() ? *aValue.type() : Type()) << ","
		    << aValue.code() << ","
		    << Core::join(aValue.args()) << ","
		    << aValue.pattern() << ","
		    << aValue.signature();
	
	if (aValue.is(Method::Highest)) aStream << ",Highest";
	if (aValue.is(Method::Num)) aStream << ",Num";
	if (aValue.is(Method::Same)) aStream << ",Same";
	if (aValue.is(Method::None)) aStream << ",None";
	if (aValue.is(Method::RightLeft)) aStream << ",RightLeft";
	if (aValue.is(Method::Last)) aStream << ",Last";
	if (aValue.is(Method::ConstExpr)) aStream << ",ConstExpr";
	if (aValue.is(Method::ConstMethod)) aStream << ",ConstMethod";
	if (aValue.is(Method::Defined)) aStream << ",Defined";
	if (aValue.is(Method::Undeclared)) aStream << ",Undeclared";
	if (aValue.is(Method::Overriden)) aStream << ",Overriden";	
	if (aValue.is(Method::Me)) aStream << ",Me";	
	if (aValue.is(Method::Static)) aStream << ",Static";	
	if (aValue.is(Method::Ref)) aStream << ",Ref";
    if (aValue.is(Method::Typename)) aStream << ",Typename";
    if (aValue.is(Method::Output)) aStream << ",Output";

	aStream << ")";
	return aStream;
}

}
