#pragma once

#include "Container.h"
#include "WithFlags.h"

#include <string>
#include <ostream>
#include <memory>

namespace nate
{
class Type;
typedef std::shared_ptr<Type> TypePtr;

class Type : public WithFlags
{
public:
	Type();
	Type(const std::string& aName, const TypePtr& aBaseType = TypePtr());
	DTOR_RULES_OF_X(Type)
	
	virtual bool is(size_t aFlags) const override;
	bool isBiggerThan(const TypePtr& aType) const;
	bool empty() const;
	void setType(const std::string& aType);
	bool isCodeVar() const;
	virtual std::ostream& print(std::ostream& aStream) const;
	virtual bool isOfType(const std::string& aType) const;

	virtual const std::string& name() const;
	
	void setTypeScopeName(const std::string& aTypeScopeName) { mTypeScopeName = aTypeScopeName; }
	const std::string& typeScopeName() const { return mTypeScopeName; }

	std::string         codeType() const;
	int                 bitSize() const;
	const TypePtr&	    typenameType() const;
	const TypePtr&		baseType() const;

	void				setTypenameType(const TypePtr& aTypenameType);
	void				setCodeType(const std::string& aCodeType);
	
	enum class CompareResult
	{
		No = 0,
		Yes = 1,
		RequiresCast = 2,
	};

	CompareResult canBeCastedFrom(const TypePtr& aType, bool needExactMatch = false) const;

	static const size_t Number     = 0;
	static const size_t Real       = 1;
	static const size_t Text       = 2;
	static const size_t Boolean    = 3;
	static const size_t Unknown    = 4;
	static const size_t Comparable = 5;
	static const size_t NeedsRef   = 6;
	static const size_t Record     = 7;
	static const size_t Scalar     = 8;
	static const size_t IsContainer= 9;
	static const size_t List       =10;
	static const size_t Integer    =11;
	static const size_t Abstract   =12;
	static const size_t Any        =13;
	static const size_t Char       =14;
	static const size_t Rational   =15;
	static const size_t Imaginary  =16;
	static const size_t Complex    =17;
	static const size_t SingleNr   =18;
	static const size_t Template   =19;
	static const size_t Object     =20;
	static const size_t ObjectImpl =21;
	static const size_t NoCode     =22;
	
private:
	void setBaseType(const TypePtr& aType);

	std::string mName;
	std::string mCodeType;
	int			mBitSize = 4;
	TypePtr	    mTypenameType;
	TypePtr	    mBaseType;
	std::string mTypeScopeName;
	bool        mIsCodeVar = false;
};

std::ostream& operator<<(std::ostream& aStream, const Type& aValue);

class Types
{
public:
	RULES_OF_X(Types)

	virtual TypePtr get(const std::string& aName);
	virtual void add(const TypePtr& aType, const std::string& aName = "");
	virtual bool contains(const TypePtr& aType);

private:			
	Container<TypePtr> mTypes;	
};

class ITypesHolder
{
public:
	virtual Types& types() = 0;
	virtual std::string typeScopeName() const = 0;

protected:
	RULES_OF_X(ITypesHolder)
};

typedef std::shared_ptr<ITypesHolder> ITypesHolderPtr;

}
