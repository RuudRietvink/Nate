
#include "Type.h"
#include "Core/Core.h"

#include <iostream>

Type::Type()
	: Type("")
{
}

Type::Type(const std::string& aType, const TypePtr& aBaseType)
	: mName(aType)
{
	setBaseType(aBaseType);
	setType(aType);
}

void Type::setType(const std::string& aType)
{
	if (aType == "any")
	{
		setFlag(Abstract, true);
		setFlag(Any, true);
	}
	else if (aType == "number")
	{
		setFlag(Abstract, true);
		setFlag(Number, true);
		setFlag(Scalar, true);
		setFlag(Comparable, true);
	}
	else if (aType == "integer")
	{
		setFlag(Integer, true);
	}
	else if (aType == "real")
	{
		setFlag(Real, true);
	}
	else if (aType == "int-8")
	{
		setFlag(Abstract, false);
		mCodeType = "int8_t";
		mBitSize = 8;
	}
	else if (aType == "int-16")
	{
		setFlag(Abstract, false);
		mCodeType = "int16_t";
		mBitSize = 16;
	}
	else if (aType == "int-32")
	{
		setFlag(Abstract, false);
		mCodeType = "int32_t";
		mBitSize = 32;
	}
	else if (aType == "int-64")
	{
		setFlag(Abstract, false);
		mCodeType = "int64_t";
		mBitSize = 64;
	}
	else if (aType == "float-32")
	{
		setFlag(Abstract, false);
		mCodeType = "float";
		mBitSize = 132;
	}
	else if (aType == "float-64")
	{
		setFlag(Abstract, false);
		mCodeType = "double";
		mBitSize = 164;
	}
	else if (aType == "boolean")
	{
		setFlag(Abstract, false);
		setFlag(Boolean, true);
		setFlag(Scalar, true);
		mCodeType = "double";
		mCodeType = "bool";
		mBitSize = 232;
	}
	else if (aType == "char")
	{
		setFlag(Abstract, false);
		setFlag(Char, true);
		setFlag(Comparable, true);
		mCodeType = "uint32_t";
		mBitSize = 332;
	}
	else if (aType == "text")
	{
		setFlag(Abstract, false);
		setFlag(Text, true);
		setFlag(Comparable, true);
		setFlag(NeedsRef, true);
		mCodeType = "std::string";
		mBitSize = 1000;
	}
	else if (aType == "record")
	{
		setFlag(Record, true);
		setFlag(NeedsRef, true);
		mCodeType = "struct";
		mBitSize = 2000;
	}
	else if (aType == "container")
	{
		setFlag(Container, true);
		setFlag(NeedsRef, true);
		mBitSize = 3000;
	}
	else if (aType == "sequence-container")
	{
	}
	else if (aType == "list")
	{
		setFlag(Abstract, false);
		setFlag(List, true);
		mCodeType = "std::list";
	}
	else
	{
		setFlag(Unknown, true);
		mCodeType = "unknown";
		mBitSize = 0;
	}
}

void Type::setBaseType(const TypePtr& aType)
{
	mBaseType = aType;
	if (mBaseType)
	{
		setFlags(mBaseType->getFlags());
		mBitSize = mBaseType->mBitSize;
		mCodeType = mBaseType->mCodeType;
	}
}

bool Type::is(size_t aFlags) const
{
	return WithFlags::is(aFlags);
}

bool Type::isOfType(const std::string& aType) const
{
	return mName == aType || 
				 (mBaseType && mBaseType->isOfType(aType));
}

bool Type::canBeCastedFrom(const TypePtr& aType) const
{
	bool result = false;
	
	if (aType->isOfType(name()))
	{
		result = true;
	}
	else if (is(Number) && aType->is(Number))
	{
		result = true;
	}
	else if (is(Char) && aType->is(Integer))
	{
		result = true;
	}
	else if (is(Integer) && aType->is(Char))
	{
		result = true;
	}
	else if (is(Text) && aType->is(Char))
	{
		result = true;
	}
	else if (is(Text) && aType->is(Text))
	{
		result = true;
	}
	else if (is(Text) && aType->is(Number))
	{
		result = true;
	}
	else if (is(Text) && aType->is(Boolean))
	{
		result = true;
	}

	return result;
}

bool Type::isBiggerThan(const TypePtr& aType) const
{
	bool result = false;

	if (bitSize() > aType->bitSize())
	{
		result = true;
	}
	else if (is(Real) && (!aType->is(Real) && aType->is(Number)))
	{
		result = true;
	}

	return result;
}

bool               Type::empty()     const { return mName.empty(); }
const std::string& Type::name()      const { return mName; }
int                Type::bitSize()   const { return mBitSize; }
const TypePtr&		 Type::typenameType() const { return mTypenameType; }
const TypePtr&		 Type::baseType()  const { return mBaseType; }

void Type::setTypenameType(const TypePtr& aTypenameType) { mTypenameType = aTypenameType; }
void Type::setCodeType(const std::string& aCodeType) { mCodeType = aCodeType; }


std::string Type::codeType() const
{ 
	std::string result;

	if (!mTypenameType || is(Text))
	{
	  result = mCodeType;
	}
	else
	{
		result = mCodeType + "<" + mTypenameType->codeType() + ">";
	}

	return result;
}

std::ostream& operator<<(std::ostream& aStream, const Type& aValue)
{
	return aValue.print(aStream);
}

std::ostream& Type::print(std::ostream& aStream) const
{
	aStream << "Type(" << name() << "," << codeType() << "," << bitSize();
	if (mBaseType) aStream << ",BaseType(" << mBaseType->name() << ")";
	if (is(Type::Any))  aStream << ",Any";
	if (is(Type::Number))  aStream << ",Number";
	if (is(Type::Real))   aStream << ",Float";
	if (is(Type::Integer))   aStream << ",Integer";
	if (is(Type::Text))  aStream << ",Text";
	if (is(Type::Boolean)) aStream << ",Boolean";
	if (is(Type::Unknown)) aStream << ",Unknown";
	if (is(Type::Scalar)) aStream << ",Scalar";
	if (is(Type::Comparable)) aStream << ",Comparable";
	if (is(Type::NeedsRef)) aStream << ",NeedsRef";
	if (is(Type::Record)) aStream << ",Record";
	if (is(Type::Container)) aStream << ",Container";
	if (is(Type::Abstract)) aStream << ",Abstract";
	if (is(Type::List)) aStream << ",List";
	if (mTypenameType) {
		aStream << "TypenameType(" << *mTypenameType << "),";
	}

	aStream << ")";
	return aStream;
}