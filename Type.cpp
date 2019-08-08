
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
	if (aType == "text")
	{
		setFlag(Text, true);
		setFlag(Comparable, true);
		setFlag(NeedsRef, true);
		mCodeType = "std::string";
		mBitSize = 1000;
	}
	else if (aType == "int-8")
	{
		setFlag(Scalar, true);
		setFlag(Number, true);
		setFlag(Comparable, true);
		mCodeType = "int8_t";
		mBitSize = 8;
	}
	else if (aType == "int-16")
	{
		setFlag(Scalar, true);
		setFlag(Number, true);
		setFlag(Comparable, true);
		mCodeType = "int16_t";
		mBitSize = 16;
	}
	else if (aType == "int-32")
	{
		setFlag(Scalar, true);
		setFlag(Number, true);
		setFlag(Comparable, true);
		mCodeType = "int32_t";
		mBitSize = 32;
	}
	else if (aType == "int-64")
	{
		setFlag(Scalar, true);
		setFlag(Number, true);
		setFlag(Comparable, true);
		mCodeType = "int64_t";
		mBitSize = 64;
	}
	else if (aType == "float-32")
	{
		setFlag(Scalar, true);
		setFlag(Number, true);
		setFlag(Comparable, true);
		setFlag(Float, true);
		mCodeType = "float";
		mBitSize = 32;
	}
	else if (aType == "float-64")
	{
		setFlag(Scalar, true);
		setFlag(Number, true);
		setFlag(Comparable, true);
		setFlag(Float, true);
		mCodeType = "double";
		mBitSize = 64;
	}
	else if (aType == "boolean")
	{
		setFlag(Scalar, true);
		setFlag(Boolean, true);
		setFlag(Comparable, true);
		mCodeType = "bool";
		mBitSize = 32;
	}
	else if (aType == "record")
	{
		setFlag(NoOutput, true);
		setFlag(Record, true);
		setFlag(NeedsRef, true);
		mCodeType = "struct";
		mBitSize = 2000;
	}
	else if (aType == "container")
	{
		setFlag(NoOutput, true);
		setFlag(Container, true);
		setFlag(NeedsRef, true);
		mCodeType = "";
		mBitSize = 3000;
	}
	else if (aType == "sequence-container")
	{
	}
	else if (aType == "list")
	{
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

bool Type::isOfType(const std::string& aType) const
{
	return mName == aType || 
				 (mBaseType && mBaseType->isOfType(aType));
}

bool Type::isCompatibleWith(const TypePtr& aType) const
{
	bool result = false;

	if ( is(Number) && aType->is(Number))
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
	else if (aType->isOfType(name()))
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
	else if (is(Float) && (!aType->is(Float) && aType->is(Number)))
	{
		result = true;
	}

	return result;
}

bool               Type::empty()     const { return mName.empty(); }
const std::string& Type::name()      const { return mName; }
int                Type::bitSize()   const { return mBitSize; }
const TypePtr&		 Type::childType() const { return mChildType; }

void Type::setChildType(const TypePtr& aChildType) { mChildType = aChildType; }
void Type::setCodeType(const std::string& aCodeType) { mCodeType = aCodeType; }


std::string Type::codeType() const
{ 
	std::string result;

	if (!mChildType)
	{
	  result = mCodeType;
	}
	else
	{
		result = mCodeType + "<" + mChildType->codeType() + ">";
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
	if (is(Type::Number))  aStream << ",Number";
	if (is(Type::Float))   aStream << ",Float";
	if (is(Type::Text))  aStream << ",Text";
	if (is(Type::Boolean)) aStream << ",Boolean";
	if (is(Type::Unknown)) aStream << ",Unknown";
	if (is(Type::Scalar)) aStream << ",Scalar";
	if (is(Type::Comparable)) aStream << ",Comparable";
	if (is(Type::NeedsRef)) aStream << ",NeedsRef";
	if (is(Type::Record)) aStream << ",Record";
	if (is(Type::Container)) aStream << ",Container";
	if (is(Type::List)) aStream << ",List";
	if (mChildType) {
		aStream << "ChildType(" << *mChildType << "),";
	}

	aStream << ")";
	return aStream;
}