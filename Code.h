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

	std::tuple<std::string, std::string, Type> evaluate(ExprNodesCIter& aBegin, ExprNodesCIter& aEnd) const override;
	void addCodeStatWord(const std::string& aWord);
	void addCodeStatId(const std::string& aId);
	std::string toCodeWord(const std::string& aWord) const override;

	const std::string&          codeStat() const;
	std::string&                codeStat();
  
private:
	std::string          mCodeStat;
};
std::ostream& operator<<(std::ostream& aStream, const Code& aValue);

