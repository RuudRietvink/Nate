
#include "NateParser.h"
#include "core/Fraction.h"

#include <iostream>
#include <direct.h>

int parse(const std::string& aIn, const std::string& aOut)
{
	std::ifstream in(aIn);
	std::ofstream out(aOut);
	NateParser nate(aIn, in, out);
	auto parseResult = nate.parse();
	std::cerr << "Errors: " << nate.errorCount() << ", Warnings: " << nate.warningCount() << std::endl;
	return (parseResult != 0 || nate.errorCount() != 0) ? 1 : 0;
}

int main()
{
	(void)_chdir("C:\\Users\\ruud\\source\\repos\\Nate\\input");
	parse("Hello.ns", "created\\Hello.cpp");
	return parse("nate.in", "C:\\Users\\ruud\\source\\repos\\Out\\Out.cpp");
}
