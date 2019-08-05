#include "Fraction.h"

#include "core.h"
#include <inttypes.h>
#include <limits>

#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <sstream>

namespace {
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

Fraction::Fraction()
{
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

	return ok;
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
	mNegative = (aValue < 0);
	mWhole = mNegative ? -aValue : aValue;
	mNumerator = 0;
	mDenominator = 1;

	return *this;
}

Fraction& Fraction::operator=(double aValue)
{
	mNegative = (aValue < 0);
	aValue = mNegative ? -aValue : aValue;

	mWhole = static_cast<int32_t>(std::trunc(aValue));
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
	//std::cerr << "+ " << *this << " " << aFraction << std::endl;

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
			preventOverflow(result, temp);
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
	//std::cerr << "- " << *this << " " << aFraction << std::endl;
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
				preventOverflow(result, temp);
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

Fraction operator+(int32_t aValue, const Fraction& aFraction)
{
	return Fraction(aValue) + aFraction;
}

Fraction operator-(int32_t aValue, const Fraction& aFraction)
{
	return Fraction(aValue) - aFraction;
}

Fraction operator+(double aValue, const Fraction& aFraction)
{
	return Fraction(aValue) + aFraction;
}

Fraction operator-(double aValue, const Fraction& aFraction)
{
	return Fraction(aValue) - aFraction;
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

	if (mNumerator > mDenominator)
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

void Fraction::preventOverflow(Fraction& aFrac1, Fraction& aFrac2)
{
	int64_t val;
	while ((val = static_cast<int64_t>(aFrac1.mDenominator) * aFrac2.mDenominator) > std::numeric_limits<int32_t>::max())
	{
		if (aFrac1.mNumerator > 1000 || aFrac2.mNumerator < 1000)
		{
			aFrac1.mNumerator /= 10;
			aFrac1.mDenominator /= 10;
		}
		if (aFrac2.mNumerator > 1000 || aFrac1.mNumerator < 1000)
		{
			aFrac2.mNumerator /= 10;
			aFrac2.mDenominator /= 10;
		}
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
