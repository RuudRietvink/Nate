
#include "NateParser.h"
#include "core/Rational.h"

#include <iostream>
#include <direct.h>
#include <Windows.h>

int parse(const std::string& aIn, const std::string& aOut, NateParser::FileType aFileType = NateParser::FileType::Normal)
{
	std::ifstream in(aIn);
	std::ofstream out(aOut);
	NateParser nate(aIn, in, out, aFileType);
	auto parseResult = nate.parse();
	std::cerr << "Errors: " << nate.errorCount() << ", Warnings: " << nate.warningCount() << std::endl;
	return (parseResult != 0 || nate.errorCount() != 0) ? 1 : 0;
}

int main()
{
    // set code page to utf8
    SetConsoleOutputCP(CP_UTF8);                        

    // Enable buffering to prevent VS from chopping up UTF-8 byte sequences
    setvbuf(stdout, nullptr, _IOFBF, 1000);
    setvbuf(stderr, nullptr, _IOFBF, 1000);

	(void)_chdir("C:\\Users\\ruud\\source\\repos\\Nate\\core");
	parse("BaseObjects\\BaseObject.ns", "created\\BaseObject.cpp", NateParser::FileType::ObjectImpl);
	(void)_chdir("C:\\Users\\ruud\\source\\repos\\Nate\\input");
	parse("Hello.ns", "created\\Hello.cpp", NateParser::FileType::ObjectImpl);
	return parse("nate.in", "C:\\Users\\ruud\\source\\repos\\Nate\\Out\\Out.cpp");
}
