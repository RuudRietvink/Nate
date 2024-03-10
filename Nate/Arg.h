#pragma once

#include "Identifier.h"
#include "WithFlags.h"

#include <string>
#include <ostream>

namespace nate
{
class Arg : public WithFlags
{
public:
	Arg(const IdentifierPtr& aIdentifier);
	Arg(const std::string& aWord);
	virtual ~Arg() = default;

	bool									isIdentifier() const;
	const IdentifierPtr&	identifier() const;
	const std::string&		word() const;
	std::string						setFlagString(const std::string& aFlag) override;
	
	static const size_t Exact   	= 1;
	static const size_t CompHigh	= 2;
	static const size_t Cmp				= 3;
	static const size_t Same			= 4;
	static const size_t Out				= 5;
	static const size_t InOut			= 6;
	static const size_t Owner			= 7;
	static const size_t Member  	= 8;
	static const size_t Template	= 9;
	static const size_t Typename	= 10;
	static const size_t Literal 	= 11;
	static const size_t WantsUnary= 12;

private:
	IdentifierPtr   mIdentifier;
	std::string     mWord;
	bool            mIsIdentifier = false;
};
std::ostream& operator<<(std::ostream& aStream, const Arg& aValue);

}
