#include "Scope.h"
#include "Identifier.h"
#include "Record.h"

Scope::Scope(const std::string& aName)
	: mName(aName)
{
}

const std::string& Scope::name() const { return mName; }

Identifier* Scope::getIdentifier(const std::string& aName)
{
	return mIds.getData(aName);
}

const std::list<Identifier>& Scope::getIdentifiers() const
{
	return mIdentifierList;
}

void Scope::addIdentifier(const Identifier& aIdentifier)
{
	mIds.addData(aIdentifier);
	mIdentifierList.push_back(aIdentifier);
}

Record* Scope::getRecord(const std::string& aName)
{
	return mRecords.getData(aName);
}

void Scope::addRecord(const Record& aRecord)
{
	mRecords.addData(aRecord);
}