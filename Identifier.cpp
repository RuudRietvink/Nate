
#include "Identifier.h"
#include "ExprNode.h"
#include "NateFunctions.h"

Identifier::Identifier(const std::string& aName, const TypePtr& aType)
	: Identifier(aName, aType, Expr(ExprNode("default", "{}", aType)))
{
	mInitValue.node().setFlag(ExprNode::Default, true);
}

Identifier::Identifier(const std::string& aName, const TypePtr& aType, const Expr& aInitValue)
	: mName(aName),
	mCodeName(toCodeName(aName)),
	mType(aType ? aType : std::make_shared<Type>()),
	mInitValue(aInitValue)
{
}

const std::string& Identifier::name()      const { return mName; }
const std::string& Identifier::codeName()  const { return mCodeName; }
const Expr&				 Identifier::initValue() const { return mInitValue; }
TypePtr            Identifier::type()      const { return mType; }

std::ostream& operator<<(std::ostream& aStream, const Identifier& aValue)
{
	aStream << "Identifier(" << aValue.name() << "," << aValue.codeName() << "," << *aValue.type() << "," << aValue.initValue() << ")";
	return aStream;
}