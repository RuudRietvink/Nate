#pragma once

#include "Identifier.h"
#include "WithFlags.h"

#include <string>
#include <ostream>

class Arg : public WithFlags
{
public:
	Arg(const IdentifierPtr& aIdentifier);
	Arg(const std::string& aWord);
	virtual ~Arg() = default;

	bool									isIdentifier() const;
	const IdentifierPtr&	identifier() const;
	const std::string&		word() const;
	bool									setArgFlag(const std::string& aFlag);
	
	static const size_t CompHigh	= 2;
	static const size_t Cmp				= 3;
	static const size_t Same			= 4;
	static const size_t Out				= 5;
	static const size_t InOut			= 6;
	static const size_t Owner			= 7;
	static const size_t Prop			= 8;
	static const size_t Template	= 9;
	static const size_t Typename	= 10;

private:
	IdentifierPtr   mIdentifier;
	std::string     mWord;
	bool            mIsIdentifier = false;
};
std::ostream& operator<<(std::ostream& aStream, const Arg& aValue);
