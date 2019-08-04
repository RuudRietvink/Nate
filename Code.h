#pragma once

#include "Method.h"

#include <string>
#include <ostream>

class Code : public Method
{
public:
	Code();
	virtual ~Code() = default;

	void addCodeStatWord(const std::string& aWord);
	void addCodeStatId(const std::string& aId);
	std::string toCodeWord(const std::string& aWord) const override;

	void setName(const std::string& aName);
	const std::string name() const;
	  
private:
	std::string mName;
};
std::ostream& operator<<(std::ostream& aStream, const Code& aValue);

