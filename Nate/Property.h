#pragma once

#include "Define.h"

namespace nate
{
class Property;

typedef std::shared_ptr<Property> PropertyPtr;

class Property : public Define
{
public:
	enum class PropType
	{
		Get = 0,
		Set = 1
	};

	Property();
	virtual ~Property() = default;
  
	PropType getPropType() const;
	void setPropType(PropType aPropType);

private:
	PropType mPropType = PropType::Get;
};
std::ostream& operator<<(std::ostream& aStream, const Property& aValue);

}
