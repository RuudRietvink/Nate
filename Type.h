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
	
	virtual bool is(size_t aFlags) const override;
	bool empty() const;
	void setType(const std::string& aType);
	bool isBiggerThan(const TypePtr& aType) const;
	virtual std::ostream& print(std::ostream& aStream) const;
	bool isOfType(const std::string& aType) const;

	const std::string& name() const;
	std::string        codeType() const;
	int                bitSize() const;
	const TypePtr&		 typenameType() const;
	const TypePtr&		 baseType() const;

	void							 setTypenameType(const TypePtr& aTypenameType);
	void							 setCodeType(const std::string& aCodeType);
	
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
	static const size_t Container  = 9;
	static const size_t List       =10;
	static const size_t Integer    =11;
	static const size_t Abstract   =12;
	static const size_t Any        =13;
	static const size_t Char       =14;
	static const size_t Fraction   =15;
	static const size_t Imaginary  =16;
	static const size_t Complex    =17;
	static const size_t SingleNr   =18;

private:
	void setBaseType(const TypePtr& aType);

	std::string mName;
	std::string mCodeType;
	int			    mBitSize = 4;
	TypePtr	    mTypenameType;
	TypePtr	    mBaseType;
};

std::ostream& operator<<(std::ostream& aStream, const Type& aValue);
