
#include "ExprNode.h"

#include <iostream>

ExprNode::ExprNode()
	: ExprNode("", "", TypePtr())
{
}

ExprNode::ExprNode(const std::string& aWord)
	: ExprNode(aWord, aWord, TypePtr())
{
	setFlag(Word, true);
	setFlag(ConstExpr, true);
}

ExprNode::ExprNode(const std::string& aText, const TypePtr& aType)
	: ExprNode(aText, aText, aType)
{
}

ExprNode::ExprNode(const std::string& aText, const std::string& aCode, const TypePtr& aType)
	: mText(aText),
	mCode(aCode),
	mType(aType)
{
}

bool ExprNode::castToType(const TypePtr& aToType)
{
	bool ok = true;

	if (type()->is(Type::Number) && aToType->is(Type::Number))
	{
		if (type()->is(Type::Fraction) && aToType->is(Type::Real))
		{
			*this = ExprNode(text(), "static_cast<" + aToType->codeType() + ">(" + code() + ".toDouble())", aToType);
		}
		else if (type()->is(Type::Fraction) && aToType->is(Type::Integer))
		{
			*this = ExprNode(text(), "static_cast<" + aToType->codeType() + ">(" + code() + ".toInt())", aToType);
		}
		else if (type()->is(Type::SingleNr) && aToType->is(Type::Fraction))
		{
			*this = ExprNode(text(), "Fraction(" + code() + ")", aToType);
		}
		else if (type()->is(Type::Imaginary) && aToType->is(Type::Complex))
		{
			*this = ExprNode(text(), "Complex<double>(0, " + code() + ")", aToType);
		}
		else if (type()->is(Type::Number) && aToType->is(Type::Imaginary))
		{
			setFlag(Type::Imaginary);
		}
		else if (type()->is(Type::SingleNr) && aToType->is(Type::Complex))
		{
			*this = ExprNode(text(), "Complex<double>(" + code() + ", 0)", aToType);
		}
		else if (!aToType->is(Type::Abstract))
		{
			if (type()->is(Type::Real) && !aToType->is(Type::Real))
			{
				*this = ExprNode(text(), "static_cast<" + aToType->codeType() + ">(" + code() + ")", aToType);
			}
			else if (!type()->is(Type::Real) && aToType->is(Type::Real))
			{
				*this = ExprNode(text(), "static_cast<" + aToType->codeType() + ">(" + code() + ")", aToType);
			}
			else if (type()->bitSize() > aToType->bitSize())
			{
				*this = ExprNode(text(), "static_cast<" + aToType->codeType() + ">(" + code() + ")", aToType);
			}
		}
	}
	else if (type()->is(Type::Number) && aToType->is(Type::Text))
	{
		*this = ExprNode(text(), "std::to_string(" + code() + ")", aToType);
  }
	else if (type()->is(Type::Boolean) && aToType->is(Type::Text))
	{
		*this = ExprNode(text(), "(" + code() + "?\"true\":\"false\")", aToType);
  }
	else if (type()->is(Type::Char) && aToType->is(Type::Text))
	{
		*this = ExprNode(text(), "Core::toString(" + code() + ")", aToType);
  }
	else if (type()->is(Type::Char) && aToType->is(Type::Integer))
	{
		*this = ExprNode(text(), "static_cast<" + aToType->codeType() + ">(" + code() + ")", aToType);
  }
	else if (type()->is(Type::Integer) && aToType->is(Type::Char))
	{
		*this = ExprNode(text(), "static_cast<" + aToType->codeType() + ">(" + code() + ")", aToType);
  }
	else if (!type()->isOfType(aToType->name()))
	{
		ok = false;
	}

	return ok;
}

const std::string& ExprNode::text()            const { return mText; }
const std::string& ExprNode::code()            const { return mCode; }
TypePtr            ExprNode::type()            const { return mType; }
bool               ExprNode::isEmpty()         const { return mText.empty(); }

std::ostream& operator<<(std::ostream& aStream, const ExprNode& aValue)
{
	aStream << "ExprNode(" << aValue.text() << "," << aValue.code() << "," << (aValue.type() ? *aValue.type() : Type());
	if (aValue.is(ExprNode::Word)) aStream << ",Word";
	if (aValue.is(ExprNode::Literal)) aStream << ",Literal";
	if (aValue.is(ExprNode::Output)) aStream << ",Output";
	if (aValue.is(ExprNode::ConstExpr)) aStream << ",ConstExpr";
	if (aValue.is(ExprNode::Default)) aStream << ",Default";
	aStream << ")";
	return aStream;
}
