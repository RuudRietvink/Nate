
#include "NateParser.h"
#include "core/Fraction.h"

#include <iostream>
#include <direct.h>

int main()
{
	(void)_chdir("C:\\Users\\ruud\\source\\repos\\Nate\\input");
	std::string filename("nate.in");
	std::ifstream in(filename);
	std::ofstream out("C:\\Users\\ruud\\source\\repos\\Out\\Out.cpp");
	NateParser nate(filename, in, out);
	auto parseResult = nate.parse();
	std::cerr << "Errors: " << nate.errorCount() << ", Warnings: " << nate.warningCount() << std::endl;
	return (parseResult != 0 || nate.errorCount() != 0) ? 1 : 0;

	/*
	std::vector<Fraction> fracs = {
		Fraction("123⅛"),
		Fraction(-42),
		Fraction(34.56f),
		Fraction(13, 0, 1),
		Fraction(13, 4, 10),
		Fraction(13, 5, 10),
		Fraction(13, 7, 10),
	};
	
	for (auto f1 : fracs)
	{
		std::cerr << "-------------------" << std::endl;
		std::cerr << "trunc " << f1 << "=" << f1.trunc() << " " << (-f1).trunc() << std::endl;
		std::cerr << "round " << f1 << "=" << f1.round() << " " << (-f1).round() << std::endl;
		std::cerr << "floor " << f1 << "=" << f1.floor() << " " << (-f1).floor() << std::endl;
		std::cerr << "ceil  " << f1 << "=" << std::ceil(f1)  << " " << (-f1).ceil()  << std::endl;
		std::cerr << "abs  " << f1 << "=" << std::abs(f1)  << " " << (-f1).abs()  << std::endl;

		//for (auto f2 : fracs)
		//{
		//	std::cerr << f1 << "==" << f2 << "=" << (f1 == f2) << std::endl;
		//	std::cerr << f1 << "!=" << f2 << "=" << (f1 != f2) << std::endl;
		//	std::cerr << f1 << "<"  << f2 << "=" << (f1 < f2) << std::endl;
		//	std::cerr << f1 << ">"  << f2 << "=" << (f1 > f2) << std::endl;
		//	std::cerr << f1 << "<="  << f2 << "=" << (f1 <= f2) << std::endl;
		//	std::cerr << f1 << ">="  << f2 << "=" << (f1 >= f2) << std::endl;
		//	std::cerr << f1 << "+"  << f2 << "=" << (f1 + f2) << std::endl;
		//	std::cerr << f1 << "-"  << f2 << "=" << (f1 - f2) << std::endl;
		//	std::cerr << f1 << "*"  << f2 << "=" << (f1 * f2) << std::endl;
		//	std::cerr << f1 << "/"  << f2 << "=" << (f1 / f2) << std::endl;
		//	std::cerr << f1 << "%"  << f2 << "=" << (f1 % f2) << std::endl;
		//}
	}
		*/
}
