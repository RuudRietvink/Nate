#include "C:\Users\ruud\source\repos\Nate\Nate\core\Core.h"
std::shared_ptr<std::ostream> output = {};
std::shared_ptr<std::ostream> error = {};
#include "C:\Users\ruud\source\repos\Nate\Nate\core\Rational.h"
#include "C:\Users\ruud\source\repos\Nate\Nate\core\Complex.h"
#include <complex>
#define NOMINMAX
#include <windows.h>
#include <cmath>
#include <string>
#include <list>
#include <fstream>
#include <fstream>
#include <sstream>
#line 16 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\core\\Complex.ns"
const double i = 1;
#line 32 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\core\\Math.ns"
const double pi = 3.14159265358979323;
#line 34
const double tau = 6.283185307179586;
#line 36
const double e = 2.718281828459045;
#define NOMINMAX
#include <windows.h>
#line 11 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\input\\nate.in"
int main(int argc, char** argv)
{
	output = std::shared_ptr<std::ostream>(&std::cout, [](void*) {});
	error = std::shared_ptr<std::ostream>(&std::cerr, [](void*) {});
	SetConsoleOutputCP(65001);
#line 12
	float x = {};
#line 12
	float z = {};
#line 12
	float a__2081_ = {};
	int32_t d = 5;
#line 13
	int32_t g = d;
	double f = 4.4;
	x = 	a__2081_ = static_cast<float>(3);
	*output << "Hello, world " << 3;*output << std::endl;
	*output << (x);*output << (d);*output << (f);*output << " ";
	*output << std::endl;
}
