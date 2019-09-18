#include "Object.h"
#include "NateFunctions.h"

#include <algorithm>

Object::Object()
	: Type(),
		mRecords(mTypes)
{
}

Object::Object(const std::string& aType, const TypePtr& aBaseType)
	: Type(aType, aBaseType),
		mRecords(mTypes)
{
}

std::stringstream& Object::getImplOut()      { return mImplOut; }
std::stringstream& Object::getNormalOut()    { return mNormalOut; }

ObjectPtr          Object::getBase()         { return mBase; }
void Object::setBase(const ObjectPtr& aBase) { mBase = aBase; }

std::ostream& operator<<(std::ostream& aStream, const Object& aValue)
{
	aStream << "Object(" 
		    << static_cast<Type>(aValue) << ",";
	
	aStream << ")";
	return aStream;
}