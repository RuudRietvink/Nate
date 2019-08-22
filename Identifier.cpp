
#include "Identifier.h"
#include "ExprNode.h"
#include "NateFunctions.h"

Identifier::Identifier(const std::shared_ptr<Scope>& aScope, const std::string& aName, const TypePtr& aType)
	: Identifier(aScope, aName, aType, Expr(ExprNode("default", "{}", aType)))
{
	mInitValue.node().setFlag(ExprNode::Default, true);
}

Identifier::Identifier(const std::shared_ptr<Scope>& aScope, const std::string& aName, const TypePtr& aType, const Expr& aInitValue)
	: mScope(aScope),
	mName(aName),
	mCodeName(toCodeName(aName)),
	mType(aType ? aType : std::make_shared<Type>()),
	mInitValue(aInitValue)
{
}

const std::string&		Identifier::name()      const { return mName; }
const std::string&		Identifier::codeName()  const { return mCodeName; }
const Expr&						Identifier::initValue() const { return mInitValue; }
TypePtr								Identifier::type()      const { return mType; }
std::weak_ptr<Scope>	Identifier::scope()     const { return mScope; }
bool					 		    Identifier::isObjectMe()const { return mName == "me"; }

std::ostream& operator<<(std::ostream& aStream, const Identifier& aValue)
{
	aStream << "Identifier(" << aValue.name() << "," << aValue.codeName() << "," << *aValue.type() << "," << aValue.initValue() << ")";
	return aStream;
}