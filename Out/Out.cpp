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
	break;
	}
#line 25
	auto const tmp__1__ = cnt;
#line 25
	switch (tmp__1__)
	{
#line 25
	case 10:
	case 11:
	{
#line 27
		*output << "case" << std::endl; 
		break;
	}
#line 28
	case 13:
	{
#line 29
		*output << "case 13" << std::endl; 
		break;
	}
	default:
	{
		*output << "else" << std::endl; 
	}
	break;
	}
#line 34
	auto const tmp__2__ = cnt;
#line 34
	if ((tmp__2__ == cnt * 4))
	{
#line 35
		*output << "hello" << std::endl; 
	}
#line 36
	else if ((tmp__2__ == cnt + 4)
#line 36

					 || (tmp__2__ == cnt - 4))
	{
#line 38
		*output << "hi" << std::endl; 
		cnt = 2;
	}
#line 42
	auto const tmp__3__ = cnt;
#line 42
	if ((tmp__3__ == cnt * 4))
	{
#line 43
		*output << "hello" << std::endl; 
	}
#line 44
	else if ((tmp__3__ == cnt + 4)
#line 44

					 || (tmp__3__ == cnt - 4))
	{
#line 46
		*output << "hi" << std::endl; 
		cnt = 2;
	}
	else
	{
		*output << "else" << std::endl; 
	}
#line 52
	auto const tmp__4__ = cnt;
#line 52
	switch (tmp__4__)
	{
#line 52
	case 10:
	case 11:
	{
#line 54
		*output << "case" << std::endl; 
		break;
	}
	{
#line 56
		*output << "hello" << std::endl; 
		break;
	}
	{
#line 59
		*output << "hi" << std::endl; 
		cnt = 2;
		break;
	}
	break;
	}
#line 63
	auto const tmp__5__ = cnt;
#line 63
	switch (tmp__5__)
	{
#line 63
	case 10:
	case 11:
	{
#line 65
		*output << "case" << std::endl; 
		break;
	}
	{
#line 67
		*output << "hello" << std::endl; 
		break;
	}
	{
#line 70
		*output << "hi" << std::endl; 
		cnt = 2;
		break;
	}
	default:
	{
#line 66
		if ((tmp__5__ == cnt * 4))
		{
#line 67
			*output << "hello" << std::endl; 
		}
#line 68
		else if ((tmp__5__ == cnt + 4)
#line 68

						 || (tmp__5__ == cnt - 4))
		{
#line 70
			*output << "hi" << std::endl; 
			cnt = 2;
		}
		else
		{
			*output << "else" << std::endl; 
		}
	}
	break;
	}
#line 76
	while (true)
	{
#line 77
		int32_t cnt = 2;
		if (!(cnt != 2)) break;
		cnt = 3;
	}
	while (true)
	{
#line 81
		if (!(cnt != 3)) break;
		int32_t cnt = 2;
		cnt = 3;
	}
	const std::string tmp__6__ = "abcd⅑⅛⅐efg";
	auto tmp__8__ = tmp__6__.cbegin();
	for (auto tmp__7__ = tmp__6__.cbegin(); tmp__7__ != tmp__6__.cend(); tmp__7__=tmp__8__)
	{
		uint32_t item = utf8::next(tmp__8__,tmp__6__.cend());
#line 86
		*output << (Core::toString(item)); *output << std::endl; 
	}
	for (int32_t lop= 1;lop<=10; lop+=1)
	{
#line 88
		if (!(lop != 20)) break;
		int32_t cnt = 2;
		*output << ((lop * cnt)); *output << std::endl; 
	}
	for (int32_t lop= 1;lop<=10; lop+=2)
	{
#line 92
		if (!(lop != 20)) break;
		int32_t cnt = 2;
		*output << ((lop * cnt)); *output << std::endl; 
	}
	for (int32_t lop= 10;lop >= 1; lop -= 1)
	{
#line 97
		int32_t cnt = 2;
		*output << ((lop * cnt)); *output << std::endl; 
	}
	for (int32_t lop= 10;lop >= 1; lop -= 2)
	{
#line 101
		int32_t cnt = 2;
		if (!(lop != 20)) break;
		*output << ((lop * cnt)); *output << std::endl; 
	}
	if (cnt == 3)
	{
#line 106
		*output << "Hello, world!" << std::endl; 
		cnt = 4;
	}
	if (cnt != 3)
	{
#line 110
		float x = static_cast<float>(3.14);
		*output << (x); *output << (x); *output << " " << (x); 
	}
#line 112
	else
	{
#line 113
		int32_t x = 3;
		*output << (x); *output << " " << ((x + 1)); *output << std::endl; 
	}
	if (cnt < 3)
	{
#line 117
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))); *output << std::endl; 
	}
#line 119
	else if (false)
	{
#line 120
		int32_t x = 3;
		*output << (x); *output << " " << ((x + 1)); *output << std::endl; 
	}
	if (cnt < 3)
	{
#line 124
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))); *output << std::endl; 
	}
#line 126
	else if (false)
	{
#line 127
		int32_t x = 3;
		*output << (x); *output << " " << ((x + 1)); *output << std::endl; 
	}
#line 129
	else if (true)
	{
#line 130
		int32_t x = 3;
		*output << "Hello, world!" << std::endl; 
	}
	if (cnt < 3)
	{
#line 134
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))); *output << std::endl; 
	}
#line 136
	else if (false)
	{
#line 137
		int32_t x = 3;
		*output << (x); *output << " " << ((x + 1)); *output << std::endl; 
	}
#line 139
	else
	{
#line 140
		int32_t x = 3;
		*output << "Hello, world!" << std::endl; 
	}
	if (cnt < 3)
	{
#line 144
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))); *output << std::endl; 
	}
#line 146
	else if (false)
	{
#line 147
		int32_t x = 3;
		*output << (x); *output << " " << ((x + 1)); *output << std::endl; 
	}
#line 149
	else if (true)
	{
#line 150
		int32_t x = 3;
		*output << "Hello, world!" << std::endl; 
	}
#line 152
	else
	{
#line 153
		int32_t y = 3;
		*output << "Bye, world!" << std::endl; 
	}
}
