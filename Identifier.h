#pragma once

#include "Expr.h"
#include "Type.h"

#include <string>
#include <ostream>

class Identifier
{
public:
	Identifier(const std::string& aName, const TypePtr& aType);
	Identifier(const std::string& aName, const TypePtr& aType, const Expr& aInitValue);
	virtual ~Identifier() = default;

	const std::string& name()      const;
	const std::string& codeName()  const;
	const Expr&        initValue() const;
	TypePtr            type()      const;

private:
	std::string mName;
	std::string mCodeName;
	Expr        mInitValue;
	TypePtr     mType;
};

typedef std::shared_ptr<Identifier> IdentifierPtr;

std::ostream& operator<<(std::ostream& aStream, const Identifier& aValue);
