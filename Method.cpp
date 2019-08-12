#include "Method.h"

#include "Identifier.h"
#include "Record.h"
#include "NateFunctions.h"
#include "core/Core.h"

#include <reflex/matcher.h>
#include <iostream>
#include <sstream>

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

const std::string&      Method::code()			const { return mCode; }
std::string&            Method::code()						{ return mCode; }

void Method::setType(const TypePtr& aType)        { mType = aType; }
void Method::setPriority(int aValue)              { mPriority = aValue; }

void Method::setReturnFlag(const std::string& aFlag)
{
	if (aFlag == "highest")
	{
		setFlag(Highest);
	}
	else if (aFlag == "same")
	{
		setFlag(Same);
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
	else
	{
		std::cerr << "Bad flag: " << aFlag << std::endl;
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

void Method::endDecl()
{
	mOwnerArg = args().cend();
	mPropArg = args().cend();
	mTemplateArg = args().cend();
	mTypenameArg = args().cend();

	for (auto arg = args().cbegin(); arg != args().cend(); ++arg)
	{
		if (arg->isIdentifier())
		{
			if (arg->is(Arg::Owner))
			{
				mOwnerArg = arg;
			}

			if (arg->is(Arg::Prop))
			{
				mPropArg = arg;
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
}

Record* Method::getOwner(const ExprNodesCIter& aNodeIter) const
{
	Record* owner = nullptr;
	if (mOwnerArg != args().cend())
	{
		auto ownerIter = aNodeIter;
		auto dist = std::distance(args().cbegin(), mOwnerArg);
		std::advance(ownerIter, dist);
		owner = dynamic_cast<Record*>(ownerIter->type().get());
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

bool Method::matches(const ExprNodesCIter& aBegin, const ExprNodesCIter& aEnd, bool aDebug) const
{
	if (aDebug)
	{
		auto exp = Expr();
		exp.addNodes(aBegin, aEnd);
		std::cerr << "//// " << pattern() << " " << exp.text() << " ";
	}
	
	ExprNodesCIter nodeIter = aBegin;
	for (const auto& arg : mArgs)
	{
		if (arg.is(Arg::Prop))
		{
			Record* owner = getOwner(aBegin);
			if (owner == nullptr || !owner->getIdentifier(nodeIter->text()))
			{
				if (aDebug) if (owner == nullptr) std::cerr << "No owner" << std::endl;
				if (aDebug) std::cerr << "prop is not member of owner: " << nodeIter->text() << std::endl;
				return false;
			}
		}
		else
		{
			if (arg.isIdentifier())
			{
				if (nodeIter->is(ExprNode::Word))
				{
					if (aDebug) std::cerr << "not id " << arg.isIdentifier() << " " << !nodeIter->is(ExprNode::Word) << std::endl;
					return false;
				}
			}
			else if (arg.word() != nodeIter->text())
			{
				if (aDebug) std::cerr << "not word " << arg.word() << " " << nodeIter->text() << std::endl;
				return false;
			}
		}

		++nodeIter;
	}

	if (aDebug) std::cerr << "matches " << std::endl;

	return true;
}

Method::EvaluateResult
Method::evaluate(const ExprNodesCIter& aBegin, const ExprNodesCIter& aEnd, bool aDebug) const
{
	EvaluateResult result;
	TypePtr firstType;
	TypePtr highestType;
	TypePtr lastType;
	bool isConst = is(ConstExpr);
	Record* owner = getOwner(aBegin);
	TypePtr templateType = getTemplateType(aBegin);
	std::string nodeCode;
			
	result.code = code();
	result.type = type();

	ExprNodesCIter nodeIter = aBegin;
	for (auto const& arg : mArgs)
	{
		if (arg.isIdentifier())
		{
			TypePtr nodeType = nodeIter->type();

			if (arg.is(Arg::Prop) && owner != nullptr)
			{
				auto identifier = owner->getIdentifier(nodeIter->text());
				if (identifier)
				{
					nodeType = identifier->type();
				}
			}

			if (nodeType)
			{
				if (!firstType) 
				{
					firstType = nodeType;
				}
			
				if (!highestType || nodeType->bitSize() > highestType->bitSize())
				{
					highestType = nodeType;
				}
			}
		}

		++nodeIter;
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
					nodeCode = identifier->codeName();
					nodeType = identifier->type();
					result.flags.push_back(ExprNode::Output);
				}
			}
			else
			{
				nodeCode = nodeIter->code();
			}

      ExprNode node = *nodeIter;
			if (arg.is(Arg::Typename))
			{
				node.castToType(templateType->typenameType());
				nodeCode = node.code();
			}
			else if (!arg.is(Arg::Prop))
			{
				if (arg.is(Arg::Same))
				{
					node.castToType(firstType);
				}
				else if (arg.is(Arg::CompHigh))
				{
					node.castToType(highestType);
				}
				else
				{
					node.castToType(argType);
				}

				nodeCode = node.code();
			}

			std::string code = (arg.is(Arg::Prop) || 
													arg.is(Arg::Out) ||
													node.is(ExprNode::Literal))
													? nodeCode 
													: "(" + nodeCode + ")";
			result.code = Core::replaceAll(result.code, "${" + arg.identifier()->name() + "}", code);
			
			lastType = nodeType;
			if (!node.is(ExprNode::ConstExpr))
			{
				isConst = false;
			}
		}

		result.origText.append(nodeIter->text());

		++nodeIter;
	}

	if (is(Same))
	{
		result.type = firstType;
	}
	else if (is(Last))
	{
		result.type = lastType;
	}
	else if (is(Highest))
	{
		result.type = highestType;
	}

	if (isConst)
	{
	  result.flags.push_back(ExprNode::ConstExpr);
	}
	
	if (type())
	{
		result.code = Core::replaceAll(result.code, "__RETURN__", result.type->codeType());
	}

	return result;
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

Method::MatchResult 
Method::checkArgTypes(const ExprNodesCIter& aBegin, const ExprNodesCIter& aEnd, bool aDebug) const
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
				if (!nodeType->is(Type::Record))
				{
					error << "Not a record: " << nodeType->name();
					result.matches = false;
				}
				else
				{
					owner = dynamic_cast<Record*>(nodeType.get());
					if (owner == nullptr)
					{
						error << "Not a record: " << nodeType->name();
						result.matches = false;
					}
				}
			}

			if (arg->is(Arg::Template))
			{
				templateType = nodeIter->type();
				if (!templateType->typenameType())
				{
					error <<  "Not a generic: " << templateType->name();
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
				if (aDebug) std::cerr << "Arg: " << *arg << std::endl;

				if (!firstType)
				{
					firstType = nodeType;
				}

				if (arg->is(Arg::Cmp) && !nodeType->is(Type::Comparable))
				{				
					error <<  "Not a comparible: " << nodeIter->text() << " for " << arg->identifier()->name();
					result.matches = false;
				}
				else if (arg->is(Arg::Same) && 
								 (comp = firstType->canBeCastedFrom(nodeType, needExactMatch)) 
														== Type::CompareResult::No)
				{
					error <<  "1 Not same type: " << arg->identifier()->name() << " of type " << nodeType->name() <<
									  " must be of type " << firstType->name();
					result.matches = false;
				}
				else if (arg->is(Arg::CompHigh) && 
								 (comp = highestType->canBeCastedFrom(nodeType, needExactMatch)) 
														== Type::CompareResult::No)
				{
					error <<  "2 Not correct type: " << arg->identifier()->name() << " of type " << nodeType->name() <<
									  " must be compatible with type " << highestType->name();
					result.matches = false;
				}
				else if (arg->is(Arg::Owner))
				{
					// above code
				}
				else if (arg->is(Arg::Prop))
				{
					if (owner == nullptr)
					{
						error <<  "No record specified for property: " << nodeIter->text();
						result.matches = false;
					}
					else if (!owner->getIdentifier(nodeIter->text()))
					{
						error <<  "Not a property of '" << owner->name() << "': " << nodeIter->text();
						result.matches = false;
					}
				}
				else if (arg->is(Arg::Template))
				{
					// above code
				}
				else if (arg->is(Arg::Typename))
				{
					if (!templateType)
					{
						error <<  "No generic supplied for : " << nodeType->name();
						result.matches = false;
					}
					else if ((comp = templateType->typenameType()->canBeCastedFrom(nodeType, needExactMatch)) 
															== Type::CompareResult::No)
					{
						error <<  "3 Not correct type: " << arg->identifier()->name() << " of type " << nodeType->name() <<
									    " must be of type " << templateType->typenameType()->name();
						result.matches = false;
					}
				}
				else if (argType && !argType->is(Type::Unknown) &&
								 (comp = argType->canBeCastedFrom(nodeType, needExactMatch)) 
															== Type::CompareResult::No)
				{
					error <<  "4 Not correct type: " << arg->identifier()->name() << " of type " << nodeType->name() <<
								    " must be of type " << argType->name();
					result.matches = false;
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
		    << aValue.type() << ","
		    << aValue.code() << ","
		    << Core::join(aValue.args())  << ","
		    << aValue.pattern();
	
	if (aValue.is(Method::Highest)) aStream << ",Highest";
	if (aValue.is(Method::Num)) aStream << ",Num";
	if (aValue.is(Method::Same)) aStream << ",Same";
	if (aValue.is(Method::None)) aStream << ",None";
	if (aValue.is(Method::RightLeft)) aStream << ",RightLeft";
	if (aValue.is(Method::Last)) aStream << ",Last";

	aStream << ")";
	return aStream;
}