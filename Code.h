#pragma once

#include "Arg.h"
#include "Method.h"

#include <string>
#include <ostream>
#include <tuple>

class Identifier;

class Code : public Method
{
public:
	Code();
	virtual ~Code() = default;

	void addCodeStatWord(const std::string& aWord);
	void addCodeStatId(const std::string& aId);
	std::string toCodeWord(const std::string& aWord) const override;
	  
private:
};
std::ostream& operator<<(std::ostream& aStream, const Code& aValue);

