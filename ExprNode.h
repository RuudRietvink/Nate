#pragma once

#include "Type.h"
#include "WithFlags.h"

#include <string>
#include <vector>
#include <ostream>

class ExprNode : public WithFlags
{
public:
	ExprNode();
	ExprNode(const std::string& aWord);
	ExprNode(const std::string& aText, const TypePtr& aType);
	ExprNode(const std::string& aText, const std::string& aCode, const TypePtr& aType);
	virtual ~ExprNode() = default;

	bool castToType(const TypePtr& aToType);

	const std::string& text()            const;
	const std::string& code()            const;
	TypePtr            type()            const;
	bool               isEmpty()         const;
	
	static const size_t Word      = 0;
	static const size_t Output    = 1;
	static const size_t Literal   = 2;
	static const size_t ConstExpr = 3;
	static const size_t Default   = 4;

private:
	std::string       mText;
	std::string       mCode;
	TypePtr           mType;
};

typedef std::vector<ExprNode> ExprNodes;
typedef std::vector<ExprNode>::const_iterator ExprNodesCIter;

std::ostream& operator<<(std::ostream& aStream, const ExprNode& aValue);