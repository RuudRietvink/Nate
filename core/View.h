#pragma once

template <typename T>
class Range
{
public:
	Range(const T::iterator& aBegin, const T::iterator& aEnd)
		: mBegin(aBegin),
		  mEnd(aEnd)
	{}

private:
	T::iterator& mBegin;
	T::iterator& mEnd;
};