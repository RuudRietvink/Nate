#pragma once

#include "Type.h"
#include "WithFlags.h"

#include <string>
#include <vector>
#include <ostream>
#include <memory>

class Expr;
typedef std::shared_ptr<Expr> ExprPtr;
class Identifier;
typedef std::shared_ptr<Identifier> IdentifierPtr;


class Expr
{
public:
	class Node : public WithFlags
	{
	public:
		Node();
		Node(const IdentifierPtr& aId);
		Node(const std::string& aWord);
		Node(const std::string& aText, const TypePtr& aType);
		Node(const std::string& aText, const std::string& aCode, const TypePtr& aType);
		Node(const Node& aOther) = default;
		virtual ~Node() = default;

		bool castToType(const TypePtr& aToType);

		const std::string& text()       const;
		const std::string& code()       const;
		std::string&       code();
		TypePtr            type()       const;
		IdentifierPtr      id()         const;


	private:
		std::string       mText;
		std::string       mCode;
		TypePtr           mType;
		IdentifierPtr     mId;
	};

	Expr();
	Expr(const Node& aNode);
	Expr(const IdentifierPtr& aId);
	Expr(const std::string& aWord);
	Expr(const Expr& aExpr1, const Expr& aExpr2);
	Expr(const std::string& aText, const TypePtr& aType);
	Expr(const std::string& aText, const std::string& aCode, const TypePtr& aType);
	static Expr parenthesized(const Expr& aExpr1);
		
	bool castToType(const TypePtr& aToType);
	bool is(size_t aFlag) const;
	void setFlag(size_t aFlag);
	void setFlag(size_t aFlag, bool aEnable);
	
	Expr(const Expr& aOther) = default;
	virtual ~Expr() = default;
	void addNode(const Node& aNode);
	void insertNode(const Node& aNode);
	void addNodes(const std::vector<Node>::const_iterator& aNodeBegin, const std::vector<Node>::const_iterator& aNodeEnd);

  std::string        text()       const;
	std::string        code()       const;
	std::string&       code();
	TypePtr            type()       const;
	bool               isEmpty()    const;
	IdentifierPtr      id()         const;

	const std::vector<Node>& nodes() const;
	
	static const size_t Word      = 0;
	static const size_t Output    = 1;
	static const size_t Literal   = 2;
	static const size_t ConstExpr = 3;
	static const size_t Default   = 4;
	static const size_t Property  = 5;
	static const size_t Identifier= 6;
	static const size_t ObjectImpl= 7;
	
private:
	std::vector<Node> mNodes;
};

typedef std::vector<Expr::Node>::const_iterator ExprNodesCIter;

std::ostream& operator<<(std::ostream& aStream, const Expr& aValue);
std::ostream& operator<<(std::ostream& aStream, const Expr::Node& aValue);
