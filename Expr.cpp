
#include "Expr.h"
#include "NateFunctions.h"

Expr::Expr()
{
}

Expr::Expr(const ExprNode& aNode)
{
	addNode(aNode);
}

Expr::Expr(const Expr& aExpr1, const Expr& aExpr2)
{
	if (!aExpr1.isEmpty() &&
		!aExpr1.nodes().front().isEmpty())
	{
		addNodes(aExpr1.nodes().begin(), aExpr1.nodes().end());
	}

	if (!aExpr2.isEmpty() &&
		!aExpr2.nodes().front().isEmpty())
	{
		addNodes(aExpr2.nodes().begin(), aExpr2.nodes().end());
	}

	if (mNodes.empty())
	{
		addNode(ExprNode());
	}
}

void Expr::addNode(const ExprNode& aNode)
{
	mNodes.push_back(aNode);
}

void Expr::insertNode(const ExprNode& aNode)
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

const std::string& Expr::code()            const { return node().code(); }
TypePtr            Expr::type()            const { return node().type(); }
bool               Expr::is(size_t aFlags) const { return !nodes().empty() && node().is(aFlags); }
bool               Expr::isEmpty()         const { return nodes().empty() || node().isEmpty(); }

const ExprNode&    Expr::node()            const { return mNodes.front(); }
ExprNode&          Expr::node()                  { return mNodes.front(); }
const std::vector<ExprNode>& Expr::nodes() const { return mNodes; }

std::ostream& operator<<(std::ostream& aStream, const Expr& aValue)
{
	aStream << "Expr(" << ",Nodes(" << join(aValue.nodes()) << ")" << ")";
	return aStream;
}