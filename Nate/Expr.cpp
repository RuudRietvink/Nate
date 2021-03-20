
#include "Expr.h"
#include "Identifier.h"
#include "core/Core.h"

Expr::Node::Node()
	: Node("", "", TypePtr())
{
}

Expr::Node::Node(const std::string& aWord)
	: Node(aWord, aWord, TypePtr())
{
	setFlag(Word, true);
	setFlag(ConstExpr, true);
}

Expr::Node::Node(const IdentifierPtr& aId)
	: Node(aId->name(), aId->codeName(), aId->type())
{
	mId = aId;
  setFlag(ConstExpr, mId->is(Identifier::Const));
  setFlag(Property, mId->is(Identifier::Property));
  setFlag(ObjectImpl, mId->is(Identifier::ObjectImpl));
  setFlag(Identifier);
}

Expr::Node::Node(const std::string& aText, const TypePtr& aType)
	: Node(aText, aText, aType)
{
}

Expr::Node::Node(const std::string& aText, const std::string& aCode, const TypePtr& aType)
	: mText(aText),
	  mCode(aCode),
	  mType(aType)
{
}

const std::string& Expr::Node::text()			const { return mText; }
const std::string& Expr::Node::code()			const { return mCode; }
std::string&       Expr::Node::code()						{ return mCode; }
TypePtr            Expr::Node::type()			const { return mType; }
IdentifierPtr      Expr::Node::id()	  		const { return mId; }

bool Expr::Node::castToType(const TypePtr& aToType)
{
	bool ok = true;

	if (type()->is(Type::Number) && aToType->is(Type::Number))
	{
		if (type()->is(Type::Rational) && aToType->is(Type::Real))
		{
			*this = Node(text(), "static_cast<" + aToType->codeType() + ">(" + code() + ".toDouble())", aToType);
		}
		else if (type()->is(Type::Rational) && aToType->is(Type::Integer))
		{
			*this = Node(text(), "static_cast<" + aToType->codeType() + ">(" + code() + ".toInt())", aToType);
		}
		else if (type()->is(Type::SingleNr) && aToType->is(Type::Rational))
		{
			*this = Node(text(), "Rational(" + code() + ")", aToType);
		}
		else if (type()->is(Type::Imaginary) && aToType->is(Type::Complex))
		{
			*this = Node(text(), aToType->codeType() + "(0, " + code() + ")", aToType);
		}
		else if (type()->is(Type::Number) && aToType->is(Type::Imaginary))
		{
			setFlag(Type::Imaginary);
		}
		else if (type()->is(Type::SingleNr) && aToType->is(Type::Complex))
		{
			*this = Node(text(), aToType->codeType() + "(" + code() + ", 0)", aToType);
		}
		else if (!aToType->is(Type::Abstract))
		{
			if (type()->is(Type::Real) && !aToType->is(Type::Real))
			{
				*this = Node(text(), "static_cast<" + aToType->codeType() + ">(" + code() + ")", aToType);
			}
			else if (!type()->is(Type::Real) && aToType->is(Type::Real))
			{
				*this = Node(text(), "static_cast<" + aToType->codeType() + ">(" + code() + ")", aToType);
			}
			else if (type()->isBiggerThan(aToType))
			{
				*this = Node(text(), "static_cast<" + aToType->codeType() + ">(" + code() + ")", aToType);
			}
		}
	}
	else if (type()->is(Type::Number) && aToType->is(Type::Text))
	{
		*this = Node(text(), "std::to_string(" + code() + ")", aToType);
  }
	else if (type()->is(Type::Boolean) && aToType->is(Type::Text))
	{
		*this = Node(text(), "(" + code() + "?\"true\":\"false\")", aToType);
  }
	else if (type()->is(Type::Char) && aToType->is(Type::Text))
	{
		*this = Node(text(), "Core::toString(" + code() + ")", aToType);
  }
	else if (type()->is(Type::Char) && aToType->is(Type::Integer))
	{
		*this = Node(text(), "static_cast<" + aToType->codeType() + ">(" + code() + ")", aToType);
  }
	else if (type()->is(Type::Integer) && aToType->is(Type::Char))
	{
		*this = Node(text(), "static_cast<" + aToType->codeType() + ">(" + code() + ")", aToType);
  }
	else if (!type()->isOfType(aToType->name()))
	{
		ok = false;
	}

	return ok;
}

Expr::Expr()
{
}

Expr::Expr(const Node& aNode)
{
	addNode(aNode);
}

Expr::Expr(const std::string& aWord)
	: Expr(Node(aWord))
{
}

Expr::Expr(const IdentifierPtr& aId)
	: Expr(Node(aId))
{
}

Expr::Expr(const std::string& aText, const TypePtr& aType)
	: Expr(Node(aText, aText, aType))
{
}

Expr::Expr(const std::string& aText, const std::string& aCode, const TypePtr& aType)
	: Expr(Node(aText, aCode, aType))
{
}

Expr::Expr(const Expr& aExpr1, const Expr& aExpr2)
{
	addNodes(aExpr1.nodes().begin(), aExpr1.nodes().end());
	addNodes(aExpr2.nodes().begin(), aExpr2.nodes().end());
}

Expr Expr::parenthesized(const Expr& aExpr)
{
	Expr result;
	result.addNode(Node("("));
	result.addNodes(aExpr.nodes().begin(), aExpr.nodes().end());
	result.addNode(Node(")"));
	return result;
}

void Expr::addNode(const Node& aNode)
{
	mNodes.push_back(aNode);
}

void Expr::insertNode(const Node& aNode)
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

	return result;
}

bool Expr::is(size_t aFlag) const { return mNodes.empty() ? false : mNodes.front().is(aFlag); }
void Expr::setFlag(size_t aFlag)  { if (!mNodes.empty()) { mNodes.front().setFlag(aFlag); } }
void Expr::setFlag(size_t aFlag, bool aEnable) { if (!mNodes.empty()) { mNodes.front().setFlag(aFlag, aEnable); } }

std::string        Expr::code()			const { return mNodes.empty() ? "" : mNodes.front().code(); }
std::string&       Expr::code()			      { return mNodes.front().code(); }
TypePtr            Expr::type()			const { return mNodes.empty() ? TypePtr() : mNodes.front().type(); }
bool               Expr::isEmpty()	const { return mNodes.empty(); }
IdentifierPtr      Expr::id()	  		const { return mNodes.empty() ? IdentifierPtr() : mNodes.front().id(); }

bool Expr::castToType(const TypePtr& aToType)
{
	return mNodes.empty() ? false : mNodes.front().castToType(aToType);
}

const std::vector<Expr::Node>& Expr::nodes() const { return mNodes; }

std::ostream& operator<<(std::ostream& aStream, const Expr::Node& aValue)
{
	aStream << "Node(";
	if (aValue.is(Expr::Word)) aStream << ",Word";
	if (aValue.is(Expr::Literal)) aStream << ",Literal";
	if (aValue.is(Expr::Output)) aStream << ",Output";
	if (aValue.is(Expr::ConstExpr)) aStream << ",ConstExpr";
	if (aValue.is(Expr::Default)) aStream << ",Default";
	if (aValue.is(Expr::Property)) aStream << ",Property";
	if (aValue.is(Expr::Identifier)) aStream << ",Identifier";
	if (aValue.is(Expr::ObjectImpl)) aStream << ",ObjectImpl";
		
	aStream << aValue.text() << "," 
		      << aValue.code() << "," 
		      << (aValue.type() ? *aValue.type() : Type());
	
	aStream << ")";

	return aStream;
}

std::ostream& operator<<(std::ostream& aStream, const Expr& aValue)
{
	aStream << "Expr(";
	aStream << aValue.text() << ",";
	aStream << " Nodes(";
	for (const Expr::Node& node : aValue.nodes())
	{
		aStream << node;
	}
	aStream << "))";

	return aStream;
}