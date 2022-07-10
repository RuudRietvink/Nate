#include "C:\Users\ruud\source\repos\Nate\Nate\core\Core.h"
#include "C:\Users\ruud\source\repos\Nate\Nate\input\created\IFile.h"
#include "C:\Users\ruud\source\repos\Nate\Nate\input\created\Hello.h"
#line 1 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\input\\nate.in"
std::shared_ptr<std::ostream> output;
#line 1
std::shared_ptr<std::ostream> error;
#line 1
std::shared_ptr<std::istream> input;
#line 6 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\core\\Rational.ns"
#include "C:\Users\ruud\source\repos\Nate\Nate\core\Rational.h"
#line 12 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\core\\Complex.ns"
#include "C:\Users\ruud\source\repos\Nate\Nate\core\Complex.h"
#include <complex>
#line 16
const double i = 1;
#line 27 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\core\\Math.ns"
#define NOMINMAX
#include <windows.h>
#include <cmath>
#line 32
const double pi = 3.14159265358979323;
#line 34
const double tau = 6.283185307179586;
#line 36
const double e = 2.718281828459045;
#line 6 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\core\\Text.ns"
#include <string>
typedef std::string string_t;
#line 6 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\core\\List.ns"
#include <list>
#line 8 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\core\\File-Output.ns"
#include <fstream>
#line 8 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\core\\File-Input.ns"
#include <fstream>
#line 8 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\core\\Data-input.ns"
#include <sstream>
#define NOMINMAX
#include <windows.h>
#line 530 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\input\\nate.in"
int main(int argc, char** argv)
{
	output = std::shared_ptr<std::ostream>(&std::cout, [](void*) {});
	error = std::shared_ptr<std::ostream>(&std::cerr, [](void*) {});
	input = std::shared_ptr<std::istream>(&std::cin, [](void*) {});
	SetConsoleOutputCP(65001);
#line 531
	int32_t cnt = 1;
	if (cnt > 0)
	{
#line 533
		*output << (cnt); *output << std::endl; 
	}
#line 534
	else
	{
#line 535
		*error << "Oops" << std::endl; 
#line 532
	}
#line 536
	if (cnt == 0)
	{
#line 537
		*output << (cnt); *output << std::endl; 
		cnt += 1;
	}
#line 539
	else if (cnt < 0)
	{
#line 540
		cnt = (std::sin(cnt)) + (std::cos(cnt));
#line 542
		*output << ((std::pow(cnt, 3))); *output << std::endl; 
	}
#line 543
	else
	{
#line 544
		*error << "Oops" << std::endl; 
#line 536
	}
#line 546
	std::shared_ptr<Hello> hello = Hello::new_Hello_();
}
