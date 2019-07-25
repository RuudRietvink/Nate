#include "Record.h"
#include "Scope.h"

#include "Identifier.h"
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

Identifier* Record::getIdentifier(const std::string& aName)
{
	return mScope.getIdentifier(aName);
}

void Record::addIdentifier(const Identifier& aIdentifier)
{
	mScope.addIdentifier(aIdentifier);
}

const std::list<Identifier>& Record::getIdentifiers() const
{
	return mScope.getIdentifiers();
}