#pragma once

#include "ExprNode.h"
#include "Type.h"

#include <string>
#include <vector>
#include <ostream>

class Expr
{
public:
	Expr();
	Expr(const ExprNode& aNode);
	Expr(const Expr& aExpr1, const Expr& aExpr2);
	virtual ~Expr() = default;
	void addNode(const ExprNode& aNode);
	void insertNode(const ExprNode& aNode);
	void addNodes(const ExprNodesCIter& aNodeBegin, const ExprNodesCIter& aNodeEnd);

	std::string        text()       const;
	const std::string& code()       const;
	TypePtr            type()       const;
	bool               is(size_t aFlags) const;
	bool               isEmpty()    const;

	const std::vector<ExprNode>& nodes() const;
	const ExprNode& node() const;
	ExprNode& node();

private:
	std::vector<ExprNode> mNodes;
};

std::ostream& operator<<(std::ostream& aStream, const Expr& aValue);
