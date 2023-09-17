#include "C:\Users\ruud\source\repos\Nate\Nate\core\Core.h"
#line 1 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\input\\nate.in2"
std::shared_ptr<std::ostream> output = {};
#line 1
std::shared_ptr<std::ostream> error = {};
#line 1
std::shared_ptr<std::istream> input = {};
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
#line 8 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\core\\File-Output.ns"
#include <fstream>
#undef NOMINMAX
#define NOMINMAX
#include <windows.h>
#line 14 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\input\\nate.in2"
int main(int argc, char** argv)
{
	output = std::shared_ptr<std::ostream>(&std::cout, [](void*) {});
	error = std::shared_ptr<std::ostream>(&std::cerr, [](void*) {});
	input = std::shared_ptr<std::istream>(&std::cin, [](void*) {});
	SetConsoleOutputCP(65001);
#line 15
	std::shared_ptr<std::ofstream> out = {};
	std::shared_ptr<std::ostream> out2 = out;
	*output << "Before: " << std::boolalpha ; *output << (((out && out->good()))); *output << " " << std::boolalpha ; *output << (((out && out->is_open()))); *output << std::endl; 
#line 19
	out2 = out;
	*output << std::endl; 
	if ((out && out->is_open()))
	{
#line 22
		*output << "Inside: " << std::boolalpha ; *output << (((out && out->good()))); *output << " " << std::boolalpha ; *output << (((out && out->is_open()))); *output << std::endl; 
		std::shared_ptr<std::ostream> nate__writer = out2;
#line 23
		*nate__writer << "Hello,planet "; 
		*nate__writer << 12; *nate__writer << " " << 13; *nate__writer << 14; 
		*nate__writer << 15; *nate__writer << std::endl; 
		nate__writer = output;
#line 26
		*nate__writer << "Hi "; 
		*output << "Hi "; 
		*nate__writer << "End" << std::endl; 
		*nate__writer << std::endl; 
		nate__writer = out2;
#line 30
		*nate__writer << "Hi there" << std::endl; 
	}
	*output << "After: " << std::boolalpha ; *output << (((out && out->good()))); *output << " " << std::boolalpha ; *output << (((out && out->is_open()))); *output << std::endl; 
#line 34
	int32_t cnt = 2;
	cnt = 3;
#line 38
	auto const tmp__0__ = cnt;
#line 38
	switch (tmp__0__)
	{
#line 38
	case 10:
	case 11:
	{
#line 40
		*output << "case" << std::endl; 
		break;
	}
#line 41
	case 13:
	{
#line 42
		*output << "case 13" << std::endl; 
		break;
	}
	break;
	}
#line 45
	auto const tmp__1__ = cnt;
#line 45
	switch (tmp__1__)
	{
#line 45
	case 10:
	case 11:
	{
#line 47
		*output << "case" << std::endl; 
		break;
	}
#line 48
	case 13:
	{
#line 49
		*output << "case 13" << std::endl; 
		break;
	}
	default:
	{
		*output << "else" << std::endl; 
	}
	break;
	}
#line 54
	auto const tmp__2__ = cnt;
#line 54
	if ((tmp__2__ == cnt * 4))
	{
#line 55
		*output << "hello" << std::endl; 
	}
#line 56
	else if ((tmp__2__ == cnt + 4)
#line 56

					 || (tmp__2__ == cnt - 4))
	{
#line 58
		*output << "hi" << std::endl; 
		cnt = 2;
	}
#line 62
	auto const tmp__3__ = cnt;
#line 62
	if ((tmp__3__ == cnt * 4))
	{
#line 63
		*output << "hello" << std::endl; 
	}
#line 64
	else if ((tmp__3__ == cnt + 4)
#line 64

					 || (tmp__3__ == cnt - 4))
	{
#line 66
		*output << "hi" << std::endl; 
		cnt = 2;
	}
	else
	{
		*output << "else" << std::endl; 
	}
#line 72
	auto const tmp__4__ = cnt;
#line 72
	switch (tmp__4__)
	{
#line 72
	case 10:
	case 11:
	{
#line 74
		*output << "case" << std::endl; 
		break;
	}
	{
#line 76
		*output << "hello" << std::endl; 
		break;
	}
	{
#line 79
		*output << "hi" << std::endl; 
		cnt = 2;
		break;
	}
	break;
	}
#line 83
	auto const tmp__5__ = cnt;
#line 83
	switch (tmp__5__)
	{
#line 83
	case 10:
	case 11:
	{
#line 85
		*output << "case" << std::endl; 
		break;
	}
	{
#line 87
		*output << "hello" << std::endl; 
		break;
	}
	{
#line 90
		*output << "hi" << std::endl; 
		cnt = 2;
		break;
	}
	default:
	{
#line 86
		if ((tmp__5__ == cnt * 4))
		{
#line 87
			*output << "hello" << std::endl; 
		}
#line 88
		else if ((tmp__5__ == cnt + 4)
#line 88

						 || (tmp__5__ == cnt - 4))
		{
#line 90
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
#line 96
	while (true)
	{
#line 97
		int32_t cnt = 2;
		if (!(cnt != 2)) break;
		cnt = 3;
	}
	while (true)
	{
#line 101
		if (!(cnt != 3)) break;
		int32_t cnt = 2;
		cnt = 3;
	}
	const std::string tmp__6__ = "abcd⅑⅛⅐efg";
	auto tmp__8__ = tmp__6__.cbegin();
	for (auto tmp__7__ = tmp__6__.cbegin(); tmp__7__ != tmp__6__.cend(); tmp__7__=tmp__8__)
	{
		uint32_t item = utf8::next(tmp__8__,tmp__6__.cend());
#line 106
		*output << (Core::toString(item)); *output << std::endl; 
	}
	for (int32_t lop= 1;lop<=10; lop+=1)
	{
#line 108
		if (!(lop != 20)) break;
		int32_t cnt = 2;
		*output << ((lop * cnt)); *output << std::endl; 
	}
	for (int32_t lop= 1;lop<=10; lop+=2)
	{
#line 112
		if (!(lop != 20)) break;
		int32_t cnt = 2;
		*output << ((lop * cnt)); *output << std::endl; 
	}
	for (int32_t lop= 10;lop >= 1; lop -= 1)
	{
#line 117
		int32_t cnt = 2;
		*output << ((lop * cnt)); *output << std::endl; 
	}
	for (int32_t lop= 10;lop >= 1; lop -= 2)
	{
#line 121
		int32_t cnt = 2;
		if (!(lop != 20)) break;
		*output << ((lop * cnt)); *output << std::endl; 
	}
	if (cnt == 3)
	{
#line 126
		*output << "Hello, world!" << std::endl; 
		cnt = 4;
	}
	if (cnt != 3)
	{
#line 130
		float x = static_cast<float>(3.14);
		*output << (x); *output << (x); *output << " " << (x); 
	}
#line 132
	else
	{
#line 133
		int32_t x = 3;
		*output << (x); *output << " " << ((x + 1)); *output << std::endl; 
	}
	if (cnt < 3)
	{
#line 137
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))); *output << std::endl; 
	}
#line 139
	else if (false)
	{
#line 140
		int32_t x = 3;
		*output << (x); *output << " " << ((x + 1)); *output << std::endl; 
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
	if (cnt < 3)
	{
#line 154
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))); *output << std::endl; 
	}
#line 156
	else if (false)
	{
#line 157
		int32_t x = 3;
		*output << (x); *output << " " << ((x + 1)); *output << std::endl; 
	}
#line 159
	else
	{
#line 160
		int32_t x = 3;
		*output << "Hello, world!" << std::endl; 
	}
	if (cnt < 3)
	{
#line 164
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))); *output << std::endl; 
	}
#line 166
	else if (false)
	{
#line 167
		int32_t x = 3;
		*output << (x); *output << " " << ((x + 1)); *output << std::endl; 
	}
#line 169
	else if (true)
	{
#line 170
		int32_t x = 3;
		*output << "Hello, world!" << std::endl; 
	}
#line 172
	else
	{
#line 173
		int32_t y = 3;
		*output << "Bye, world!" << std::endl; 
	}
}
