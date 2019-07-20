#include "Scope.h"
#include "Identifier.h"

Scope::Scope(const std::string& aName)
	: mName(aName)
{
}

const std::string& Scope::name() const { return mName; }

Identifier* Scope::getIdentifier(const std::string& aName)
{
	auto iter = mIdentifiers.find(aName);
	return iter == mIdentifiers.end() ? nullptr : &iter->second;
}

void Scope::addIdentifier(const Identifier& aIdentifier)
{
	mIdentifiers.emplace(aIdentifier.name(), aIdentifier);
}
