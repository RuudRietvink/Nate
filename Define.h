#pragma once

#include "Method.h"

#include <ostream>

class Identifier;

class Define : public Method
{
public:
	Define();
	virtual ~Define() = default;
  
	void createCodeCall();
	std::string createCodeDecl();

private:
};
std::ostream& operator<<(std::ostream& aStream, const Define& aValue);

