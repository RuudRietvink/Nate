#pragma once
#include "Method.h"

#include <string>
#include <ostream>

class Value : public Method
{
public:
	Value();
	virtual ~Value() = default;

	const std::string& name() const;
	void addValue(const std::string& aWord);
	std::string toCodeWord(const std::string& aWord) const override;
	  
private:
};
std::ostream& operator<<(std::ostream& aStream, const Value& aValue);

