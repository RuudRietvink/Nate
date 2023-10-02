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
	string_t hello = " world";
#line 17
	std::ostringstream data1_temp;
#line 17
	data1_temp << 1;data1_temp << " " << 2;data1_temp << " sdsddsaaa " << (hello);data1_temp << ((static_cast<int32_t>(hello.size())));data1_temp << std::endl;
	data1_temp << 3;data1_temp << " " << 4;data1_temp << 5;data1_temp << " ";
	data1_temp << 6;
	data1_temp << 7;data1_temp << " " << 8;data1_temp << std::endl;
	data1_temp << "hi " << ((4 * 5));data1_temp << std::endl;
	const string_t data1= data1_temp.str();
#line 24
	std::ostringstream data2_temp;
#line 24
	data2_temp << 1;data2_temp << (data1);data2_temp << std::endl;
	const string_t data2= data2_temp.str();
#line 27
	*error << "Hello " << (hello); *error << std::endl; 
#line 29
	std::shared_ptr<std::ofstream> out = {};
	std::shared_ptr<std::ostream> out2 = out;
	*output << "Before: " << std::boolalpha ; *output << (((out && out->good()))); *output << " " << std::boolalpha ; *output << (((out && out->is_open()))); *output << std::endl; 
#line 33
	out2 = out;
	*output << std::endl; 
	if ((out && out->is_open()))
	{
#line 36
		*output << "Inside: " << std::boolalpha ; *output << (((out && out->good()))); *output << " " << std::boolalpha ; *output << (((out && out->is_open()))); *output << std::endl; 
		std::shared_ptr<std::ostream> nate__writer = out2;
#line 37
		*nate__writer << "Hello,planet "; 
		*nate__writer << 12; *nate__writer << " " << 13; *nate__writer << 14; 
		*nate__writer << 15; *nate__writer << std::endl; 
		nate__writer = output;
#line 40
		*nate__writer << "Hi "; 
		*output << "Hi "; 
		*nate__writer << "End" << std::endl; 
		*nate__writer << std::endl; 
		nate__writer = out2;
#line 44
		*nate__writer << "Hi there" << std::endl; 
	}
	*output << "After: " << std::boolalpha ; *output << (((out && out->good()))); *output << " " << std::boolalpha ; *output << (((out && out->is_open()))); *output << std::endl; 
#line 48
	int32_t cnt = 2;
	cnt = 3;
#line 52
	auto const tmp__0__ = cnt;
#line 52
	switch (tmp__0__)
	{
#line 52
	case 10:
	case 11:
	{
#line 54
		*output << "case" << std::endl; 
		break;
	}
#line 55
	case 13:
	{
#line 56
		*output << "case 13" << std::endl; 
		break;
	}
	break;
	}
#line 59
	auto const tmp__1__ = cnt;
#line 59
	switch (tmp__1__)
	{
#line 59
	case 10:
	case 11:
	{
#line 61
		*output << "case" << std::endl; 
		break;
	}
#line 62
	case 13:
	{
#line 63
		*output << "case 13" << std::endl; 
		break;
	}
	default:
	{
		*output << "else" << std::endl; 
	}
	break;
	}
#line 68
	auto const tmp__2__ = cnt;
#line 68
	if ((tmp__2__ == cnt * 4))
	{
#line 69
		*output << "hello" << std::endl; 
	}
#line 70
	else if ((tmp__2__ == cnt + 4)
#line 70

					 || (tmp__2__ == cnt - 4))
	{
#line 72
		*output << "hi" << std::endl; 
		cnt = 2;
	}
#line 76
	auto const tmp__3__ = cnt;
#line 76
	if ((tmp__3__ == cnt * 4))
	{
#line 77
		*output << "hello" << std::endl; 
	}
#line 78
	else if ((tmp__3__ == cnt + 4)
#line 78

					 || (tmp__3__ == cnt - 4))
	{
#line 80
		*output << "hi" << std::endl; 
		cnt = 2;
	}
	else
	{
		*output << "else" << std::endl; 
	}
#line 86
	auto const tmp__4__ = cnt;
#line 86
	switch (tmp__4__)
	{
#line 86
	case 10:
	case 11:
	{
#line 88
		*output << "case" << std::endl; 
		break;
	}
	{
#line 90
		*output << "hello" << std::endl; 
		break;
	}
	{
#line 93
		*output << "hi" << std::endl; 
		cnt = 2;
		break;
	}
	break;
	}
#line 97
	auto const tmp__5__ = cnt;
#line 97
	switch (tmp__5__)
	{
#line 97
	case 10:
	case 11:
	{
#line 99
		*output << "case" << std::endl; 
		break;
	}
	{
#line 101
		*output << "hello" << std::endl; 
		break;
	}
	{
#line 104
		*output << "hi" << std::endl; 
		cnt = 2;
		break;
	}
	default:
	{
#line 100
		if ((tmp__5__ == cnt * 4))
		{
#line 101
			*output << "hello" << std::endl; 
		}
#line 102
		else if ((tmp__5__ == cnt + 4)
#line 102

						 || (tmp__5__ == cnt - 4))
		{
#line 104
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
#line 110
	while (true)
	{
#line 111
		int32_t cnt = 2;
		if (!(cnt != 2)) break;
		cnt = 3;
	}
	while (true)
	{
#line 115
		if (!(cnt != 3)) break;
		int32_t cnt = 2;
		cnt = 3;
	}
	const std::string tmp__6__ = "abcd⅑⅛⅐efg";
	auto tmp__8__ = tmp__6__.cbegin();
	for (auto tmp__7__ = tmp__6__.cbegin(); tmp__7__ != tmp__6__.cend(); tmp__7__=tmp__8__)
	{
		uint32_t item = utf8::next(tmp__8__,tmp__6__.cend());
#line 120
		*output << (Core::toString(item)); *output << std::endl; 
	}
	for (int32_t lop= 1;lop<=10; lop+=1)
	{
#line 122
		if (!(lop != 20)) break;
		int32_t cnt = 2;
		*output << ((lop * cnt)); *output << std::endl; 
	}
	for (int32_t lop= 1;lop<=10; lop+=2)
	{
#line 126
		if (!(lop != 20)) break;
		int32_t cnt = 2;
		*output << ((lop * cnt)); *output << std::endl; 
	}
	for (int32_t lop= 10;lop >= 1; lop -= 1)
	{
#line 131
		int32_t cnt = 2;
		*output << ((lop * cnt)); *output << std::endl; 
	}
	for (int32_t lop= 10;lop >= 1; lop -= 2)
	{
#line 135
		int32_t cnt = 2;
		if (!(lop != 20)) break;
		*output << ((lop * cnt)); *output << std::endl; 
	}
	if (cnt == 3)
	{
#line 140
		*output << "Hello, world!" << std::endl; 
		cnt = 4;
	}
	if (cnt != 3)
	{
#line 144
		float x = static_cast<float>(3.14);
		*output << (x); *output << (x); *output << " " << (x); 
	}
#line 146
	else
	{
#line 147
		int32_t x = 3;
		*output << (x); *output << " " << ((x + 1)); *output << std::endl; 
	}
	if (cnt < 3)
	{
#line 151
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))); *output << std::endl; 
	}
#line 153
	else if (false)
	{
#line 154
		int32_t x = 3;
		*output << (x); *output << " " << ((x + 1)); *output << std::endl; 
	}
	if (cnt < 3)
	{
#line 158
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))); *output << std::endl; 
	}
#line 160
	else if (false)
	{
#line 161
		int32_t x = 3;
		*output << (x); *output << " " << ((x + 1)); *output << std::endl; 
	}
#line 163
	else if (true)
	{
#line 164
		int32_t x = 3;
		*output << "Hello, world!" << std::endl; 
	}
	if (cnt < 3)
	{
#line 168
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))); *output << std::endl; 
	}
#line 170
	else if (false)
	{
#line 171
		int32_t x = 3;
		*output << (x); *output << " " << ((x + 1)); *output << std::endl; 
	}
#line 173
	else
	{
#line 174
		int32_t x = 3;
		*output << "Hello, world!" << std::endl; 
	}
	if (cnt < 3)
	{
#line 178
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))); *output << std::endl; 
	}
#line 180
	else if (false)
	{
#line 181
		int32_t x = 3;
		*output << (x); *output << " " << ((x + 1)); *output << std::endl; 
	}
#line 183
	else if (true)
	{
#line 184
		int32_t x = 3;
		*output << "Hello, world!" << std::endl; 
	}
#line 186
	else
	{
#line 187
		int32_t y = 3;
		*output << "Bye, world!" << std::endl; 
	}
}
