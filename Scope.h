#pragma once

#include "Container.h"
#include "Record.h"
#include "Type.h"
#include "Identifier.h"

#include <list>

class Scope
{
public:
	Scope(const std::string& aName = "");
	virtual ~Scope() = default;

	const std::string& name() const;
	IdentifierPtr getIdentifier(const std::string& aName);
	void addIdentifier(const IdentifierPtr& aIdentifier);
	const std::list<IdentifierPtr>& getIdentifiers() const;

	RecordPtr getRecord(const std::string& aName);
	void addRecord(const RecordPtr& aRecord);

	TypePtr getType(const std::string& aName);
	void addType(const TypePtr& aType);

private:
	std::string					     mName;
	Container<IdentifierPtr> mIds;
	std::list<IdentifierPtr> mIdentifierList;
	Container<RecordPtr>     mRecords;					
	Container<TypePtr>       mTypes;													
};