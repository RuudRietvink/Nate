
#include "Identifier.h"
#include "Expr.h"
#include "NateFunctions.h"

Identifier::Identifier(const IIdentifiersHolderPtr& aIdentifiersHolder, const std::string& aName, const TypePtr& aType)
	: Identifier(aIdentifiersHolder, aName, aType, Expr("default", "{}", aType))
{
	mInitValue.setFlag(Expr::Default, true);
}

Identifier::Identifier(const IIdentifiersHolderPtr& aIdentifiersHolder, const std::string& aName, const TypePtr& aType, const Expr& aInitValue)
	: mIdentifiersHolder(aIdentifiersHolder),
	mName(aName),
	mCodeName(toCodeName(aName)),
	mType(aType ? aType : std::make_shared<Type>()),
	mInitValue(aInitValue)
{
}

std::string Identifier::setFlagString(const std::string& aFlag)
{
	std::string errorResult;

	if (aFlag == "const")
	{
		setFlag(Const);
	}
	else if (aFlag == "readonly")
	{
		setFlag(ReadOnly);
	}
	else if (aFlag == "final")
	{
		setFlag(Final);
	}
	else
	{
		errorResult = "Bad flag: " + aFlag;
	}

	return errorResult;
}

std::string Identifier::nameMe()
{
	return "me";
}

bool Identifier::isNameMe(const std::string& aName)
{
	return aName == nameMe();
}

const std::string&		Identifier::name()      const { return mName; }
const std::string&		Identifier::codeName()  const { return mCodeName; }
Expr    		      		Identifier::initValue() const { return mInitValue; }
TypePtr								Identifier::type()      const { return mType; }
std::weak_ptr<IIdentifiersHolder>	Identifier::identifiersHolder() const { return mIdentifiersHolder; }
bool					 		    Identifier::isObjectMe()const { return isNameMe(mName); }

std::ostream& operator<<(std::ostream& aStream, const Identifier& aValue)
{
	aStream << "Identifier(" << aValue.name() << "," << aValue.codeName() << "," << *aValue.type() << "," << aValue.initValue();
	if (aValue.is(Identifier::Const)) aStream << ",Const";
	if (aValue.is(Identifier::Property)) aStream << ",Property";
	if (aValue.is(Identifier::ReadOnly)) aStream << ",ReadOnly";
	if (aValue.is(Identifier::ObjectImpl)) aStream << ",ObjectImpl";
	if (aValue.is(Identifier::Local)) aStream << ",Local";
	aStream << ")";
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