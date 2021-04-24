#include "C:\Users\ruud\source\repos\Nate\Nate\core\Core.h"
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
#line 472 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\input\\nate.in"
int main(int argc, char** argv)
{
	output = std::shared_ptr<std::ostream>(&std::cout, [](void*) {});
	error = std::shared_ptr<std::ostream>(&std::cerr, [](void*) {});
	input = std::shared_ptr<std::istream>(&std::cin, [](void*) {});
	SetConsoleOutputCP(65001);
#line 473
	float x = static_cast<float>(Rational(3,1,2).toDouble());
	double aaa = 3.5;
	double z = 2.0;
	z = (((x * (((std::pow(((((((((((((((std::sqrt((4))) * (std::pow((x), (2))))) - (std::sqrt(((((x / 4)))))))) / ((1.3 + ((((std::sqrt(((x - 2))))) / (((((x * x)) * x)))))))))) / ((std::cos(((4 * (std::pow((aaa), (2)))))))))) * ((((((((((std::sqrt((4))) * (std::pow((x), (2))))) - (std::sqrt(((((x / 4)))))))) / ((1.3 + ((((std::sqrt(((x - 2))))) / (((((x * x)) * x)))))))))) / ((std::cos(((4 * (std::pow((aaa), (2)))))))))))))), ((x - 3.3)))) + (std::exp(((std::pow((3), (z))))))))) * (std::pow(((std::sqrt((((std::sqrt((((std::pow((x), (3))) - 4))))))))), ((std::pow(((((1 / ((x + z)))))), (((((((((2 * x)) * z))) / 2.4)))))))))) * (-(std::pow((0.34), (((-2.5) + ((x * (std::pow((((z - 5))), ((std::pow((0.3), ((std::pow((5), (4))))))))))))))))) + 5;
#line 491
	*output << (x); *output << " " << (z); *output << " " << ((std::exp(3))); *output << " " << ((std::exp((std::exp(4))))); *output << std::endl; 
}
