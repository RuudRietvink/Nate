#include "Fraction.h"

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
	static const std::string superDigits = "⁰¹²³⁴⁵⁶⁷⁸⁹";
	static const std::string subDigits = "₀₁₂₃₄₅₆₇₈₉";
	static const std::string fractions = "⅒⅑⅛⅐⅙⅕¼⅓½⅖⅔⅜⅗¾⅘⅝⅚⅞";
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

namespace std
{
	int32_t trunc(const Fraction& aFraction)
	{
		return aFraction.trunc();
	}

	int32_t round(const Fraction& aFraction)
	{
		return aFraction.round();
	}

	int32_t floor(const Fraction& aFraction)
	{
		return aFraction.floor();
	}

	int32_t ceil(const Fraction& aFraction)
	{
		return aFraction.ceil();
	}
}

Fraction::Fraction()
{
}

Fraction::Fraction(int32_t aWhole, int32_t aNumerator, int32_t aDenominator)
{
	getSign(aWhole);
	mNumerator = aNumerator;
	mDenominator = aDenominator;
	simplify();
}

Fraction::Fraction(const std::string& aString)
{
	convertFromString(aString);
}

Fraction::Fraction(const Fraction& aFraction)
: mWhole(aFraction.mWhole),
	mNumerator(aFraction.mNumerator),
	mDenominator(aFraction.mDenominator),
	mNegative(aFraction.mNegative)
{
}

Fraction::Fraction(int32_t aValue)
{
	*this = aValue;
}

Fraction::Fraction(float aValue)
{
	*this = static_cast<double>(aValue);
}

Fraction::Fraction(double aValue)
{
	*this = aValue;
}

bool Fraction::convertFromString(const std::string& aString)
{
	bool ok = true;
	utf8::iterator<std::string::const_iterator> iter(aString.cbegin(), aString.cbegin(), aString.cend());
	utf8::iterator<std::string::const_iterator> end(aString.cend(), aString.cbegin(), aString.cend());

	mWhole = 0;
	mNumerator = 0;
	mDenominator = 1;

	mNegative = utf8::peek_next(iter, end) == '-';
	if (mNegative)
	{
		++iter;
	}
	auto begin = iter;

	ok = Core::numberFrom(iter, end, mWhole);

	if (iter == begin || ok)
	{
		if (iter != end)
		{
			auto pos = Core::positionIn(fractions, *iter);
			if (pos >= 0)
			{
				mNumerator = values[pos].first;
				mDenominator = values[pos].second;

				if (++iter != end)
				{
					ok = false;
				}
			}
			else
			{
				auto prev = iter;
				ok = Core::numberFrom(iter, end, superDigits, mNumerator);
				if (prev != iter && iter != end && *iter == '/')
				{
					++iter;
					ok = Core::numberFrom(iter, end, subDigits, mDenominator);
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

void Fraction::getSign(int32_t aValue)
{
	mNegative = (aValue < 0);
	mWhole = mNegative ? -aValue : aValue;
}

int32_t Fraction::signIt(int32_t aValue) const
{
	return mNegative ? -aValue : aValue;
}

int32_t Fraction::trunc() const
{
	return signIt(mWhole);
}

int32_t Fraction::round() const
{
	return signIt(mWhole + std::round(static_cast<float>(mNumerator) / mDenominator)); 
}

int32_t Fraction::floor() const
{
	return signIt(mWhole) + std::floor(static_cast<float>(signIt(mNumerator)) / mDenominator); 
}

int32_t Fraction::ceil() const
{
	return signIt(mWhole) + std::ceil(static_cast<float>(signIt(mNumerator)) / mDenominator); 
}

Fraction& Fraction::operator=(const Fraction& aFraction)
{
	if (this != &aFraction)
	{
		mWhole = aFraction.mWhole;
		mNumerator = aFraction.mNumerator;
		mDenominator = aFraction.mDenominator;
		mNegative = aFraction.mNegative;
	}

	return *this;
}

Fraction& Fraction::operator=(int32_t aValue)
{
	getSign(aValue);
	mNumerator = 0;
	mDenominator = 1;

	return *this;
}

Fraction& Fraction::operator=(double aValue)
{
	mWhole = static_cast<int32_t>(std::trunc(aValue));
	getSign(mWhole);
	mDenominator = 10000;
	mNumerator = static_cast<int32_t>((aValue - mWhole) * mDenominator);
	simplify();

	return *this;
}

bool Fraction::operator==(const Fraction& aFraction) const
{
	return mNegative == aFraction.mNegative &&
		     mWhole == aFraction.mWhole && 
				 mNumerator * aFraction.mDenominator == aFraction.mNumerator * mDenominator;
}

bool Fraction::operator!=(const Fraction& aFraction) const
{
	return !operator==(aFraction);
}

bool Fraction::operator<(const Fraction& aFraction) const
{
	return (mNegative && !aFraction.mNegative) ||
		     (mNegative == aFraction.mNegative &&
					((!mNegative && 
					 	(mWhole < aFraction.mWhole || 
						 (mWhole == aFraction.mWhole && 
							mNumerator * aFraction.mDenominator < aFraction.mNumerator * mDenominator))) ||
					 (mNegative && 
						(mWhole > aFraction.mWhole || 
						 (mWhole == aFraction.mWhole && 
						  mNumerator * aFraction.mDenominator > aFraction.mNumerator * mDenominator)))));
}

bool Fraction::operator>(const Fraction& aFraction) const
{
	return (!mNegative && aFraction.mNegative) ||
		     (mNegative == aFraction.mNegative &&
					((!mNegative && 
					 	(mWhole > aFraction.mWhole || 
						 (mWhole == aFraction.mWhole && 
							mNumerator * aFraction.mDenominator > aFraction.mNumerator * mDenominator))) ||
					 (mNegative && 
						(mWhole < aFraction.mWhole || 
						 (mWhole == aFraction.mWhole && 
						  mNumerator * aFraction.mDenominator < aFraction.mNumerator * mDenominator)))));
}

bool Fraction::operator<=(const Fraction& aFraction) const
{
	return !operator>(aFraction);
}

bool Fraction::operator>=(const Fraction& aFraction) const
{
	return !operator<(aFraction);
}

Fraction Fraction::operator-() const
{
	Fraction result = *this;
	result.mNegative = !result.mNegative;
	return result;
}

Fraction Fraction::operator+(const Fraction& aFraction) const
{
	Fraction result = *this;

	if (result.mNegative == aFraction.mNegative)
	{
		result.mWhole += aFraction.mWhole;

		if (result.mDenominator == aFraction.mDenominator)
		{
			result.mNumerator += aFraction.mNumerator;
		}
		else
		{
			Fraction temp(aFraction);
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
		return *this - -aFraction;
	}

	return result;
}

Fraction Fraction::operator-(const Fraction& aFraction) const
{
	Fraction result = *this;

	if (result.mNegative == aFraction.mNegative)
	{
		if (result.mNegative && result > aFraction)
		{
			return -aFraction - -result;
		}
		else if (!result.mNegative && result < aFraction)
		{
			return -(aFraction - result);
		}
		else
		{
			result.mWhole -= aFraction.mWhole;
		
			if (aFraction.mDenominator == 0)
			{
				result.mNumerator ;
			}

			if (result.mDenominator == aFraction.mDenominator)
			{
				result.mNumerator -= aFraction.mNumerator;
			}
			else
			{
				Fraction temp(aFraction);
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
		return *this + -aFraction;
	}

	return result;
}

Fraction Fraction::operator*(const Fraction& aFraction) const
{
	Fraction result = *this;
	Fraction temp = aFraction;
	
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

Fraction Fraction::operator/(const Fraction& aFraction) const
{
	Fraction result = *this;
	Fraction temp = aFraction;
	
	preventOverflow(result.mWhole, result.mDenominator, result.mNumerator, result, 0);
	result.mNumerator += result.mWhole * result.mDenominator;
	result.mWhole = 0;
	preventOverflow(temp.mWhole, temp.mDenominator, temp.mNumerator, temp, 0);
	temp.mNumerator += temp.mWhole * temp.mDenominator;
	temp.mWhole = 0;
	std::swap(temp.mNumerator, temp.mDenominator);
	return operator*(temp);
}

Fraction Fraction::operator%(const Fraction& aFraction) const
{
	int32_t divisor = (*this / aFraction).trunc();
	Fraction result = *this - (divisor * aFraction);

	result.simplify();

	return result;
}

Fraction operator+(int32_t aValue, const Fraction& aFraction)
{
	return Fraction(aValue) + aFraction;
}

Fraction operator-(int32_t aValue, const Fraction& aFraction)
{
	return Fraction(aValue) - aFraction;
}

Fraction operator*(int32_t aValue, const Fraction& aFraction)
{
	return Fraction(aValue) * aFraction;
}

Fraction operator/(int32_t aValue, const Fraction& aFraction)
{
	return Fraction(aValue) / aFraction;
}

Fraction operator%(int32_t aValue, const Fraction& aFraction)
{
	return Fraction(aValue) % aFraction;
}

Fraction operator+(double aValue, const Fraction& aFraction)
{
	return Fraction(aValue) + aFraction;
}

Fraction operator-(double aValue, const Fraction& aFraction)
{
	return Fraction(aValue) - aFraction;
}

Fraction operator*(double aValue, const Fraction& aFraction)
{
	return Fraction(aValue) * aFraction;
}

Fraction operator/(double aValue, const Fraction& aFraction)
{
	return Fraction(aValue) / aFraction;
}

Fraction operator%(double aValue, const Fraction& aFraction)
{
	return Fraction(aValue) % aFraction;
}

void Fraction::simplify()
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

void Fraction::preventOverflow(int32_t& aMul1, int32_t& aMul2, int64_t aSum,
														 	 Fraction& aFrac,
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

double Fraction::toDouble() const
{
	float result = mWhole + (static_cast<float>(mNumerator) / mDenominator);
	return mNegative ? -result : result;
}

std::string Fraction::toString() const
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
			auto fracIter = fractions.cbegin();
			auto dist = std::distance(values.cbegin(), valIter);
			utf8::advance(fracIter, dist, fractions.cend());
			auto point = utf8::next(fracIter, fractions.cend());
			std::string temp;
			utf8::append(point, std::back_inserter<std::string>(temp));
			ss << temp;
		}
		else
		{
			ss << Core::toString(mNumerator, superDigits) << "/" << Core::toString(mDenominator, subDigits);
		}
	}

	return ss.str();
}

std::ostream& operator<<(std::ostream& aStream, const Fraction& aFraction)
{
	aStream << aFraction.toString();
	return aStream;
}
