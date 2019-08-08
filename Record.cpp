#include "Record.h"
#include "Scope.h"

#include "NateFunctions.h"

Record::Record(const std::string& aName)
	: Type("record"),
	  mScope(std::make_shared<Scope>(aName))
{
	setCodeType(toCodeName(aName));
}

ScopePtr& Record::scope()
{
	return mScope;
}

IdentifierPtr Record::getIdentifier(const std::string& aName)
{
	return mScope->getIdentifier(aName);
}

void Record::addIdentifier(const IdentifierPtr& aIdentifier)
{
	mScope->addIdentifier(aIdentifier);
}

const std::list<IdentifierPtr>& Record::getIdentifiers() const
{
	return mScope->getIdentifiers();
}

std::ostream& Record::print(std::ostream& aStream) const
{
	aStream << "Record(";
	Type::print(aStream);
	aStream << "Identifiers(";
	for (auto id : getIdentifiers())
	{
		aStream << *id << ",";
	}
	aStream << ")";
	aStream << ")";
	return aStream;
}