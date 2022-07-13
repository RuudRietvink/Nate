#include "C:\Users\ruud\source\repos\Nate\Nate\core\Core.h"
#include "C:\Users\ruud\source\repos\Nate\Nate\input\created\Hello.h"
#line 1 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\input\\nate.in"
std::shared_ptr<std::ostream> output;
#line 1
std::shared_ptr<std::ostream> error;
#line 1
std::shared_ptr<std::istream> input;
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
#define NOMINMAX
#include <windows.h>
#line 531 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\input\\nate.in"
int main(int argc, char** argv)
{
	output = std::shared_ptr<std::ostream>(&std::cout, [](void*) {});
	error = std::shared_ptr<std::ostream>(&std::cerr, [](void*) {});
	input = std::shared_ptr<std::istream>(&std::cin, [](void*) {});
	SetConsoleOutputCP(65001);
#line 532
	int32_t cnt = 1;
	if (cnt > 0)
	{
#line 534
		*output << (cnt); *output << std::endl; 
	}
#line 535
	else
	{
#line 536
		*error << "Oops" << std::endl; 
#line 533
	}
#line 537
	if (cnt == 0)
	{
#line 538
		*output << (cnt); *output << std::endl; 
		cnt += 1;
	}
#line 540
	else if (cnt < 0)
	{
#line 541
		cnt = (std::sin(cnt)) + (std::cos(cnt));
#line 543
		*output << ((std::pow(cnt, 3))); *output << std::endl; 
	}
#line 544
	else
	{
#line 545
		*error << "Oops" << std::endl; 
#line 537
	}
#line 547
	std::shared_ptr<Hello> hello = Hello::new_Hello_();
}
