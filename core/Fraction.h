#pragma once
#include <stdint.h>
#include <string>
#include <iostream>

class Fraction
{
public:
	Fraction();
	Fraction(const std::string& aString);
	virtual ~Fraction() = default;

	float toFloat() const;
	bool convertFromString(const std::string& aString);
	std::string toString() const;

private:
	int32_t mWhole       = 0;
	int32_t mNumerator   = 0;
	int32_t mDenominator = 1;
	bool		mNegative    = false;
};

std::ostream& operator<<(std::ostream& aStream, const Fraction& aFraction);
