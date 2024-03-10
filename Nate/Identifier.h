#pragma once

#include "Container.h"
#include "Type.h"
#include "WithFlags.h"
#include "Expr.h"

#include <list>
#include <string>
#include <ostream>
#include <memory>

namespace nate
{
class Scope;
class Identifier;
typedef std::shared_ptr<Identifier> IdentifierPtr;
class IIdentifiersHolder;
typedef std::shared_ptr<IIdentifiersHolder> IIdentifiersHolderPtr;

class Identifier : public WithFlags
{
public:
	Identifier(const IIdentifiersHolderPtr& aIdentifiersHolder, const std::string& aName, const TypePtr& aType);
	Identifier(const IIdentifiersHolderPtr& aIdentifiersHolder, const std::string& aName, const TypePtr& aType, const Expr& aInitValue);
	virtual ~Identifier() = default;
	std::string setFlagString(const std::string& aFlag) override;

	const std::string&								name()							const;
	const std::string&								codeName()					const;
	Expr   				    								initValue()				  const;
	TypePtr														type()							const;
	std::weak_ptr<IIdentifiersHolder> identifiersHolder()	const;
	bool															isObjectMe()				const;
	bool															isProperty()				const;
	static bool					   						isNameMe(const std::string& aName);
	static std::string     						nameMe();
	
	static const size_t Const        = 0;
	static const size_t Property     = 1;
	static const size_t ReadOnly     = 2;
	static const size_t Final        = 3;
	static const size_t ObjectImpl   = 4;
	static const size_t Local        = 5;
	static const size_t Undeclared   = 6;
	static const size_t CodeVar      = 7;

private:
	std::string												mName;
	std::string												mCodeName;
	Expr    													mInitValue;
	TypePtr														mType;
	std::weak_ptr<IIdentifiersHolder>	mIdentifiersHolder;
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
	virtual IdentifierPtr getIdentifier(const std::string& aName) = 0;
	enum class ScopeFlag
	{
		Local = 1,
		ObjectImpl = 2,
		Member = 3,
		Global = 4,
	};
	virtual ScopeFlag scopeFlag() const = 0;
};

}
