#include "Complex.h"
#include "Core.h"

#include <sstream>

Complex::Complex()
{
}

Complex::Complex(const Complex& aValue)
: mComplex(aValue.mComplex)
{
}

Complex::Complex(float aValue)
{
	mComplex.real(aValue);
}

Complex::Complex(double aValue)
{
	mComplex.real(aValue);
}

bool Complex::isImaginaryLetter(uint32_t aLetter)
{	
	return (aLetter == 'i' || aLetter == 0x1D456 /*𝑖*/);
}

bool Complex::checkAndRemoveImaginaryLetter(std::string& aString)
{
  auto last = Core::lastOf(aString);
  auto lastChar = utf8::peek_next(last, aString.cend());
  bool result = Complex::isImaginaryLetter(lastChar);

	if (result)
  {
    aString.erase(last, aString.cend());
  }

	return result;
}


double Complex::abs() const
{
	return std::abs(mComplex);
}

Complex Complex::cos() const { return Complex(std::cos(mComplex)); }
Complex Complex::sin() const { return Complex(std::sin(mComplex)); }
Complex Complex::tan() const { return Complex(std::tan(mComplex)); }
Complex Complex::acos() const { return Complex(std::acos(mComplex)); }
Complex Complex::asin() const { return Complex(std::asin(mComplex)); }
Complex Complex::atan() const { return Complex(std::atan(mComplex)); }
Complex Complex::cosh() const { return Complex(std::cosh(mComplex)); }
Complex Complex::sinh() const { return Complex(std::sinh(mComplex)); }
Complex Complex::tanh() const { return Complex(std::tanh(mComplex)); }
Complex Complex::log() const { return Complex(std::log(mComplex)); }
Complex Complex::log10() const { return Complex(std::log10(mComplex)); }
Complex Complex::sqrt() const { return Complex(std::sqrt(mComplex)); }

Complex Complex::pow(double aPower) const
{
	return Complex(std::pow(mComplex, aPower));
}

Complex Complex::pow(const Complex& aPower) const
{
	return Complex(std::pow(mComplex, aPower.mComplex));
}

Complex& Complex::operator=(const Complex& aComplex)
{
	if (this != &aComplex)
	{
		mComplex = aComplex.mComplex;
	}

	return *this;
}

Complex& Complex::operator=(double aValue)
{
	mComplex.real(aValue);
	mComplex.imag(0.0);

	return *this;
}

bool Complex::operator==(const Complex& aComplex) const
{
	return mComplex == aComplex.mComplex;
}

bool Complex::operator!=(const Complex& aComplex) const
{
	return !operator==(aComplex);
}

Complex Complex::operator-() const
{
	return Complex(-mComplex);
}

Complex Complex::operator+(const Complex& aComplex) const
{
	return Complex(mComplex + aComplex.mComplex);
}

Complex Complex::operator-(const Complex& aComplex) const
{
	return Complex(mComplex - aComplex.mComplex);
}

Complex Complex::operator*(const Complex& aComplex) const
{
	return Complex(mComplex * aComplex.mComplex);
}

Complex Complex::operator/(const Complex& aComplex) const
{
	return Complex(mComplex / aComplex.mComplex);
}

Complex operator+(double aValue, const Complex& aComplex)
{
	return Complex(aValue + aComplex.value());
}

Complex operator-(double aValue, const Complex& aComplex)
{
	return Complex(aValue - aComplex.value());
}

Complex operator*(double aValue, const Complex& aComplex)
{
	return Complex(aValue * aComplex.value());
}

Complex operator*(const Complex& aComplex, double aValue)
{
	return Complex(aValue * aComplex.value());
}

Complex operator/(double aValue, const Complex& aComplex)
{
	return Complex(aValue / aComplex.value());
}

Complex operator/(const Complex& aComplex, double aValue)
{
	return Complex(aComplex.value() / aValue);
}

std::string Complex::toString() const
{
	std::stringstream ss;
	
	if (mComplex.real() != 0 || mComplex.imag() == 0)
	{
		ss << mComplex.real();
	}

	if (mComplex.imag() > 0)
	{
		if (mComplex.real() != 0)
		{
			ss << "+";
		}

		ss << mComplex.imag() << "i";
	}
	else if (mComplex.imag() < 0)
	{
		ss << "-" << -mComplex.imag() << "i";
	}

	return ss.str();
}

std::ostream& operator<<(std::ostream& aStream, const Complex& aComplex)
{
	aStream << aComplex.toString();
	return aStream;
}

namespace std
{
	double abs(const Complex& aComplex) { return aComplex.abs(); }
	Complex pow(const Complex& aComplex, double aPower) { return aComplex.pow(aPower); }
	Complex pow(const Complex& aComplex, const Complex& aPower) { return aComplex.pow(aPower); }
	Complex pow(double aValue, const Complex& aPower) { return Complex(std::pow(aValue, aPower.value())); }
	Complex cos(const Complex& aComplex) { return aComplex.cos(); }
	Complex sin(const Complex& aComplex) { return aComplex.sin(); }
	Complex tan(const Complex& aComplex) { return aComplex.tan(); }
	Complex acos(const Complex& aComplex) { return aComplex.acos(); }
	Complex asin(const Complex& aComplex) { return aComplex.asin(); }
	Complex atan(const Complex& aComplex) { return aComplex.atan(); }
	Complex cosh(const Complex& aComplex) { return aComplex.cosh(); }
	Complex sinh(const Complex& aComplex) { return aComplex.sinh(); }
	Complex tanh(const Complex& aComplex) { return aComplex.tanh(); }
	Complex log(const Complex& aComplex) { return aComplex.log(); }
	Complex log10(const Complex& aComplex) { return aComplex.log10(); }
	Complex sqrt(const Complex& aComplex) { return aComplex.sqrt(); }
}
