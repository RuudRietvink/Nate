
#include "Type.h"
#include "Core/Core.h"

#include <iostream>

Type::Type()
	: Type("")
{
}

Type::Type(const std::string& aName, const TypePtr& aBaseType)
	: mName(aName)
{
	setBaseType(aBaseType);
	setType(aName);
}

void Type::setType(const std::string& aName)
{
	if (aName == "any")
	{
		setFlag(Abstract, true);
		setFlag(Any, true);
	}
	else if (aName == "number")
	{
		setFlag(Abstract, true);
		setFlag(Number, true);
		setFlag(Scalar, true);
		setFlag(Comparable, true);
	}
	else if (aName == "integer")
	{
		setFlag(Integer, true);
		setFlag(SingleNr, true);
	}
	else if (aName == "real")
	{
		setFlag(Real, true);
		setFlag(SingleNr, true);
	}
	else if (aName == "int-8")
	{
		setFlag(Abstract, false);
		mCodeType = "int8_t";
		mBitSize = 8;
	}
	else if (aName == "int-16")
	{
		setFlag(Abstract, false);
		mCodeType = "int16_t";
		mBitSize = 16;
	}
	else if (aName == "int-32")
	{
		setFlag(Abstract, false);
		mCodeType = "int32_t";
		mBitSize = 32;
	}
	else if (aName == "int-64")
	{
		setFlag(Abstract, false);
		mCodeType = "int64_t";
		mBitSize = 64;
	}
	else if (aName == "float-32")
	{
		setFlag(Abstract, false);
		mCodeType = "float";
		mBitSize = 132;
	}
	else if (aName == "float-64")
	{
		setFlag(Abstract, false);
		mCodeType = "double";
		mBitSize = 164;
	}
	else if (aName == "rational")
	{
		setFlag(Abstract, false);
		setFlag(Rational, true);
		mCodeType = "Rational";
		mBitSize = 264;
	}
	else if (aName == "imaginary")
	{
		setFlag(Abstract, false);
		setFlag(Imaginary, true);
		setFlag(Template, true);
		mCodeType = "";
		mBitSize = 265;
	}
	else if (aName == "complex")
	{
		setFlag(Abstract, false);
		setFlag(Complex, true);
		setFlag(NeedsRef, true);
		setFlag(Template, true);
		mCodeType = "Complex";
		mBitSize = 267;
	}
	else if (aName == "boolean")
	{
		setFlag(Abstract, false);
		setFlag(Boolean, true);
		setFlag(Scalar, true);
		mCodeType = "double";
		mCodeType = "bool";
		mBitSize = 232;
	}
	else if (aName == "char")
	{
		setFlag(Abstract, false);
		setFlag(Char, true);
		setFlag(Comparable, true);
		mCodeType = "uint32_t";
		mBitSize = 332;
	}
	else if (aName == "text")
	{
		setFlag(Abstract, false);
		setFlag(Text, true);
		setFlag(Comparable, true);
		setFlag(NeedsRef, true);
		setFlag(Object, false);
		mCodeType = "string_t";
		mBitSize = 1000;
	}
	else if (aName == "record")
	{
		setFlag(Record, true);
		setFlag(NeedsRef, true);
		mCodeType = "struct";
		mBitSize = 2000;
	}
	else if (aName == "object")
	{
		setFlag(Object, true);
		setFlag(NeedsRef, true);
		mCodeType = "";
		mBitSize = 2500;
	}
	else if (aName == "container")
	{
		setFlag(IsContainer, true);
		setFlag(NeedsRef, true);
		setFlag(Template, true);
		mBitSize = 3000;
	}
	else if (aName == "sequence-container")
	{
	}
	else if (aName == "list")
	{
		setFlag(Abstract, false);
		setFlag(List, true);
		mCodeType = "std::list";
	}
	else if (aName == "output")
	{
		setFlag(Abstract, false);
		setFlag(NeedsRef, true);
		mCodeType = "std::ostream";
	}
	else if (aName == "file-output")
	{
		setFlag(Abstract, false);
		setFlag(NeedsRef, true);
		mCodeType = "std::ofstream";
	}
	else if (aName == "input")
	{
		setFlag(Abstract, false);
		setFlag(NeedsRef, true);
		mCodeType = "std::istream";
	}
	else if (aName == "file-input")
	{
		setFlag(Abstract, false);
		setFlag(NeedsRef, true);
		mCodeType = "std::ifstream";
	}
	else if (aName == "data-input")
	{
		setFlag(Abstract, false);
		setFlag(NeedsRef, true);
		mCodeType = "std::istringstream";
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
	return name() == aType || 
				 (mBaseType && mBaseType->isOfType(aType));
}

bool Type::isBiggerThan(const TypePtr& aType) const
{
	bool result = false;

	if (bitSize() > aType->bitSize())
	{
		result = true;
	}
	else if (is(Real) && (!aType->is(Real) && aType->is(Integer)))
	{
		result = true;
	}

	return result;
}

Type::CompareResult Type::canBeCastedFrom(const TypePtr& aType, bool needExactMatch) const
{
	CompareResult result = CompareResult::No;
	
	if (aType->name() == name())
	{
		result = CompareResult::Yes;
	}
	else if (!needExactMatch)
	{
		if (aType->isOfType(name()))
		{
			result = CompareResult::RequiresCast;
		}
		else if (is(Number) && aType->is(Number))
		{
			result = CompareResult::RequiresCast;
		}
		else if (is(Char) && aType->is(Integer))
		{
			result = CompareResult::RequiresCast;
		}
		else if (is(Integer) && aType->is(Char))
		{
			result = CompareResult::RequiresCast;
		}
		else if (is(Text) && aType->is(Char))
		{
			result = CompareResult::RequiresCast;
		}
		else if (is(Text) && aType->is(Text))
		{
			result = CompareResult::RequiresCast;
		}
		else if (is(Text) && aType->is(Number))
		{
			result = CompareResult::RequiresCast;
		}
		else if (is(Text) && aType->is(Boolean))
		{
			result = CompareResult::RequiresCast;
		}
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
		if (is(Object))
		{
			result = "std::shared_ptr<" + mCodeType + ">";
		}
		else
		{
			result = mCodeType;
		}
	}
	else
	{
		if (!mCodeType.empty())
		{
			result = mCodeType + "<" + mTypenameType->codeType() + ">";
		}
		else
		{
			result = mTypenameType->codeType();
		}
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
	if (is(Type::Char)) aStream << ",Char";
	if (is(Type::Rational)) aStream << ",Rational";
	if (is(Type::Imaginary)) aStream << ",Imaginary";
	if (is(Type::Complex)) aStream << ",Complex";
	if (is(Type::Record)) aStream << ",Record";
	if (is(Type::Unknown)) aStream << ",Unknown";
	if (is(Type::Scalar)) aStream << ",Scalar";
	if (is(Type::Abstract)) aStream << ",Abstract";
	if (is(Type::SingleNr)) aStream << ",SingleNr";
	if (is(Type::Object)) aStream << ",Object";
	if (is(Type::ObjectImpl)) aStream << ",ObjectImpl";
	if (is(Type::Comparable)) aStream << ",Comparable";
	if (is(Type::NeedsRef)) aStream << ",NeedsRef";
	if (is(Type::IsContainer)) aStream << ",IsContainer";
	if (is(Type::List)) aStream << ",List";
	if (is(Type::Template)) aStream << ",Template";
	if (mTypenameType) {
		aStream << "TypenameType(" << *mTypenameType << "),";
	}

	aStream << ")";
	return aStream;
}

TypePtr Types::get(const std::string& aName)
{
	return mTypes.getData(aName);
}

void Types::add(const TypePtr& aType, const std::string& aName)
{
	mTypes.addData(aType, aName);
}

bool Types::contains(const TypePtr& aType)
{
	return mTypes.contains(aType);
}