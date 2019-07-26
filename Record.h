#pragma once
#include "Type.h"
#include "Identifier.h"

#include <list>

class Scope;

class Record : public Type
{
public:
	Record() = default;
	Record(const std::string& aName, Scope& aScope);
	virtual ~Record() = default;

	Scope& getScope();

	IdentifierPtr getIdentifier(const std::string& aName);
	void addIdentifier(const IdentifierPtr& aIdentifier);
	const std::list<IdentifierPtr>& getIdentifiers() const;

private:
	Scope&	mScope;
};

typedef std::shared_ptr<Record> RecordPtr;
