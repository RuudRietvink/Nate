#pragma once

#include "WithFlags.h"

#include <string>
#include <ostream>
#include <memory>

class Type;

typedef std::shared_ptr<Type> TypePtr;

class Type : public WithFlags
{
public:
	Type();
	Type(const std::string& aType, const TypePtr& aBaseType = TypePtr());
	virtual ~Type() = default;

	bool empty() const;
	void setType(const std::string& aType);
	bool isBiggerThan(const TypePtr& aType) const;
	bool isCompatibleWith(const TypePtr& aType) const;
	virtual std::ostream& print(std::ostream& aStream) const;
	bool isOfType(const std::string& aType) const;

	const std::string& name() const;
	std::string        codeType() const;
	int                bitSize() const;
	const TypePtr&		 childType() const;

	void							 setChildType(const TypePtr& aChildType);
	void							 setCodeType(const std::string& aCodeType);
	
	static const size_t Number     = 0;
	static const size_t Float      = 1;
	static const size_t Text       = 2;
	static const size_t Boolean    = 3;
	static const size_t Unknown    = 4;
	static const size_t Comparable = 5;
	static const size_t NeedsRef   = 6;
	static const size_t Record     = 7;
	static const size_t Scalar     = 8;
	static const size_t Container  = 9;
	static const size_t List       =10;
	static const size_t NoOutput   =11;

private:
	void setBaseType(const TypePtr& aType);

	std::string mName;
	std::string mCodeType;
	int			    mBitSize = 4;
	TypePtr	    mChildType;
	TypePtr	    mBaseType;
};

std::ostream& operator<<(std::ostream& aStream, const Type& aValue);
