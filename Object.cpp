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

const std::vector<ObjectPtr>& Object::getBases() { return mBases; }
void Object::addBase(const ObjectPtr& aBase) { mBases.push_back(aBase); }

bool Object::isOfType(const std::string& aType) const
{
	return Type::isOfType(aType) ||
		     std::any_of(mBases.begin(), mBases.end(), 
	                   [&](const ObjectPtr& aBase) 
										 { return aBase->name() == aType || aBase->isOfType(aType); } );
}

DefinePtr Object::basesGetLike(const DefinePtr& aDefine)
{
	DefinePtr result;

	for (auto base = mBases.begin(); base != mBases.end() && !result; ++base)
	{
		result = (*base)->defines().getLike(aDefine);
		if (!result)
		{
			result = (*base)->basesGetLike(aDefine);
		}
	}

	return result;
}

void Object::addProp(const IdentifierPtr& anId, const yy::parser::location_type& aLocation, const std::string& filename)
{
	PropData prop;
	prop.location = aLocation;
	prop.filename = filename;
	prop.states[(int)PropType::Get] = PropState{ PropState::State::Declared, false };
	prop.states[(int)PropType::Set] = PropState{ (anId->is(Identifier::ReadOnly) 
		                                            ? PropState::State::Unknown
																					 		  : PropState::State::Declared), false };
	for (auto propType : { PropType::Get, PropType::Set })	
	{
		if (basesIsPropDeclared(anId, propType))
		{
			prop.states[(int)propType].overriden = true;
		}
	}

	mPropertyMethods.emplace(anId, prop);
} 

Object::PropState Object::getPropState(const IdentifierPtr& anId, PropType aPropType) const
{
	PropState result = PropState { PropState::State::Unknown, false };

	auto iter = std::find_if(mPropertyMethods.begin(), mPropertyMethods.end(),
													 [&](const auto& aPair) { return anId->name() == aPair.first->name(); });
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
	return getPropState(anId, aPropType).state != PropState::State::Unknown;
}

bool Object::basesIsPropDeclared(const IdentifierPtr& anId, PropType aPropType) const
{
	return std::any_of(mBases.begin(), mBases.end(), 
	                   [&](const ObjectPtr& aBase) 
										 { return aBase->isPropDeclared(anId, aPropType) || 
										          aBase->basesIsPropDeclared(anId, aPropType); } );
}

bool Object::isPropDefined(const IdentifierPtr& anId, PropType aPropType) const
{
	return getPropState(anId, aPropType).state == PropState::State::Defined;
}

bool Object::isRole() const
{
	return mIsRole;
}

void Object::setIsRole(bool aIsRole)
{
	mIsRole = aIsRole;
}

std::ostream& operator<<(std::ostream& aStream, const Object& aValue)
{
	aStream << "Object(" 
		    << static_cast<Type>(aValue) << ",";
	
	if (aValue.isRole()) aStream << ",isRole";
	aStream << ")";
	return aStream;
}