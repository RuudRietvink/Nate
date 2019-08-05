#pragma once
#include <stdint.h>
#include <string>
#include <iostream>

class Fraction
{
public:
	Fraction();
	explicit Fraction(const std::string& aString);
	Fraction(int32_t aValue);
	Fraction(float aValue);
	Fraction(double aValue);
	Fraction(const Fraction& aFraction);
	virtual ~Fraction() = default;

	double toDouble() const;
	bool convertFromString(const std::string& aString);
	std::string toString() const;
	
	Fraction& operator=(const Fraction& aFraction);
	Fraction& operator=(int32_t aValue);
	Fraction& operator=(double aValue);

	bool operator==(const Fraction& aFraction) const;
	bool operator!=(const Fraction& aFraction) const;
	bool operator<(const Fraction& aFraction) const;
	bool operator>(const Fraction& aFraction) const;
	bool operator<=(const Fraction& aFraction) const;
	bool operator>=(const Fraction& aFraction) const;

	Fraction operator-() const;
	Fraction operator+(const Fraction& aFraction) const;
	Fraction operator-(const Fraction& aFraction) const;

	friend Fraction operator+(int32_t aValue, const Fraction& aFraction);
	friend Fraction operator-(int32_t aValue, const Fraction& aFraction);
	friend Fraction operator+(double aValue, const Fraction& aFraction);
	friend Fraction operator-(double aValue, const Fraction& aFraction);

private:
	void simplify();
	static void preventOverflow(Fraction& aFrac1, Fraction& aFrac2);

	int32_t mWhole       = 0;
	int32_t mNumerator   = 0;
	int32_t mDenominator = 1;
	bool		mNegative    = false;
};

std::ostream& operator<<(std::ostream& aStream, const Fraction& aFraction);
