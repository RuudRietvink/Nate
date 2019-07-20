#pragma once

#include "Identifier.h"
#include "WithFlags.h"

#include <string>
#include <ostream>

class CodeArg : public WithFlags
{
public:
	CodeArg(const Identifier& aIdentifier);
	CodeArg(const std::string& aWord);
	virtual ~CodeArg() = default;

	bool               isIdentifier() const;
	const Identifier&  identifier() const;
	const std::string& word() const;
	void               setArgFlag(const std::string& aFlag);

	static const size_t Num    = 1;
	static const size_t Any    = 2;
	static const size_t Cmp    = 3;
	static const size_t Same   = 4;

private:
	Identifier      mIdentifier;
	std::string     mWord;
	bool            mIsIdentifier = false;
};
std::ostream& operator<<(std::ostream& aStream, const CodeArg& aValue);
