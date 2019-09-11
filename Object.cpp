#include "Object.h"
#include "NateFunctions.h"

#include <algorithm>

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


Define* Object::getDefineLike(const Define& aDefine)
{
	auto iter = std::find_if(mDefines.begin(), mDefines.end(),
													 [&aDefine](const Define& item)
													 { return aDefine.pattern() == item.pattern(); });
	return (iter != mDefines.end()) ? &(*iter) : nullptr;
}

std::ostream& operator<<(std::ostream& aStream, const Object& aValue)
{
	aStream << "Object(" 
		    << static_cast<Type>(aValue) << ",";
	
	aStream << ")";
	return aStream;
}