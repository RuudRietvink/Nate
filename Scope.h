#pragma once

#include "Container.h"

#include <list>

class Identifier;
class Record;

class Scope
{
public:
	Scope(const std::string& aName = "");
	virtual ~Scope() = default;

	const std::string& name() const;
	Identifier* getIdentifier(const std::string& aName);
	void addIdentifier(const Identifier& aIdentifier);
	const std::list<Identifier>& getIdentifiers() const;

	Record* getRecord(const std::string& aName);
	void addRecord(const Record& aRecord);

private:
	std::string					  mName;
	Container<Identifier> mIds;
	std::list<Identifier> mIdentifierList;
	Container<Record>     mRecords;													
};