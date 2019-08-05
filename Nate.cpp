
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
	Fraction frac2("⅛");
	Fraction frac3("-5⁷⁸/₆₇");
	Fraction frac4("-²³/₁₂");

	std::cerr << frac1 << " " << frac2 << " " << frac3 << " " << frac4 << std::endl;
}
