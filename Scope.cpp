#include "Scope.h"
#include "Identifier.h"
#include "Record.h"

Scope::Scope(const std::string& aName)
	: mName(aName)
{
}

const std::string& Scope::name() const { return mName; }

IdentifierPtr Scope::getIdentifier(const std::string& aName)
{
	return mIds.getData(aName);
}

const std::list<IdentifierPtr>& Scope::getIdentifiers() const
{
	return mIdentifierList;
}

void Scope::addIdentifier(const IdentifierPtr& aIdentifier)
{
	mIds.addData(aIdentifier);
	mIdentifierList.push_back(aIdentifier);
}

RecordPtr Scope::getRecord(const std::string& aName)
{
	return mRecords.getData(aName);
}

void Scope::addRecord(const RecordPtr& aRecord)
{
	mRecords.addData(aRecord);
	mTypes.addData(aRecord);
}

TypePtr Scope::getType(const std::string& aName)
{
	return mTypes.getData(aName);
}

void Scope::addType(const TypePtr& aType)
{
	mTypes.addData(aType);
}