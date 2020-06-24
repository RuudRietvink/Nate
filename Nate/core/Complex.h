#pragma once

#include "Core.h"

#include <string>
#include <iostream>
#include <ostream>
#include <sstream>
#include <complex>

template<typename TYPE>
class Complex
{
public:
	Complex() = default;
	Complex(const Complex<TYPE>& aValue) = default;
	
	constexpr explicit Complex(const std::complex<TYPE>& aComplex)
		: mComplex(aComplex)
	{
	}
	
  template<typename OTHER>
	constexpr Complex(const Complex<OTHER>& aComplex)
		: mComplex(static_cast<TYPE>(aComplex.real()),
							 static_cast<TYPE>(aComplex.imaginary()))
	{
	}

	constexpr Complex(TYPE aReal, TYPE aImaginary = 0)
		: mComplex(aReal, aImaginary)
	{
	}
	
	virtual ~Complex() = default;
	
	Complex<TYPE>& operator=(const Complex<TYPE>& aComplex) = default;
	Complex<TYPE>& operator=(TYPE aValue)
	{
		mComplex.real(aValue);
		mComplex.imag(TYPE{});

		return *this;
	}

	bool operator==(const Complex<TYPE>& aComplex) const
	{
		return mComplex == aComplex.mComplex;
	}

	bool operator!=(const Complex<TYPE>& aComplex) const
	{
		return !operator==(aComplex);
	}

	Complex<TYPE> operator-() const
	{
		return Complex<TYPE>(-mComplex);
	}

	Complex<TYPE> operator+(const Complex<TYPE>& aComplex) const
	{
		return Complex<TYPE>(mComplex + aComplex.mComplex);
	}

	Complex<TYPE> operator-(const Complex<TYPE>& aComplex) const
	{
		return Complex<TYPE>(mComplex - aComplex.mComplex);
	}

	Complex<TYPE> operator*(const Complex<TYPE>& aComplex) const
	{
		return Complex<TYPE>(mComplex * aComplex.mComplex);
	}

	Complex<TYPE> operator/(const Complex<TYPE>& aComplex) const
	{
		return Complex<TYPE>(mComplex / aComplex.mComplex);
	}

	friend Complex<TYPE> operator+(TYPE aValue, const Complex<TYPE>& aComplex)
	{
		return Complex<TYPE>(aValue + aComplex.value());
	}

	friend Complex<TYPE> operator+(const Complex<TYPE>& aComplex, TYPE aValue)
	{
		return Complex<TYPE>(aValue + aComplex.value());
	}

	friend Complex<TYPE> operator-(TYPE aValue, const Complex<TYPE>& aComplex)
	{
		return Complex<TYPE>(aValue - aComplex.value());
	}

	friend Complex<TYPE> operator-(const Complex<TYPE>& aComplex, TYPE aValue)
	{
		return Complex<TYPE>(aComplex.value() - aValue);
	}

	friend Complex<TYPE> operator*(TYPE aValue, const Complex<TYPE>& aComplex)
	{
		return Complex<TYPE>(aValue * aComplex.value());
	}

	friend Complex<TYPE> operator*(const Complex<TYPE>& aComplex, TYPE aValue)
	{
		return Complex<TYPE>(aValue * aComplex.value());
	}

	friend Complex<TYPE> operator/(TYPE aValue, const Complex<TYPE>& aComplex)
	{
		return Complex<TYPE>(aValue / aComplex.value());
	}

	friend Complex<TYPE> operator/(const Complex<TYPE>& aComplex, TYPE aValue)
	{
		return Complex<TYPE>(aComplex.value() / aValue);
	}
	
	constexpr std::complex<TYPE> value() const { return mComplex; }
	constexpr TYPE real() const { return mComplex.real(); }
	constexpr TYPE imaginary() const { return mComplex.imag(); }
	
	TYPE abs() const { return std::abs(mComplex); }
	Complex<TYPE> cos() const { return Complex<TYPE>(std::cos(mComplex)); }
	Complex<TYPE> sin() const { return Complex<TYPE>(std::sin(mComplex)); }
	Complex<TYPE> tan() const { return Complex<TYPE>(std::tan(mComplex)); }
	Complex<TYPE> acos() const { return Complex<TYPE>(std::acos(mComplex)); }
	Complex<TYPE> asin() const { return Complex<TYPE>(std::asin(mComplex)); }
	Complex<TYPE> atan() const { return Complex<TYPE>(std::atan(mComplex)); }
	Complex<TYPE> cosh() const { return Complex<TYPE>(std::cosh(mComplex)); }
	Complex<TYPE> sinh() const { return Complex<TYPE>(std::sinh(mComplex)); }
	Complex<TYPE> tanh() const { return Complex<TYPE>(std::tanh(mComplex)); }
	Complex<TYPE> log() const { return Complex<TYPE>(std::log(mComplex)); }
	Complex<TYPE> log10() const { return Complex<TYPE>(std::log10(mComplex)); }
	Complex<TYPE> sqrt() const { return Complex<TYPE>(std::sqrt(mComplex)); }
	Complex<TYPE> pow(TYPE aPower) const { return Complex<TYPE>(std::pow(mComplex, aPower)); }
	Complex<TYPE> pow(const Complex<TYPE>& aPower) const { return Complex<TYPE>(std::pow(mComplex, aPower.mComplex)); }
	
	void toString(std::ostream& aStream) const
	{
		if (mComplex.real() != 0 || mComplex.imag() == 0)
		{
			aStream << mComplex.real();
		}

		if (mComplex.imag() > 0)
		{
			if (mComplex.imag() != 1)
			{
				if (mComplex.real() != 0 && !(aStream.flags() & std::ios_base::showpos))
				{
					aStream << '+';
				}

				aStream << mComplex.imag() << 'i';
			}
			else 
			{
				if (aStream.flags() & std::ios_base::showpos)
				{
					aStream << '+';
				}

				aStream << 'i';
			}
		}
		else if (mComplex.imag() < 0)
		{
			if (mComplex.imag() != -1)
			{
				aStream << mComplex.imag() << 'i';
			}
			else
			{
				aStream << "-i";
			}
		}
	}
	
	std::string toString() const
	{
		std::stringstream ss;
		toString(ss);
		return ss.str();
	}

	std::string toString(const Core::Format& aFormat) const
	{
		using F = Core::Format::Flags;
		
		std::stringstream ss;
		Core::setTempFormat(ss, aFormat);
		toString(ss);
		return ss.str();
	}
	
	
private:
	std::complex<TYPE> mComplex;
};

template<typename TYPE>
std::ostream& operator<<(std::ostream& aStream, const Complex<TYPE>& aComplex)
{
	aStream << aComplex.toString();
	return aStream;
}

template<typename TYPE>
std::istream& operator>>(std::istream& aStream, Complex<TYPE>& aComplex)
{
	TYPE real;
	aStream >> real;

	if (aStream.good())
	{
		if (aStream.peek() == 'i')
		{
			aStream.get();
			aComplex = Complex<TYPE>(0, real);
		}
		else if (aStream.peek() == '+' || aStream.peek() == '-')
		{
			TYPE imag;
			aStream >> imag;
			if (aStream.good())
			{
		  	if (aStream.peek() == 'i')
		  	{
					aStream.get();
				  aComplex = Complex<TYPE>(real, imag);
				}
				else
				{
					aStream.setstate(std::ios::failbit);
				}
			}
		}
	}

	return aStream;
}

namespace std
{
	template<typename TYPE>
	TYPE abs(const Complex<TYPE>& aComplex) { return aComplex.abs(); }
	template<typename TYPE>
	Complex<TYPE> pow(const Complex<TYPE>& aComplex, double aPower) { return aComplex.pow(aPower); }
	template<typename TYPE>
	Complex<TYPE> pow(const Complex<TYPE>& aComplex, const Complex<TYPE>& aPower) { return aComplex.pow(aPower); }
	template<typename TYPE>
	Complex<TYPE> pow(double aValue, const Complex<TYPE>& aPower) { return Complex<TYPE>(std::pow(aValue, aPower.value())); }
	template<typename TYPE>
	Complex<TYPE> cos(const Complex<TYPE>& aComplex) { return aComplex.cos(); }
	template<typename TYPE>
	Complex<TYPE> sin(const Complex<TYPE>& aComplex) { return aComplex.sin(); }
	template<typename TYPE>
	Complex<TYPE> tan(const Complex<TYPE>& aComplex) { return aComplex.tan(); }
	template<typename TYPE>
	Complex<TYPE> acos(const Complex<TYPE>& aComplex) { return aComplex.acos(); }
	template<typename TYPE>
	Complex<TYPE> asin(const Complex<TYPE>& aComplex) { return aComplex.asin(); }
	template<typename TYPE>
	Complex<TYPE> atan(const Complex<TYPE>& aComplex) { return aComplex.atan(); }
	template<typename TYPE>
	Complex<TYPE> cosh(const Complex<TYPE>& aComplex) { return aComplex.cosh(); }
	template<typename TYPE>
	Complex<TYPE> sinh(const Complex<TYPE>& aComplex) { return aComplex.sinh(); }
	template<typename TYPE>
	Complex<TYPE> tanh(const Complex<TYPE>& aComplex) { return aComplex.tanh(); }
	template<typename TYPE>
	Complex<TYPE> log(const Complex<TYPE>& aComplex) { return aComplex.log(); }
	template<typename TYPE>
	Complex<TYPE> log10(const Complex<TYPE>& aComplex) { return aComplex.log10(); }
	template<typename TYPE>
	Complex<TYPE> sqrt(const Complex<TYPE>& aComplex) { return aComplex.sqrt(); }
}

namespace Core
{
	template <typename T>
	std::string outputValue(const Complex<T>& aValue, const Core::Format& aFormat)
	{
		return aValue.toString(aFormat);
	}
}