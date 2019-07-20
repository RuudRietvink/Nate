#pragma once

#include "WithFlags.h"

#include <string>
#include <ostream>

class Type : public WithFlags
{
public:
	Type();
	Type(const std::string& aName);
	virtual ~Type() = default;

	static Type makeType(const std::string& aValue);
	bool isBiggerThan(const Type& aType) const;
	bool isCompatibleWith(const Type& aType) const;

	const std::string& name() const;
	const std::string& codeType() const;
	int                bitSize() const;
	
	static const size_t Number     = 0;
	static const size_t Float      = 1;
	static const size_t String     = 2;
	static const size_t Boolean    = 3;
	static const size_t Unknown    = 4;
	static const size_t Comparable = 5;

private:
	std::string mName;
	std::string mCodeType;
	int			    mBitSize = 4;
};

std::ostream& operator<<(std::ostream& aStream, const Type& aValue);
