
#include "Expr.h"
#include "Identifier.h"
#include "core/Core.h"

Expr::Expr()
	: Expr("", "", TypePtr())
{
}

Expr::Expr(const std::string& aWord)
	: Expr(aWord, aWord, TypePtr())
{
	setFlag(Word, true);
	setFlag(ConstExpr, true);
}

Expr::Expr(const IdentifierPtr& aId)
	: Expr(aId->name(), aId->codeName(), aId->type())
{
	mId = aId;
  setFlag(ConstExpr, mId->is(Identifier::Const));
  setFlag(Property, mId->is(Identifier::Property));
  setFlag(ObjectImpl, mId->is(Identifier::ObjectImpl));
  setFlag(Identifier);
}

Expr::Expr(const std::string& aText, const TypePtr& aType)
	: Expr(aText, aText, aType)
{
}

Expr::Expr(const std::string& aText, const std::string& aCode, const TypePtr& aType)
	: mText(aText),
	  mCode(aCode),
	  mType(aType)
{
}

Expr::Expr(const Expr& aExpr1, const Expr& aExpr2)
{
	*this = aExpr1;

	if (!aExpr2.isEmpty())
	{
		addNodes(aExpr2.nodes().begin(), aExpr2.nodes().end());
	}
}

Expr Expr::parenthesized(const Expr& aExpr)
{
	Expr result("(");
	result.addNodes(aExpr.nodes().begin(), aExpr.nodes().end());
	result.addNode(Expr(")"));
	return result;
}

void Expr::addNode(const Expr& aNode)
{
	mNodes.push_back(aNode);
}

void Expr::insertNode(const Expr& aNode)
{
	mNodes.insert(mNodes.begin(), aNode);
}

void Expr::addNodes(const ExprNodesCIter& aNodeBegin, const ExprNodesCIter& aNodeEnd)
{
	for (auto iter = aNodeBegin; iter != aNodeEnd; ++iter)
	{
		addNode(*iter);
	}
}

std::string Expr::text() const
{ 
	std::string result;
	if (mNodes.empty())
	{
		result = mText;
	}
	else
	{
		bool addSpace = false;
		for (auto& node : mNodes)
		{
			if (addSpace)
			{
				result += " ";
			}

			addSpace = true;
			result += node.text();
		}
	}

	return result;
}

bool Expr::castToType(const TypePtr& aToType)
{
	bool ok = true;

	if (type()->is(Type::Number) && aToType->is(Type::Number))
	{
		if (type()->is(Type::Rational) && aToType->is(Type::Real))
		{
			*this = Expr(text(), "static_cast<" + aToType->codeType() + ">(" + code() + ".toDouble())", aToType);
		}
		else if (type()->is(Type::Rational) && aToType->is(Type::Integer))
		{
			*this = Expr(text(), "static_cast<" + aToType->codeType() + ">(" + code() + ".toInt())", aToType);
		}
		else if (type()->is(Type::SingleNr) && aToType->is(Type::Rational))
		{
			*this = Expr(text(), "Rational(" + code() + ")", aToType);
		}
		else if (type()->is(Type::Imaginary) && aToType->is(Type::Complex))
		{
			*this = Expr(text(), aToType->codeType() + "(0, " + code() + ")", aToType);
		}
		else if (type()->is(Type::Number) && aToType->is(Type::Imaginary))
		{
			setFlag(Type::Imaginary);
		}
		else if (type()->is(Type::SingleNr) && aToType->is(Type::Complex))
		{
			*this = Expr(text(), aToType->codeType() + "(" + code() + ", 0)", aToType);
		}
		else if (!aToType->is(Type::Abstract))
		{
			if (type()->is(Type::Real) && !aToType->is(Type::Real))
			{
				*this = Expr(text(), "static_cast<" + aToType->codeType() + ">(" + code() + ")", aToType);
			}
			else if (!type()->is(Type::Real) && aToType->is(Type::Real))
			{
				*this = Expr(text(), "static_cast<" + aToType->codeType() + ">(" + code() + ")", aToType);
			}
			else if (type()->isBiggerThan(aToType))
			{
				*this = Expr(text(), "static_cast<" + aToType->codeType() + ">(" + code() + ")", aToType);
			}
		}
	}
	else if (type()->is(Type::Number) && aToType->is(Type::Text))
	{
		*this = Expr(text(), "std::to_string(" + code() + ")", aToType);
  }
	else if (type()->is(Type::Boolean) && aToType->is(Type::Text))
	{
		*this = Expr(text(), "(" + code() + "?\"true\":\"false\")", aToType);
  }
	else if (type()->is(Type::Char) && aToType->is(Type::Text))
	{
		*this = Expr(text(), "Core::toString(" + code() + ")", aToType);
  }
	else if (type()->is(Type::Char) && aToType->is(Type::Integer))
	{
		*this = Expr(text(), "static_cast<" + aToType->codeType() + ">(" + code() + ")", aToType);
  }
	else if (type()->is(Type::Integer) && aToType->is(Type::Char))
	{
		*this = Expr(text(), "static_cast<" + aToType->codeType() + ">(" + code() + ")", aToType);
  }
	else if (!type()->isOfType(aToType->name()))
	{
		ok = false;
	}

	return ok;
}

const std::string& Expr::code()			const { return mCode; }
std::string&       Expr::code()						{ return mCode; }
TypePtr            Expr::type()			const { return mType; }
bool               Expr::isEmpty()	const { return text().empty(); }
IdentifierPtr      Expr::id()	  		const { return mId; }

const std::vector<Expr>& Expr::nodes() const { return mNodes; }

std::ostream& operator<<(std::ostream& aStream, const Expr& aValue)
{
	aStream << "Expr(";
	if (aValue.is(Expr::Word)) aStream << ",Word";
	if (aValue.is(Expr::Literal)) aStream << ",Literal";
	if (aValue.is(Expr::Output)) aStream << ",Output";
	if (aValue.is(Expr::ConstExpr)) aStream << ",ConstExpr";
	if (aValue.is(Expr::Default)) aStream << ",Default";
	if (aValue.is(Expr::Property)) aStream << ",Property";
	if (aValue.is(Expr::Identifier)) aStream << ",Identifier";
	if (aValue.is(Expr::ObjectImpl)) aStream << ",ObjectImpl";
		
	aStream << aValue.text() << "," << aValue.code() << "," << (aValue.type() ? *aValue.type() : Type());

	aStream << " Nodes(";
	for (const Expr& node : aValue.nodes())
	{
		aStream << node;
	}
	aStream << "))";

	return aStream;
}