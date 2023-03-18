#include "C:\Users\ruud\source\repos\Nate\Nate\core\Core.h"
#include "C:\Users\ruud\source\repos\Nate\Nate\input\created\File-Input.h"
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
#line 532 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\input\\nate.in"
int main(int argc, char** argv)
{
	output = std::shared_ptr<std::ostream>(&std::cout, [](void*) {});
	error = std::shared_ptr<std::ostream>(&std::cerr, [](void*) {});
	input = std::shared_ptr<std::istream>(&std::cin, [](void*) {});
	SetConsoleOutputCP(65001);
#line 568
	int32_t xCount = 37;
	int32_t yCount = 18 + 3;
	double lat = yCount * 1.73;
	double cosi = std::cos((((lat / 180) * pi)));
	double step = ((90 / xCount)) / cosi;
	int32_t ind = 0;
	*output << (lat); *output << " " << (cosi); *output << " " << (step); *output << " " << (xCount); *output << " " << (yCount); *output << std::endl; 
	for (double lon= 0.0;lon<=180.0; lon+=step)
	{
#line 576
		*output << ((ind++)); *output << " " << (lat); *output << ", " << ((-lon)); *output << std::endl; 
	}
}
