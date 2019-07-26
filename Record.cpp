#include "Record.h"
#include "Scope.h"

#include "NateFunctions.h"

Record::Record(const std::string& aName, Scope& aScope)
	: Type(aName),
	  mScope(aScope)
{
	setCodeType(toCodeName(aName));
}

Scope& Record::getScope()
{
	return mScope;
}

IdentifierPtr Record::getIdentifier(const std::string& aName)
{
	return mScope.getIdentifier(aName);
}

void Record::addIdentifier(const IdentifierPtr& aIdentifier)
{
	mScope.addIdentifier(aIdentifier);
}

const std::list<IdentifierPtr>& Record::getIdentifiers() const
{
	return mScope.getIdentifiers();
}