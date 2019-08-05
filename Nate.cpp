
#include "NateParser.h"
#include "core/Fraction.h"

#include <iostream>

int main()
{
	//std::string filename("d:\\nate.in");
	//std::ifstream in(filename);
	//std::ofstream out("C:\\Users\\ruud\\source\\repos\\Out\\Out.cpp");
	//NateParser nate(filename, in, out);
	//return (nate.parse() != 0 || nate.errorCount() != 0) ? 1 : 0;

	Fraction frac1("23⅛");
	int frac2(-42);
	float frac3(-34.56f);
	Fraction frac4(frac1);

	std::cerr << frac1 << " " << frac2 << " " << Fraction(frac3) << " " << frac3 << " " << frac4 << std::endl;
	std::cerr << frac1 << "==" << frac2 << "=" << (frac1 == frac2) << std::endl;
	std::cerr << frac1 << "==" << frac3 << "=" << (frac1 == frac3) << std::endl;
	std::cerr << frac1 << "==" << frac4 << "=" << (frac1 == frac4) << std::endl;
	std::cerr << frac1 << "!=" << frac2 << "=" << (frac1 != frac2) << std::endl;
	std::cerr << frac1 << "!=" << frac3 << "=" << (frac1 != frac3) << std::endl;
	std::cerr << frac1 << "!=" << frac4 << "=" << (frac1 != frac4) << std::endl;
	std::cerr << frac1 << "<" << frac2 << "=" << (frac1 < frac2) << std::endl;
	std::cerr << frac1 << "<" << frac3 << "=" << (frac1 < frac3) << std::endl;
	std::cerr << frac1 << "<" << frac4 << "=" << (frac1 < frac4) << std::endl;
	std::cerr << frac1 << ">" << frac2 << "=" << (frac1 > frac2) << std::endl;
	std::cerr << frac1 << ">" << frac3 << "=" << (frac1 > frac3) << std::endl;
	std::cerr << frac1 << ">" << frac4 << "=" << (frac1 > frac4) << std::endl;
	std::cerr << frac1 << "<=" << frac2 << "=" << (frac1 <= frac2) << std::endl;
	std::cerr << frac1 << "<=" << frac3 << "=" << (frac1 <= frac3) << std::endl;
	std::cerr << frac1 << "<=" << frac4 << "=" << (frac1 <= frac4) << std::endl;
	std::cerr << frac1 << ">=" << frac2 << "=" << (frac1 >= frac2) << std::endl;
	std::cerr << frac1 << ">=" << frac3 << "=" << (frac1 >= frac3) << std::endl;
	std::cerr << frac1 << ">=" << frac4 << "=" << (frac1 >= frac4) << std::endl;
	std::cerr << frac1 << "+" << frac2 << "=" << frac1 + frac2 << std::endl;
	std::cerr << frac1 << "+" << frac3 << "=" << frac1 + frac3 << std::endl;
	std::cerr << frac1 << "+" << frac4 << "=" << frac1 + frac4 << std::endl;
	std::cerr << frac1 << "-" << frac2 << "=" << frac1 - frac2 << std::endl;
	std::cerr << frac1 << "-" << frac3 << "=" << frac1 - frac3 << std::endl;
	std::cerr << frac1 << "-" << frac4 << "=" << frac1 - frac4 << std::endl;
	std::cerr << frac2 << "-" << frac1 << "=" << frac2 - frac1 << std::endl;
	std::cerr << frac3 << "-" << frac1 << "=" << frac3 - frac1 << std::endl;
	std::cerr << frac4 << "-" << frac1 << "=" << frac4 - frac1 << std::endl;
	std::cerr << frac1 << "*" << frac2 << "=" << frac1 * frac2 << std::endl;
	std::cerr << frac1 << "*" << frac3 << "=" << frac1 * frac3 << std::endl;
	std::cerr << frac1 << "*" << frac4 << "=" << frac1 * frac4 << std::endl;
}
