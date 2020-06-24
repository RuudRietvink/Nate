#pragma once
#include <stdint.h>
#include <string>
#include <iostream>

class Rational
{
public:
	Rational();
	explicit Rational(const std::string& aString);
	explicit Rational(float aValue);
	Rational(double aValue);
	Rational(const Rational& aRational);

	constexpr Rational(int32_t aWhole, int32_t aNumerator, int32_t aDenominator)
		: mNegative(aWhole < 0 || aNumerator < 0 || aDenominator < 0),
			mWhole(aWhole < 0 ? -aWhole : aWhole),
			mNumerator(aNumerator < 0 ? -aNumerator : aNumerator),
			mDenominator(aDenominator < 0 ? -aDenominator : aDenominator)
	{
	}

	constexpr Rational(int32_t aValue)
		: Rational(aValue, 0, 1)
	{}

	constexpr explicit Rational(int64_t aValue)
		: Rational(static_cast<int32_t>(aValue), 0, 1)
	{}

	virtual ~Rational() = default;

	int32_t trunc() const;
	int32_t round() const;
	int32_t floor() const;
	int32_t ceil() const;
	Rational abs() const;
	Rational cos() const;
	Rational sin() const;
	Rational tan() const;
	Rational acos() const;
	Rational asin() const;
	Rational atan() const;
	Rational cosh() const;
	Rational sinh() const;
	Rational tanh() const;
	Rational log() const;
	Rational log2() const;
	Rational log10() const;
	Rational pow(double aPower) const;
	Rational pow(const Rational& aPower) const;
	Rational sqrt() const;
	
	bool convertFromString(const std::string& aString);
	std::string toString() const;
	
	Rational& operator=(const Rational& aRational);
	Rational& operator=(int32_t aValue);
	Rational& operator=(double aValue);

	bool operator==(const Rational& aRational) const;
	bool operator!=(const Rational& aRational) const;
	bool operator<(const Rational& aRational) const;
	bool operator>(const Rational& aRational) const;
	bool operator<=(const Rational& aRational) const;
	bool operator>=(const Rational& aRational) const;

	Rational operator-() const;
	Rational operator+(const Rational& aRational) const;
	Rational operator-(const Rational& aRational) const;
	Rational operator*(const Rational& aRational) const;
	Rational operator/(const Rational& aRational) const;
	Rational operator%(const Rational& aRational) const;
	Rational operator++();
	Rational operator--();
	Rational operator++(int);
	Rational operator--(int);

	friend Rational operator+(int32_t aValue, const Rational& aRational);
	friend Rational operator-(int32_t aValue, const Rational& aRational);
	friend Rational operator*(int32_t aValue, const Rational& aRational);
	friend Rational operator/(int32_t aValue, const Rational& aRational);
	friend Rational operator%(int32_t aValue, const Rational& aRational);
	friend Rational operator+(double aValue, const Rational& aRational);
	friend Rational operator-(double aValue, const Rational& aRational);
	friend Rational operator*(double aValue, const Rational& aRational);
	friend Rational operator/(double aValue, const Rational& aRational);
	friend Rational operator%(double aValue, const Rational& aRational);
	
	constexpr double toDouble() const
	{ 
		return signIt(static_cast<double>(mWhole) + (static_cast<float>(mNumerator) / mDenominator));
	}

	constexpr int32_t toInt() const
	{
		return signIt(mWhole);
	}
	
	constexpr explicit operator double () const
	{
		return toDouble();
	}
	
	constexpr explicit operator int32_t () const
	{
		return toInt();
	}

	constexpr int32_t whole() { return signIt(mWhole); }
	constexpr int32_t numerator() { return mNumerator; }
	constexpr int32_t denominator() { return mDenominator; }

private:
	void simplify();
	static void preventOverflow(int32_t& aMul1, int32_t& aMul2, int64_t aSum,
														 	Rational& aFrac,
														 	int32_t aNum2);
	constexpr int32_t signIt(int32_t aValue) const { return mNegative ? -aValue : aValue; }
	constexpr double signIt(double aValue) const { return mNegative ? -aValue : aValue; }
	

	int32_t mWhole       = 0;
	int32_t mNumerator   = 0;
	int32_t mDenominator = 1;
	bool		mNegative    = false;
};

std::ostream& operator<<(std::ostream& aStream, const Rational& aRational);

namespace std {
	int32_t trunc(const Rational& aRational);
	int32_t round(const Rational& aRational);
	int32_t floor(const Rational& aRational);
	int32_t ceil(const Rational& aRational);
	Rational abs(const Rational& aRational);
	Rational pow(const Rational& aRational, double aPower);
	Rational pow(double aValue, const Rational& aPower);
	Rational pow(const Rational& aRational, const Rational& aPower);
	Rational sqrt(const Rational& aRational);
	Rational cos(const Rational& aRational);
	Rational sin(const Rational& aRational);
	Rational tan(const Rational& aRational);
	Rational acos(const Rational& aRational);
	Rational asin(const Rational& aRational);
	Rational atan(const Rational& aRational);
	Rational cosh(const Rational& aRational);
	Rational sinh(const Rational& aRational);
	Rational tanh(const Rational& aRational);
	Rational log(const Rational& aRational);
	Rational log2(const Rational& aRational);
	Rational log10(const Rational& aRational);
}
