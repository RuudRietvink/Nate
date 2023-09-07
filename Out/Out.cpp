#include "C:\Users\ruud\source\repos\Nate\Nate\core\Core.h"
#line 1 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\input\\nate.in2"
std::shared_ptr<std::ostream> output = {};
#line 1
std::shared_ptr<std::ostream> error = {};
#line 1
std::shared_ptr<std::istream> input = {};
#define NOMINMAX
#include <windows.h>
#line 11
int main(int argc, char** argv)
{
	output = std::shared_ptr<std::ostream>(&std::cout, [](void*) {});
	error = std::shared_ptr<std::ostream>(&std::cerr, [](void*) {});
	input = std::shared_ptr<std::istream>(&std::cin, [](void*) {});
	SetConsoleOutputCP(65001);
#line 12
	int32_t cnt = 2;
	cnt = 3;
	if (cnt == 3)
	{
#line 15
		*output << "Hello, world!" << std::endl; 
		cnt = 4;
	}
	if (cnt != 3)
	{
#line 19
		float x = static_cast<float>(3.14);
		*output << (x); *output << (x); *output << " " << (x); *output << std::endl; 
	}
#line 21
	else
	{
#line 22
		int32_t x = 3;
		*output << (x); *output << " " << ((x + 1)); *output << std::endl; 
	}
}
