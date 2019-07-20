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
	ExprNode(const std::string& aText, const Type& aType);
	ExprNode(const std::string& aText, const std::string& aCode, const Type& aType);
	virtual ~ExprNode() = default;

	bool castToType(const Type& aToType);

	const std::string& text()            const;
	const std::string& code()            const;
	const Type&        type()            const;
	bool               isEmpty()         const;
	
	static const size_t Word    = 0;
	static const size_t Output  = 1;
	static const size_t Literal = 2;

private:
	std::string       mText;
	std::string       mCode;
	Type              mType;
};

typedef std::vector<ExprNode> ExprNodes;
typedef std::vector<ExprNode>::const_iterator ExprNodesCIter;

std::ostream& operator<<(std::ostream& aStream, const ExprNode& aValue);