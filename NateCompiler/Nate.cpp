
#include "NateParser.h"
#include "NateCode.h"
#include "core/Rational.h"

#include <iostream>
#include <direct.h>
#ifdef WIN32
#include <Windows.h>
#endif

int parse(const std::string& aIn, const std::string& aOut, nate::NateParser::FileType aFileType = nate::NateParser::FileType::Normal)
{
	std::ifstream in(aIn);
	std::ofstream out(aOut);
    nate::NateCode coder;
	nate::NateParser nate(coder, aIn, in, out, aFileType);
	auto parseResult = nate.parseAndCode();
	std::cerr << "Errors: " << nate.errorCount() << ", Warnings: " << nate.warningCount() << std::endl;
	return (parseResult != 0 || nate.errorCount() != 0) ? 1 : 0;
}

int main()
{

#ifdef WIN32
    // set code page to utf8
    SetConsoleOutputCP(CP_UTF8);                        

    // Enable buffering to prevent VS from chopping up UTF-8 byte sequences
    setvbuf(stdout, nullptr, _IOFBF, 1000);
    setvbuf(stderr, nullptr, _IOFBF, 1000);
#endif

	(void)_chdir("C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\core");
	parse("BaseObject.ns", "created\\BaseObject.cpp", nate::NateParser::FileType::ObjectImpl);
	parse("File-Input.ns", "created\\File-Input.cpp", nate::NateParser::FileType::ObjectImpl);
	//(void)_chdir("C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\input");
	//parse("Hello.ns", "created\\Hello.cpp", nate::NateParser::FileType::ObjectImpl);
	return parse("C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\input\\nate.in2", "C:\\Users\\ruud\\source\\repos\\Nate\\Out\\Out.cpp");
}