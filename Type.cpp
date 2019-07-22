
#include "Type.h"
#include "NateFunctions.h"

#include <iostream>

Type::Type()
	: Type("")
{
}

Type::Type(const std::string& aName)
	: mName(aName)
{
	if (aName == "text")
	{
		setFlag(String, true);
		setFlag(Comparable, true);
		mCodeType = "std::string";
		mBitSize = 1000;
	}
	else if (aName == "int-8")
	{
		setFlag(Number, true);
		setFlag(Comparable, true);
		mCodeType = "int8_t";
		mBitSize = 8;
	}
	else if (aName == "int-16")
	{
		setFlag(Number, true);
		setFlag(Comparable, true);
		mCodeType = "int16_t";
		mBitSize = 16;
	}
	else if (aName == "int-32")
	{
		setFlag(Number, true);
		setFlag(Comparable, true);
		mCodeType = "int32_t";
		mBitSize = 32;
	}
	else if (aName == "int-64")
	{
		setFlag(Number, true);
		setFlag(Comparable, true);
		mCodeType = "int64_t";
		mBitSize = 64;
	}
	else if (aName == "float-32")
	{
		setFlag(Number, true);
		setFlag(Comparable, true);
		setFlag(Float, true);
		mCodeType = "float";
		mBitSize = 32;
	}
	else if (aName == "float-64")
	{
		setFlag(Number, true);
		setFlag(Comparable, true);
		setFlag(Float, true);
		mCodeType = "double";
		mBitSize = 64;
	}
	else if (aName == "bool" || aName == "boolean")
	{
    mName = "boolean";
		setFlag(Boolean, true);
		setFlag(Comparable, true);
		mCodeType = "bool";
		mBitSize = 32;
	}
	else
	{
		setFlag(Unknown, true);
		mCodeType = "unknown";
		mBitSize = 0;
	}
}

Type Type::makeType(const std::string& aValue)
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
		strtodbl(aValue.c_str(), value);
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
		strtoi64(aValue.c_str(), value);
		if (value > std::numeric_limits<int32_t>::max() || value < std::numeric_limits<int32_t>::lowest())
		{
			type = "int-64";
		}
		else
		{
			type = "int-32";
		}
	}

	return Type(type);
}

bool Type::isCompatibleWith(const Type& aType) const
{
	bool result = false;

	if ( is(Number) && aType.is(Number))
	{
		result = true;
	}
	else if (is(String) && aType.is(Number))
	{
		result = true;
	}
	else if (is(String) && aType.is(Boolean))
	{
		result = true;
	}
	else if (name() == aType.name())
	{
		result = true;
	}

	return result;
}

bool Type::isBiggerThan(const Type& aType) const
{
	bool result = false;

	if (bitSize() > aType.bitSize())
	{
		result = true;
	}
	else if (is(Float) && (!aType.is(Float) && aType.is(Number)))
	{
		result = true;
	}

	return result;
}

const std::string& Type::name()            const { return mName; }
const std::string& Type::codeType()        const { return mCodeType; }
int                Type::bitSize()         const { return mBitSize; }


std::ostream& operator<<(std::ostream& aStream, const Type& aValue)
{
	aStream << "Type(" << aValue.name() << "," << aValue.codeType() << "," << aValue.bitSize();
	if (aValue.is(Type::Number))  aStream << ",Number";
	if (aValue.is(Type::Float))   aStream << ",Float";
	if (aValue.is(Type::String))  aStream << ",String";
	if (aValue.is(Type::Boolean)) aStream << ",Boolean";
	if (aValue.is(Type::Unknown)) aStream << ",Unknown";
	aStream << ")";
	return aStream;
}