
#include "Property.h"
#include "Identifier.h"
#include "NateFunctions.h"

#include <reflex/matcher.h>
#include <iostream>
#include <sstream>
#include <algorithm>

namespace nate
{
Property::Property()
{
}

Property::PropType Property::getPropType() const
{
	return mPropType;
}
void Property::setPropType(Property::PropType aPropType)
{
	mPropType = aPropType;
}

std::ostream& operator<<(std::ostream& aStream, const Property& aValue)
{
	aStream << "Property(";
	aStream << static_cast<int>(aValue.getPropType()) << ", ";
    operator<<(aStream, static_cast<const Define&>(aValue));
	return aStream;
}

}
