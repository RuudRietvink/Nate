#pragma once
#include <stdint.h>
#include <string>

class Fraction
{
public:
	Fraction();
	virtual ~Fraction() = default;

	float toFloat() const;
	bool convertFromString(const std::string& aString);

//private:
	int32_t mWhole       = 0;
	int32_t mNumerator   = 0;
	int32_t mDenominator = 1;

};

