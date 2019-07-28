#include "Fraction.h"
#include "NateFunctions.h"

#include "utf8.h"
#include <inttypes.h>
#include <limits>

#include <string>
#include <utility>
#include <vector>
#include <iostream>

namespace {
	bool strtoi32(const char* aString, int32_t& aResult)
	{
		char c;
		int64_t temp;
		int scanned = sscanf_s(aString, "%" SCNd64 "%c", &temp, &c, 1);
		bool ok = (scanned == 1);
		if (temp > std::numeric_limits<int32_t>::max() || temp < std::numeric_limits<int32_t>::lowest())
		{
			ok = false;
		}
		else
		{
			aResult = static_cast<int32_t>(temp);
		}
		
		return ok;
	}

	int positionIn(const std::string& aString, uint32_t aChar)
	{
		int result = -1;

		auto iter = find(aString, aChar);
		if (iter != cend(aString))
		{
			result = static_cast<int>(std::distance(cbegin(aString), iter));
		}

		return result;
	}

	bool numberFrom(utf8::iterator<std::string::const_iterator>& aIter,
									const utf8::iterator<std::string::const_iterator>& aEnd,
									const std::string& aDigits,
									int32_t& aNumber)
	{
		bool ok = false;
		int pos = 0;
		aNumber = 0;
		std::string buf;

		while (aIter != aEnd && pos >= 0)
		{
			pos = positionIn(aDigits, *aIter);
			if (pos >= 0) 
			{
				buf.push_back('0' + pos);
				++aIter;
			}
		}

		if (!buf.empty())
		{
			ok = strtoi32(buf.c_str(), aNumber);
		}

		return ok;
	}

}

Fraction::Fraction()
{
}

bool Fraction::convertFromString(const std::string& aString)
{
	bool ok = true;
	utf8::iterator<std::string::const_iterator> iter(aString.cbegin(), aString.cbegin(), aString.cend());
	utf8::iterator<std::string::const_iterator> end(aString.cend(), aString.cbegin(), aString.cend());

	mWhole = 0;
	mNumerator = 0;
	mDenominator = 1;

	ok = numberFrom(iter, end, "0123456789", mWhole);

	if (iter == cbegin(aString) || ok)
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
			auto pos = positionIn(fractions, *iter);
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
				ok = numberFrom(iter, end, "⁰¹²³⁴⁵⁶⁷⁸⁹", mNumerator);
				if (prev != iter && iter != end && *iter == '/')
				{
					++iter;
					ok = numberFrom(iter, end, "₀₁₂₃₄₅₆₇₈₉", mDenominator);
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