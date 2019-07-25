#pragma once
#include "Type.h"
#include "Scope.h"

#include <list>

class Identifier;
class Scope;

class Record : public Type
{
public:
	Record() = default;
	Record(const std::string& aName, Scope& aScope);
	virtual ~Record() = default;

	Scope& getScope();

	Identifier* getIdentifier(const std::string& aName);
	void addIdentifier(const Identifier& aIdentifier);
	const std::list<Identifier>& getIdentifiers() const;

private:
	Scope&	mScope;
};

