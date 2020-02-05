#include "Object.h"
#include "NateFunctions.h"

#include <algorithm>

Object::Object()
	: Record(),
		mRecords(mTypes)
{
}

Object::Object(const std::string& aName, const TypePtr& aBaseType)
	: Record(aName, aBaseType),
		mRecords(mTypes)
{
}

std::stringstream& Object::getImplOut()      { return mImplOut; }
std::stringstream& Object::getNormalOut()    { return mNormalOut; }

ObjectPtr          Object::getBase()         { return mBase; }
void Object::setBase(const ObjectPtr& aBase) { mBase = aBase; }

Object::PropState Object::getPropState(const IdentifierPtr& anId, PropType aPropType) const
{
	PropState result = PropState::Unknown;

	auto iter = mPropertyMethods.find(anId);
	if (iter != mPropertyMethods.cend())
	{
		result = iter->second.states[(int)aPropType];
	}

	return result;
}

void Object::setPropState(const IdentifierPtr& anId, PropType aPropType, PropState aPropState)
{
	mPropertyMethods[anId].states[(int)aPropType] = aPropState;
}

bool Object::isPropDeclared(const IdentifierPtr& anId, PropType aPropType) const
{
	return getPropState(anId, aPropType) != PropState::Unknown;
}

bool Object::isPropDefined(const IdentifierPtr& anId, PropType aPropType) const
{
	return getPropState(anId, aPropType) == PropState::Defined;
}

std::ostream& operator<<(std::ostream& aStream, const Object& aValue)
{
	aStream << "Object(" 
		    << static_cast<Type>(aValue) << ",";
	
	aStream << ")";
	return aStream;
}