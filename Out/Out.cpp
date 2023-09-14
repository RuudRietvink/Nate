#include "C:\Users\ruud\source\repos\Nate\Nate\core\Core.h"
#line 1 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\input\\nate.in2"
std::shared_ptr<std::ostream> output = {};
#line 1
std::shared_ptr<std::ostream> error = {};
#line 1
std::shared_ptr<std::istream> input = {};
#line 32 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\core\\Math.ns"
const double pi = 3.14159265358979323;
#line 34
const double tau = 6.283185307179586;
#line 36
const double e = 2.718281828459045;
#define NOMINMAX
#include <windows.h>
#line 13 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\input\\nate.in2"
int main(int argc, char** argv)
{
	output = std::shared_ptr<std::ostream>(&std::cout, [](void*) {});
	error = std::shared_ptr<std::ostream>(&std::cerr, [](void*) {});
	input = std::shared_ptr<std::istream>(&std::cin, [](void*) {});
	SetConsoleOutputCP(65001);
#line 14
	int32_t cnt = 2;
	cnt = 3;
#line 18
	auto const tmp__0__ = cnt;
#line 18
	switch (tmp__0__)
	{
#line 18
	case 10:
	case 11:
	{
#line 20
		*output << "case" << std::endl; 
		break;
	}
#line 21
	case 13:
	{
#line 22
		*output << "case 13" << std::endl; 
		break;
	}
	default:
	{
		*output << "else" << std::endl; 
	}
	break;
	}
#line 27
	auto const tmp__1__ = cnt;
#line 30
	if ((tmp__1__ == cnt * 4))
	{
#line 31
		*output << "hello" << std::endl; 
	}
#line 32
	else if ((tmp__1__ == cnt + 4)
#line 32

					 || (tmp__1__ == cnt - 4))
	{
#line 34
		*output << "hi" << std::endl; 
		cnt = 2;
	}
	else
	{
#line 27
		switch (tmp__1__)
		{
#line 27
		case 10:
		case 11:
		{
#line 29
			*output << "case" << std::endl; 
			break;
		}
		{
#line 31
			*output << "hello" << std::endl; 
			break;
		}
		{
#line 34
			*output << "hi" << std::endl; 
			cnt = 2;
			break;
		}
		default:
		{
			*output << "else" << std::endl; 
		}
		break;
		}
	}
#line 40
	while (true)
	{
#line 41
		int32_t cnt = 2;
		if (!(cnt != 2)) break;
		cnt = 3;
	}
	while (true)
	{
#line 45
		if (!(cnt != 3)) break;
		int32_t cnt = 2;
		cnt = 3;
	}
	const std::string tmp__2__ = "abcd⅑⅛⅐efg";
	auto tmp__4__ = tmp__2__.cbegin();
	for (auto tmp__3__ = tmp__2__.cbegin(); tmp__3__ != tmp__2__.cend(); tmp__3__=tmp__4__)
	{
		uint32_t item = utf8::next(tmp__4__,tmp__2__.cend());
#line 50
		*output << (Core::toString(item)); *output << std::endl; 
	}
	for (int32_t lop= 1;lop<=10; lop+=1)
	{
#line 52
		if (!(lop != 20)) break;
		int32_t cnt = 2;
		*output << ((lop * cnt)); *output << std::endl; 
	}
	for (int32_t lop= 1;lop<=10; lop+=2)
	{
#line 56
		if (!(lop != 20)) break;
		int32_t cnt = 2;
		*output << ((lop * cnt)); *output << std::endl; 
	}
	for (int32_t lop= 10;lop >= 1; lop -= 1)
	{
#line 61
		int32_t cnt = 2;
		*output << ((lop * cnt)); *output << std::endl; 
	}
	for (int32_t lop= 10;lop >= 1; lop -= 2)
	{
#line 65
		int32_t cnt = 2;
		if (!(lop != 20)) break;
		*output << ((lop * cnt)); *output << std::endl; 
	}
	if (cnt == 3)
	{
#line 70
		*output << "Hello, world!" << std::endl; 
		cnt = 4;
	}
	if (cnt != 3)
	{
#line 74
		float x = static_cast<float>(3.14);
		*output << (x); *output << (x); *output << " " << (x); 
	}
#line 76
	else
	{
#line 77
		int32_t x = 3;
		*output << (x); *output << " " << ((x + 1)); *output << std::endl; 
	}
	if (cnt < 3)
	{
#line 81
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))); *output << std::endl; 
	}
#line 83
	else if (false)
	{
#line 84
		int32_t x = 3;
		*output << (x); *output << " " << ((x + 1)); *output << std::endl; 
	}
	if (cnt < 3)
	{
#line 88
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))); *output << std::endl; 
	}
#line 90
	else if (false)
	{
#line 91
		int32_t x = 3;
		*output << (x); *output << " " << ((x + 1)); *output << std::endl; 
	}
#line 93
	else if (true)
	{
#line 94
		int32_t x = 3;
		*output << "Hello, world!" << std::endl; 
	}
	if (cnt < 3)
	{
#line 98
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))); *output << std::endl; 
	}
#line 100
	else if (false)
	{
#line 101
		int32_t x = 3;
		*output << (x); *output << " " << ((x + 1)); *output << std::endl; 
	}
#line 103
	else
	{
#line 104
		int32_t x = 3;
		*output << "Hello, world!" << std::endl; 
	}
	if (cnt < 3)
	{
#line 108
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))); *output << std::endl; 
	}
#line 110
	else if (false)
	{
#line 111
		int32_t x = 3;
		*output << (x); *output << " " << ((x + 1)); *output << std::endl; 
	}
#line 113
	else if (true)
	{
#line 114
		int32_t x = 3;
		*output << "Hello, world!" << std::endl; 
	}
#line 116
	else
	{
#line 117
		int32_t y = 3;
		*output << "Bye, world!" << std::endl; 
	}
}
