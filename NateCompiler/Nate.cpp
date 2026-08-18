
#include "NateParser.h"
#include "NateCode.h"
#include "core/Rational.h"

#include <filesystem>
#include <fstream>
#include <iostream>

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
	namespace fs = std::filesystem;

	Core::enableUtf8Console();
	setvbuf(stdout, nullptr, _IOFBF, 1000);
	setvbuf(stderr, nullptr, _IOFBF, 1000);

	const fs::path repoRoot = fs::current_path();
	const fs::path nateRoot = repoRoot / "Nate";
	const fs::path coreDir = nateRoot / "core";
	const fs::path inputDir = nateRoot / "input";
	const fs::path outDir = repoRoot / "Out";

	fs::current_path(coreDir);
	parse("BaseObject.ns", (coreDir / "created" / "BaseObject.cpp").string(), nate::NateParser::FileType::ObjectImpl);
	parse("File-Input.ns", (coreDir / "created" / "File-Input.cpp").string(), nate::NateParser::FileType::ObjectImpl);
	return parse((inputDir / "nate.in2").string(), (outDir / "Out.cpp").string());
}