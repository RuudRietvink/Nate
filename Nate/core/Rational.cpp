#include "Rational.h"

#include "core.h"
#include <inttypes.h>
#include <limits>

#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <sstream>

namespace
{
	static const std::string rationals = "⅒⅑⅛⅐⅙⅕¼⅓½⅖⅔⅜⅗¾⅘⅝⅚⅞";
	static const std::vector<std::pair<int, int>> values = 
	{
		{ 1, 10},
		{ 1, 9},
		{ 1, 8},
		{ 1, 7},
		{ 1, 6},
		{ 1, 5},
		{ 1, 4},
		{ 1, 3},
		{ 1, 2},
		{ 2, 5},
		{ 2, 3},
		{ 3, 8},
		{ 3, 5},
		{ 3, 4},
		{ 4, 5},
		{ 5, 8},
		{ 5, 6},
		{ 7, 8},
	};

	int32_t gcd(int32_t a, int32_t b)
	{
		while (b != 0)
		{
				int32_t t = b; 
				b = a % b; 
				a = t;
		}

		return a;
	}
}

Rational::Rational()
{
}

Rational::Rational(const std::string& aString)
{
	convertFromString(aString);
}

Rational::Rational(const Rational& aRational)
: mWhole(aRational.mWhole),
	mNumerator(aRational.mNumerator),
	mDenominator(aRational.mDenominator),
	mNegative(aRational.mNegative)
{
}

Rational::Rational(float aValue)
{
	*this = static_cast<double>(aValue);
}

Rational::Rational(double aValue)
{
	*this = aValue;
}

bool Rational::convertFromString(const std::string& aString)
{
	bool ok = true;
	mWhole = 0;
	mNumerator = 0;
	mDenominator = 1;
	std::string::const_iterator start = aString.begin();

	mNegative = start != aString.end() && *start == '-';
	if (mNegative)
	{
		++start;
	}

	std::string zeroPreprended = "0" + std::string(start, aString.end());
	Core::Utf8 string(zeroPreprended);

	ok = Core::numberFrom(string, mWhole);

	if (string.iter() == string.end() || ok || string.iter() == string.begin())
	{
		if (string)
		{
			auto pos = Core::positionIn(rationals, *string);
			if (pos >= 0)
			{
				mNumerator = values[pos].first;
				mDenominator = values[pos].second;

				if (++string)
				{
					ok = false;
				}
			}
			else
			{
				auto prev = string.iter();
				ok = Core::numberFrom(string, Core::superDigits, mNumerator);
				if (prev != string.iter() && (*string == '/' || *string == 0x2044 /*'⁄'*/))
				{
					++string;
					ok = Core::numberFrom(string, Core::subDigits, mDenominator);
					if (mDenominator == 0)
					{
						ok = false;
					}
				}
				else
				{
					ok = false;
				}
			}
		}
	}

	if (ok)
	{
		simplify();
	}

	return ok;
}

int32_t Rational::trunc() const
{
	return signIt(mWhole);
}

int32_t Rational::round() const
{
	return signIt(static_cast<int32_t>(mWhole + std::round(static_cast<float>(mNumerator) / mDenominator))); 
}

int32_t Rational::floor() const
{
	return signIt(mWhole) + static_cast<int32_t>(std::floor(static_cast<float>(signIt(mNumerator)) / mDenominator)); 
}

int32_t Rational::ceil() const
{
	return signIt(mWhole) + static_cast<int32_t>(std::ceil(static_cast<float>(signIt(mNumerator)) / mDenominator)); 
}

Rational Rational::abs() const
{
	Rational result = *this;
	result.mNegative = false;
	return result;
}

Rational Rational::cos() const { return Rational(std::cos(toDouble())); }
Rational Rational::sin() const { return Rational(std::sin(toDouble())); }
Rational Rational::tan() const { return Rational(std::tan(toDouble())); }
Rational Rational::acos() const { return Rational(std::acos(toDouble())); }
Rational Rational::asin() const { return Rational(std::asin(toDouble())); }
Rational Rational::atan() const { return Rational(std::atan(toDouble())); }
Rational Rational::cosh() const { return Rational(std::cosh(toDouble())); }
Rational Rational::sinh() const { return Rational(std::sinh(toDouble())); }
Rational Rational::tanh() const { return Rational(std::tanh(toDouble())); }
Rational Rational::log() const { return Rational(std::log(toDouble())); }
Rational Rational::log2() const { return Rational(std::log2(toDouble())); }
Rational Rational::log10() const { return Rational(std::log10(toDouble())); }
Rational Rational::sqrt() const { return Rational(std::sqrt(toDouble())); }

Rational Rational::pow(double aPower) const
{
	return Rational(std::pow(toDouble(), aPower));
}

Rational Rational::pow(const Rational& aPower) const
{
	return Rational(std::pow(toDouble(), aPower.toDouble()));
}

Rational& Rational::operator=(const Rational& aRational)
{
	if (this != &aRational)
	{
		mWhole = aRational.mWhole;
		mNumerator = aRational.mNumerator;
		mDenominator = aRational.mDenominator;
		mNegative = aRational.mNegative;
	}

	return *this;
}

Rational& Rational::operator=(int32_t aValue)
{
	mNegative = (aValue < 0);
	mWhole = mNegative ? -aValue : aValue;
	mNumerator = 0;
	mDenominator = 1;

	return *this;
}

Rational& Rational::operator=(double aValue)
{
	mWhole = static_cast<int32_t>(std::trunc(aValue));
	mNegative = (aValue < 0);
	if (mNegative)
	{
		mWhole = -mWhole;
		aValue = -aValue;
	}

	mDenominator = 10000;
	mNumerator = static_cast<int32_t>((aValue - mWhole) * mDenominator);
	simplify();

	return *this;
}

bool Rational::operator==(const Rational& aRational) const
{
	return mNegative == aRational.mNegative &&
		     mWhole == aRational.mWhole && 
				 mNumerator * aRational.mDenominator == aRational.mNumerator * mDenominator;
}

bool Rational::operator!=(const Rational& aRational) const
{
	return !operator==(aRational);
}

bool Rational::operator<(const Rational& aRational) const
{
	return (mNegative && !aRational.mNegative) ||
		     (mNegative == aRational.mNegative &&
					((!mNegative && 
					 	(mWhole < aRational.mWhole || 
						 (mWhole == aRational.mWhole && 
							mNumerator * aRational.mDenominator < aRational.mNumerator * mDenominator))) ||
					 (mNegative && 
						(mWhole > aRational.mWhole || 
						 (mWhole == aRational.mWhole && 
						  mNumerator * aRational.mDenominator > aRational.mNumerator * mDenominator)))));
}

bool Rational::operator>(const Rational& aRational) const
{
	return (!mNegative && aRational.mNegative) ||
		     (mNegative == aRational.mNegative &&
					((!mNegative && 
					 	(mWhole > aRational.mWhole || 
						 (mWhole == aRational.mWhole && 
							mNumerator * aRational.mDenominator > aRational.mNumerator * mDenominator))) ||
					 (mNegative && 
						(mWhole < aRational.mWhole || 
						 (mWhole == aRational.mWhole && 
						  mNumerator * aRational.mDenominator < aRational.mNumerator * mDenominator)))));
}

bool Rational::operator<=(const Rational& aRational) const
{
	return !operator>(aRational);
}

bool Rational::operator>=(const Rational& aRational) const
{
	return !operator<(aRational);
}

Rational Rational::operator-() const
{
	Rational result = *this;
	result.mNegative = !result.mNegative;
	return result;
}

Rational Rational::operator+(const Rational& aRational) const
{
	Rational result = *this;

	if (result.mNegative == aRational.mNegative)
	{
		result.mWhole += aRational.mWhole;

		if (result.mDenominator == aRational.mDenominator)
		{
			result.mNumerator += aRational.mNumerator;
		}
		else
		{
			Rational temp(aRational);
			preventOverflow(result.mDenominator, temp.mDenominator, 0, result, temp.mNumerator);
			preventOverflow(result.mDenominator, temp.mDenominator, 0, temp, result.mNumerator);
			result.mNumerator = result.mNumerator * temp.mDenominator +
													temp.mNumerator * result.mDenominator;
			result.mDenominator *= temp.mDenominator;
		}

		result.simplify();
	}
	else
	{
		return *this - -aRational;
	}

	return result;
}

Rational Rational::operator-(const Rational& aRational) const
{
	Rational result = *this;

	if (result.mNegative == aRational.mNegative)
	{
		if (result.mNegative && result > aRational)
		{
			return -aRational - -result;
		}
		else if (!result.mNegative && result < aRational)
		{
			return -(aRational - result);
		}
		else
		{
			result.mWhole -= aRational.mWhole;
		
			if (aRational.mDenominator == 0)
			{
				result.mNumerator ;
			}

			if (result.mDenominator == aRational.mDenominator)
			{
				result.mNumerator -= aRational.mNumerator;
			}
			else
			{
				Rational temp(aRational);
				preventOverflow(result.mDenominator, temp.mDenominator, 0, result, temp.mNumerator);
				preventOverflow(result.mDenominator, temp.mDenominator, 0, temp, result.mNumerator);
				result.mNumerator = result.mNumerator * temp.mDenominator -
														temp.mNumerator * result.mDenominator;
				result.mDenominator *= temp.mDenominator;
			}

			result.simplify();
		}
	}
	else
	{
		return *this + -aRational;
	}

	return result;
}

Rational Rational::operator*(const Rational& aRational) const
{
	Rational result = *this;
	Rational temp = aRational;
	
	preventOverflow(result.mWhole, result.mDenominator, result.mNumerator, result, 0);
	result.mNumerator += result.mWhole * result.mDenominator;
	result.mWhole = 0;
	preventOverflow(temp.mWhole, temp.mDenominator, temp.mNumerator, temp, 0);
	temp.mNumerator += temp.mWhole * temp.mDenominator;
	temp.mWhole = 0;

	result.mNegative = (result.mNegative != temp.mNegative);
	preventOverflow(result.mNumerator, temp.mNumerator, 0, result, temp.mNumerator);
	preventOverflow(result.mNumerator, temp.mNumerator, 0, temp, result.mNumerator);
	result.mNumerator *= temp.mNumerator;
	preventOverflow(result.mDenominator, temp.mDenominator, 0, result, 0);
	result.mDenominator *= temp.mDenominator;

	result.simplify();

	return result;
}

Rational Rational::operator/(const Rational& aRational) const
{
	Rational result = *this;
	Rational temp = aRational;
	
	preventOverflow(result.mWhole, result.mDenominator, result.mNumerator, result, 0);
	result.mNumerator += result.mWhole * result.mDenominator;
	result.mWhole = 0;
	preventOverflow(temp.mWhole, temp.mDenominator, temp.mNumerator, temp, 0);
	temp.mNumerator += temp.mWhole * temp.mDenominator;
	temp.mWhole = 0;
	std::swap(temp.mNumerator, temp.mDenominator);
	return operator*(temp);
}

Rational Rational::operator%(const Rational& aRational) const
{
	int32_t divisor = (*this / aRational).trunc();
	Rational result = *this - (Rational(divisor) * aRational);

	result.simplify();

	return result;
}

Rational Rational::operator++()
{
	*this = operator+(Rational(1));
	return *this;
}

Rational Rational::operator--()
{
	*this = operator+(Rational(-1));
	return *this;
}

Rational Rational::operator++(int)
{
	Rational temp = *this;
	*this = operator+(Rational(1));
	return temp;
}

Rational Rational::operator--(int)
{
	Rational temp = *this;
	*this = operator+(Rational(-1));
	return temp;
}

Rational operator+(int32_t aValue, const Rational& aRational)
{
	return Rational(aValue) + aRational;
}

Rational operator-(int32_t aValue, const Rational& aRational)
{
	return Rational(aValue) - aRational;
}

Rational operator*(int32_t aValue, const Rational& aRational)
{
	return Rational(aValue) * aRational;
}

Rational operator/(int32_t aValue, const Rational& aRational)
{
	return Rational(aValue) / aRational;
}

Rational operator%(int32_t aValue, const Rational& aRational)
{
	return Rational(aValue) % aRational;
}

Rational operator+(double aValue, const Rational& aRational)
{
	return Rational(aValue) + aRational;
}

Rational operator-(double aValue, const Rational& aRational)
{
	return Rational(aValue) - aRational;
}

Rational operator*(double aValue, const Rational& aRational)
{
	return Rational(aValue) * aRational;
}

Rational operator/(double aValue, const Rational& aRational)
{
	return Rational(aValue) / aRational;
}

Rational operator%(double aValue, const Rational& aRational)
{
	return Rational(aValue) % aRational;
}

void Rational::simplify()
{
	//std::cerr << "simplify " << *this << "(" << mWhole << " " << mNumerator << " " << mDenominator << ") ";
	if (mNumerator < 0)
	{
		mNumerator += mDenominator;
		--mWhole;
	}
	
	if (mWhole < 0)
	{
		mNegative = !mNegative;
		mWhole = -mWhole;
	}

	if (mNumerator == mDenominator)
	{
		++mWhole;
		mNumerator = 0;
		mDenominator = 1;
	}
	else if (mNumerator > mDenominator)
	{
		int32_t whole = mNumerator / mDenominator;
		mWhole += whole;
		mNumerator -= whole * mDenominator;
	}

	if (mNumerator != 0)
	{
		auto common = gcd(mNumerator, mDenominator);
		if (common != 1)
		{
			mNumerator /= common;
			mDenominator /= common;
		}
	}

	//std::cerr << *this << "(" << mWhole << " " << mNumerator << " " << mDenominator << ") " << std::endl;
}

void Rational::preventOverflow(int32_t& aMul1, int32_t& aMul2, int64_t aSum,
														 	 Rational& aFrac,
														 	 int32_t aNum2)
{
	int64_t val;
	while ((val = static_cast<int64_t>(aMul1) * aMul2 + aSum) > std::numeric_limits<int32_t>::max() &&
				 (aFrac.mNumerator > 1000 || aNum2 < 1000))
	{
		aFrac.mNumerator /= 2;
		aFrac.mDenominator /= 2;
	}
}

std::string Rational::toString() const
{
	std::stringstream ss;
	
	if (mNegative)
	{
		ss << "-";
	}

	if (mWhole != 0 || mNumerator == 0)
	{
		ss << mWhole;
	}

	if (mNumerator > 0)
	{
		auto valIter = std::find(values.cbegin(), values.cend(), std::make_pair(mNumerator, mDenominator));
		if (valIter != values.cend())
		{
			auto fracIter = rationals.cbegin();
			auto dist = std::distance(values.cbegin(), valIter);
			utf8::advance(fracIter, dist, rationals.cend());
			auto point = utf8::next(fracIter, rationals.cend());
			std::string temp;
			utf8::append(point, std::back_inserter<std::string>(temp));
			ss << temp;
		}
		else
		{
			ss << Core::toString(mNumerator, Core::superDigits) << "⁄" 
				 << Core::toString(mDenominator, Core::subDigits);
		}
	}

	return ss.str();
}

std::ostream& operator<<(std::ostream& aStream, const Rational& aRational)
{
	aStream << aRational.toString();
	return aStream;
}

namespace std
{
	int32_t trunc(const Rational& aRational) { return aRational.trunc(); }
	int32_t round(const Rational& aRational) { return aRational.round(); }
	int32_t floor(const Rational& aRational) { return aRational.floor(); }
	int32_t ceil(const Rational& aRational) { return aRational.ceil(); }
	Rational abs(const Rational& aRational) { return aRational.abs(); }
	Rational pow(const Rational& aRational, double aPower) { return aRational.pow(aPower); }
	Rational pow(const Rational& aRational, const Rational& aPower) { return aRational.pow(aPower); }
	Rational pow(double aValue, const Rational& aPower) { return Rational(std::pow(aValue, aPower.toDouble())); }
	Rational cos(const Rational& aRational) { return aRational.cos(); }
	Rational sin(const Rational& aRational) { return aRational.sin(); }
	Rational tan(const Rational& aRational) { return aRational.tan(); }
	Rational acos(const Rational& aRational) { return aRational.acos(); }
	Rational asin(const Rational& aRational) { return aRational.asin(); }
	Rational atan(const Rational& aRational) { return aRational.atan(); }
	Rational cosh(const Rational& aRational) { return aRational.cosh(); }
	Rational sinh(const Rational& aRational) { return aRational.sinh(); }
	Rational tanh(const Rational& aRational) { return aRational.tanh(); }
	Rational log(const Rational& aRational) { return aRational.log(); }
	Rational log2(const Rational& aRational) { return aRational.log2(); }
	Rational log10(const Rational& aRational) { return aRational.log10(); }
	Rational sqrt(const Rational& aRational) { return aRational.sqrt(); }
}
