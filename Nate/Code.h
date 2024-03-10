#pragma once

#include "Method.h"

#include <string>
#include <ostream>

namespace nate
{
class Code;
typedef std::shared_ptr<Code> CodePtr;

class Code : public Method
{
public:
	Code();
	virtual ~Code() = default;

	void addCodeStatWord(const std::string& aWord);
	void addCodeStatId(const std::string& aId);
	std::string toCodeWord(const std::string& aWord) const override;
	bool equals(const Code& aCode) const;
	  
private:
	bool isCodeMethod() const override { return true; }
};
std::ostream& operator<<(std::ostream& aStream, const Code& aValue);

}
