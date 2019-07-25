
#include "ExprNode.h"

#include <iostream>

ExprNode::ExprNode()
	: ExprNode("", "", Type())
{
}

ExprNode::ExprNode(const std::string& aWord)
	: ExprNode(aWord, "", Type())
{
	setFlag(Word, true);
	setFlag(ConstExpr, true);
}

ExprNode::ExprNode(const std::string& aText, const Type& aType)
	: ExprNode(aText, "", aType)
{
}

ExprNode::ExprNode(const std::string& aText, const std::string& aCode, const Type& aType)
	: mText(aText),
	mCode(aCode),
	mType(aType)
{
}

bool ExprNode::castToType(const Type& aToType)
{
	bool ok = true;

	if (type().is(Type::Number) && aToType.is(Type::Number))
	{
		if (type().is(Type::Float) && !aToType.is(Type::Float))
		{
			*this = ExprNode(text(), "static_cast<" + aToType.codeType() + ">(" + code() + ")", aToType);
		}
		else if (!type().is(Type::Float) && aToType.is(Type::Float))
		{
			*this = ExprNode(text(), "static_cast<" + aToType.codeType() + ">(" + code() + ")", aToType);
		}
		else if (type().bitSize() > aToType.bitSize())
		{
			*this = ExprNode(text(), "static_cast<" + aToType.codeType() + ">(" + code() + ")", aToType);
		}
	}
	else if (type().is(Type::Number) && aToType.is(Type::Text))
	{
			*this = ExprNode(text(), "std::to_string(" + code() + ")", aToType);
  }
	else if (type().is(Type::Boolean) && aToType.is(Type::Text))
	{
			*this = ExprNode(text(), "(" + code() + "?\"true\":\"false\")", aToType);
  }
	else if (type().name() != aToType.name())
	{
		ok = false;
	}

	return ok;
}

const std::string& ExprNode::text()            const { return mText; }
const std::string& ExprNode::code()            const { return mCode; }
const Type&        ExprNode::type()            const { return mType; }
bool               ExprNode::isEmpty()         const { return mText.empty(); }

std::ostream& operator<<(std::ostream& aStream, const ExprNode& aValue)
{
	aStream << "ExprNode(" << aValue.text() << "," << aValue.code() << "," << aValue.type();
	if (aValue.is(ExprNode::Word)) aStream << ",Word";
	if (aValue.is(ExprNode::Literal)) aStream << ",Literal";
	if (aValue.is(ExprNode::Output)) aStream << ",Output";
	if (aValue.is(ExprNode::ConstExpr)) aStream << ",ConstExpr";
	if (aValue.is(ExprNode::Default)) aStream << ",Default";
	aStream << ")";
	return aStream;
}
