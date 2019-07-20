
#include "NateParser.h"

#include <fstream>

int main()
{
	std::string filename("d:\\nate.in");
	std::ifstream in(filename);
	std::ofstream out("C:\\Users\\ruud\\source\\repos\\Out\\Out.cpp");
	NateParser nate(filename, in, out);
	return (nate.parse() != 0 || nate.errorCount() != 0) ? 1 : 0;
}