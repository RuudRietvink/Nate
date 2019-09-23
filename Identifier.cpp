
#include "Identifier.h"
#include "ExprNode.h"
#include "NateFunctions.h"

Identifier::Identifier(const std::shared_ptr<IIdentifiersHolder>& aIdentifiersHolder, const std::string& aName, const TypePtr& aType)
	: Identifier(aIdentifiersHolder, aName, aType, Expr(ExprNode("default", "{}", aType)))
{
	mInitValue.node().setFlag(ExprNode::Default, true);
}

Identifier::Identifier(const std::shared_ptr<IIdentifiersHolder>& aIdentifiersHolder, const std::string& aName, const TypePtr& aType, const Expr& aInitValue)
	: mIdentifiersHolder(aIdentifiersHolder),
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
std::weak_ptr<IIdentifiersHolder>	Identifier::identifiersHolder() const { return mIdentifiersHolder; }
bool					 		    Identifier::isObjectMe()const { return mName == "me"; }

std::ostream& operator<<(std::ostream& aStream, const Identifier& aValue)
{
	aStream << "Identifier(" << aValue.name() << "," << aValue.codeName() << "," << *aValue.type() << "," << aValue.initValue() << ")";
	return aStream;
}

IdentifierPtr Identifiers::get(const std::string& aName)
{
	return mIds.getData(aName);
}

const std::list<IdentifierPtr>& Identifiers::get() const
{
	return mIdentifierList;
}

void Identifiers::add(const IdentifierPtr& aIdentifier)
{
	mIds.addData(aIdentifier);
	mIdentifierList.push_back(aIdentifier);
}