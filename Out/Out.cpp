#include "C:\Users\ruud\source\repos\Nate\Nate\core\Core.h"
std::shared_ptr<std::ostream> output = {};
std::shared_ptr<std::ostream> error = {};
#define NOMINMAX
#include <windows.h>
#line 1 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\input\\nate.in"
int main(int argc, char** argv)
{
	output = std::shared_ptr<std::ostream>(&std::cout, [](void*) {});
	error = std::shared_ptr<std::ostream>(&std::cerr, [](void*) {});
	SetConsoleOutputCP(65001);
#line 2
	*output << "Hello, world " << 3;*output << std::endl;
}

