#include "Object.h"
#include "NateFunctions.h"

RecordPtr Object::getRecord(const std::string& aName)
{
	return mRecords.getData(aName);
}

void Object::addRecord(RecordPtr& aRecord, const std::string& aName)
{
	aRecord->setFlag(Type::Record, true);
	mRecords.addData(aRecord);
	mTypes.addData(aRecord, aName);			
}

TypePtr Object::getType(const std::string& aName)
{
	return mTypes.getData(aName);
}

void Object::addType(const TypePtr& aType, const std::string& aName)
{
	mTypes.addData(aType, aName);
}

std::list<Define>& Object::getDefines()
{
	return mDefines;
}

void Object::addDefine(const Define& aDefine)
{
	mDefines.push_back(aDefine);
}
