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
}

Fraction::Fraction()
{
}

Fraction::Fraction(const std::string& aString)
{
	convertFromString(aString);
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

float Fraction::toFloat() const
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