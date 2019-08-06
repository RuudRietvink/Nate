#pragma once
#include <stdint.h>
#include <string>
#include <iostream>

class Fraction
{
public:
	Fraction();
	explicit Fraction(const std::string& aString);
	Fraction(int32_t aWhole, int32_t aNumerator, int32_t aDenominator);
	Fraction(int32_t aValue);
	Fraction(float aValue);
	Fraction(double aValue);
	Fraction(const Fraction& aFraction);
	virtual ~Fraction() = default;

	int32_t trunc() const;
	int32_t round() const;
	int32_t floor() const;
	int32_t ceil() const;
	Fraction abs() const;
	
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
	Fraction operator*(const Fraction& aFraction) const;
	Fraction operator/(const Fraction& aFraction) const;
	Fraction operator%(const Fraction& aFraction) const;

	friend Fraction operator+(int32_t aValue, const Fraction& aFraction);
	friend Fraction operator-(int32_t aValue, const Fraction& aFraction);
	friend Fraction operator*(int32_t aValue, const Fraction& aFraction);
	friend Fraction operator/(int32_t aValue, const Fraction& aFraction);
	friend Fraction operator%(int32_t aValue, const Fraction& aFraction);
	friend Fraction operator+(double aValue, const Fraction& aFraction);
	friend Fraction operator-(double aValue, const Fraction& aFraction);
	friend Fraction operator*(double aValue, const Fraction& aFraction);
	friend Fraction operator/(double aValue, const Fraction& aFraction);
	friend Fraction operator%(double aValue, const Fraction& aFraction);

private:
	void simplify();
	static void preventOverflow(int32_t& aMul1, int32_t& aMul2, int64_t aSum,
														 	Fraction& aFrac,
														 	int32_t aNum2);
	int32_t signIt(int32_t aValue) const;
	void getSign(int32_t aValue);

	int32_t mWhole       = 0;
	int32_t mNumerator   = 0;
	int32_t mDenominator = 1;
	bool		mNegative    = false;
};

std::ostream& operator<<(std::ostream& aStream, const Fraction& aFraction);

namespace std {
	int32_t trunc(const Fraction& aFraction);
	int32_t round(const Fraction& aFraction);
	int32_t floor(const Fraction& aFraction);
	int32_t ceil(const Fraction& aFraction);
	Fraction abs(const Fraction& aFraction);
}
