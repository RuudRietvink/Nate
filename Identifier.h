#pragma once

#include "Expr.h"
#include "Type.h"

#include <string>
#include <ostream>

class Identifier
{
public:
	Identifier(const std::string& aName, const Type& aType);
	Identifier(const std::string& aName, const Type& aType, const Expr& aInitValue);
	virtual ~Identifier() = default;

	const std::string& name()      const;
	const std::string& codeName()  const;
	const Expr&        initValue() const;
	const Type&        type()      const;

private:
	std::string mName;
	std::string mCodeName;
	Expr        mInitValue;
	Type        mType;
};

std::ostream& operator<<(std::ostream& aStream, const Identifier& aValue);
