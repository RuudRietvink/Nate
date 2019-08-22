#pragma once

#include "Expr.h"
#include "Type.h"
#include "WithFlags.h"

#include <string>
#include <ostream>
#include <memory>

class Scope;

class Identifier : public WithFlags
{
public:
	Identifier(const std::shared_ptr<Scope>& aScope, const std::string& aName, const TypePtr& aType);
	Identifier(const std::shared_ptr<Scope>& aScope, const std::string& aName, const TypePtr& aType, const Expr& aInitValue);
	virtual ~Identifier() = default;

	const std::string&	 name()      const;
	const std::string&	 codeName()  const;
	const Expr&					 initValue() const;
	TypePtr							 type()      const;
	std::weak_ptr<Scope> scope()     const;
	bool							   isObjectMe()const;
	
	static const size_t Const      = 0;

private:
	std::string						mName;
	std::string						mCodeName;
	Expr									mInitValue;
	TypePtr								mType;
	std::weak_ptr<Scope>	mScope;
};

typedef std::shared_ptr<Identifier> IdentifierPtr;

std::ostream& operator<<(std::ostream& aStream, const Identifier& aValue);
