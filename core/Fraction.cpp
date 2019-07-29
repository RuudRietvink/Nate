#include "Fraction.h"

#include "core.h"
#include <inttypes.h>
#include <limits>

#include <string>
#include <utility>
#include <vector>
#include <iostream>

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

	ok = Core::numberFrom(iter, end, "0123456789", mWhole);

	if (iter == Core::cbegin(aString) || ok)
	{
		std::cerr << "checking fraction " << *iter << std::endl;
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
				ok = Core::numberFrom(iter, end, "⁰¹²³⁴⁵⁶⁷⁸⁹", mNumerator);
				if (prev != iter && iter != end && *iter == '/')
				{
					++iter;
					ok = Core::numberFrom(iter, end, "₀₁₂₃₄₅₆₇₈₉", mDenominator);
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