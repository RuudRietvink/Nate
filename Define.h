#pragma once

#include "Method.h"

#include <ostream>

class Identifier;

class Define : public Method
{
public:
	Define();
	virtual ~Define() = default;
  
	std::tuple<std::string, std::string, Type> evaluate(ExprNodesCIter& aBegin, ExprNodesCIter& aEnd) const override;

	void createCodeCall();
	std::string createCodeDecl();

	const std::string&          codeCall() const;
	std::string&                codeCall();

private:
	std::string          mCodeCall;
};
std::ostream& operator<<(std::ostream& aStream, const Define& aValue);

