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

typedef std::vector<Expr> ExprNodes;
typedef std::vector<Expr>::const_iterator ExprNodesCIter;

class Expr : public WithFlags
{
public:
	Expr();
	Expr(const IdentifierPtr& aId);
	Expr(const std::string& aWord);
	Expr(const Expr& aExpr1, const Expr& aExpr2);
	Expr(const std::string& aText, const TypePtr& aType);
	Expr(const std::string& aText, const std::string& aCode, const TypePtr& aType);
	static Expr parenthesized(const Expr& aExpr1);
	
	Expr(const Expr& aOther) = default;
	virtual ~Expr() = default;
	void addNode(const Expr& aNode);
	void insertNode(const Expr& aNode);
	void addNodes(const ExprNodesCIter& aNodeBegin, const ExprNodesCIter& aNodeEnd);
	bool castToType(const TypePtr& aToType);

  std::string        text()       const;
	const std::string& code()       const;
	std::string&       code();
	TypePtr            type()       const;
	bool               isEmpty()    const;
	IdentifierPtr      id()         const;

	const std::vector<Expr>& nodes() const;
	
	static const size_t Word      = 0;
	static const size_t Output    = 1;
	static const size_t Literal   = 2;
	static const size_t ConstExpr = 3;
	static const size_t Default   = 4;
	static const size_t Property  = 5;
	static const size_t Identifier= 6;
	static const size_t ObjectImpl= 7;

private:
	std::vector<Expr> mNodes;
	std::string       mText;
	std::string       mCode;
	TypePtr           mType;
	IdentifierPtr     mId;
};


std::ostream& operator<<(std::ostream& aStream, const Expr& aValue);
