
#include "Type.h"
#include "Core/Core.h"

#include <iostream>

Type::Type()
	: Type("")
{
}

Type::Type(const std::string& aName)
	: Type(aName, aName)
{
}

Type::Type(const std::string& aType, const std::string& aName)
	: mName(aName)
{
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
	else if (aType == "int-32" || aType == "int")
	{
		setFlag(Scalar, true);
		mName = "int-32";
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
	else if (aType == "float-32" || aType == "float")
	{
		setFlag(Scalar, true);
		mName = "float-32";
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
	else if (aType == "bool" || aType == "boolean")
	{
		setFlag(Scalar, true);
    mName = "boolean";
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
		setBaseType("container");
	}
	else if (aType == "list")
	{
		setBaseType("sequence-container");
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

void Type::setBaseType(const std::string& aType)
{
	mBaseType = std::make_shared<Type>(aType);
	setFlags(mBaseType->getFlags());
	mBitSize = mBaseType->mBitSize;
	mCodeType = mBaseType->mCodeType;
}

bool Type::isOfType(const std::string& aType) const
{
	return mName == aType || 
				 (mBaseType && mBaseType->isOfType(aType));
}

TypePtr Type::makeType(const std::string& aValue)
{
	std::string type = "";

	if (aValue[0] == '"')
	{
		type = "text";
	}
	else if (aValue == "false" || aValue == "true")
	{
		type = "bool";
	}
	else if (aValue.find('.') != std::string::npos)
	{
		/*double value;
		Core::strtodbl(aValue.c_str(), value);
		if (value > std::numeric_limits<float>::max() || 
		    (value < 0 && value < std::numeric_limits<float>::lowest()) ||
		    (value > 0 && value < std::numeric_limits<float>::min()))
		{
		  type = "float-64";
		}
		else
		{
		  type = "float-32";
		}*/
		type = "float-64";
	}
	else
	{
		int64_t value;
		Core::strtoi64(aValue.c_str(), value);
		if (value > std::numeric_limits<int32_t>::max() || value < std::numeric_limits<int32_t>::lowest())
		{
			type = "int-64";
		}
		else
		{
			type = "int-32";
		}
	}

	return std::make_shared<Type>(type);
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