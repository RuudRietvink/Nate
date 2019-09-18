#pragma once

#include "Container.h"
#include "Expr.h"
#include "Type.h"
#include "WithFlags.h"

#include <list>
#include <string>
#include <ostream>
#include <memory>

class Scope;
class Identifier;
typedef std::shared_ptr<Identifier> IdentifierPtr;

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
	
	static const size_t Const        = 0;
	static const size_t Property     = 1;

private:
	std::string						mName;
	std::string						mCodeName;
	Expr									mInitValue;
	TypePtr								mType;
	std::weak_ptr<Scope>	mScope;
};


std::ostream& operator<<(std::ostream& aStream, const Identifier& aValue);

class Identifiers
{
public:
	virtual IdentifierPtr get(const std::string& aName);
	virtual void add(const IdentifierPtr& aIdentifier);
	virtual const std::list<IdentifierPtr>& get() const;

private:					
	Container<IdentifierPtr> mIds;
	std::list<IdentifierPtr> mIdentifierList;	
};

class IIdentifiersHolder
{
public:
	virtual Identifiers& identifiers() = 0;
};
typedef std::shared_ptr<IIdentifiersHolder> IIdentifiersHolderPtr;
