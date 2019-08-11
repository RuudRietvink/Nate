#pragma once

#include <string>
#include <iostream>
#include <complex>

class Complex
{
public:
	static bool isImaginaryLetter(uint32_t aLetter);
	static bool checkAndRemoveImaginaryLetter(std::string& aString);

	Complex();
	explicit Complex(float aValue);
	explicit Complex(double aValue);
	Complex(const Complex& aValue);
	
	constexpr explicit Complex(const std::complex<double>& aComplex)
		: mComplex(aComplex)
	{
	}
	constexpr Complex(double aReal, double aImaginary)
		: mComplex(aReal, aImaginary)
	{
	}
	
	virtual ~Complex() = default;

	double abs() const;
	Complex cos() const;
	Complex sin() const;
	Complex tan() const;
	Complex acos() const;
	Complex asin() const;
	Complex atan() const;
	Complex cosh() const;
	Complex sinh() const;
	Complex tanh() const;
	Complex log() const;
	Complex log10() const;
	Complex pow(double aPower) const;
	Complex pow(const Complex& aPower) const;
	Complex sqrt() const;
	
	std::string toString() const;
	
	Complex& operator=(const Complex& aComplex);
	Complex& operator=(double aValue);

	bool operator==(const Complex& aComplex) const;
	bool operator!=(const Complex& aComplex) const;

	Complex operator-() const;
	Complex operator+(const Complex& aComplex) const;
	Complex operator-(const Complex& aComplex) const;
	Complex operator*(const Complex& aComplex) const;
	Complex operator/(const Complex& aComplex) const;

	friend Complex operator+(double aValue, const Complex& aComplex);
	friend Complex operator+(const Complex& aComplex, double aValue);
	friend Complex operator-(double aValue, const Complex& aComplex);
	friend Complex operator-(const Complex& aComplex, double aValue);
	friend Complex operator*(double aValue, const Complex& aComplex);
	friend Complex operator*(const Complex& aComplex, double aValue);
	friend Complex operator/(double aValue, const Complex& aComplex);
	friend Complex operator/(const Complex& aComplex, double aValue);
	
	constexpr std::complex<double> value() const { return mComplex; }
	constexpr double real() const { return mComplex.real(); }
	constexpr double imaginary() const { return mComplex.imag(); }

private:
	std::complex<double> mComplex;
};

std::ostream& operator<<(std::ostream& aStream, const Complex& aComplex);

namespace std {
	double abs(const Complex& aComplex);
	Complex pow(const Complex& aComplex, double aPower);
	Complex pow(double aValue, const Complex& aPower);
	Complex pow(const Complex& aComplex, const Complex& aPower);
	Complex sqrt(const Complex& aComplex);
	Complex cos(const Complex& aComplex);
	Complex sin(const Complex& aComplex);
	Complex tan(const Complex& aComplex);
	Complex acos(const Complex& aComplex);
	Complex asin(const Complex& aComplex);
	Complex atan(const Complex& aComplex);
	Complex cosh(const Complex& aComplex);
	Complex sinh(const Complex& aComplex);
	Complex tanh(const Complex& aComplex);
	Complex log(const Complex& aComplex);
	Complex log10(const Complex& aComplex);
}
