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
#line 8 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\core\\Data-Input.ns"
#include <sstream>
#undef NOMINMAX
#define NOMINMAX
#include <windows.h>
#line 15 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\input\\nate.in2"
int main(int argc, char** argv)
{
	output = std::shared_ptr<std::ostream>(&std::cout, [](void*) {});
	error = std::shared_ptr<std::ostream>(&std::cerr, [](void*) {});
	input = std::shared_ptr<std::istream>(&std::cin, [](void*) {});
	SetConsoleOutputCP(65001);
#line 16
	string_t hello = " world";
#line 18
	std::ostringstream data1_temp;
#line 18
	data1_temp << 1;data1_temp << " " << 2;data1_temp << " sdsddsaaa " << (hello);data1_temp << ((static_cast<int32_t>(hello.size())));data1_temp << std::endl;
	data1_temp << 3;data1_temp << " " << 4;data1_temp << 5;data1_temp << " ";
	data1_temp << 6;
	data1_temp << 7;data1_temp << " " << 8;data1_temp << std::endl;
	data1_temp << "hi " << ((4 * 5));data1_temp << std::endl;
	const string_t data1= data1_temp.str();
#line 25
	std::ostringstream data2_temp;
#line 25
	data2_temp << 1;data2_temp << (data1);data2_temp << std::endl;
	const string_t data2= data2_temp.str();
#line 28
	*error << "Hello " << (hello); *error << std::endl; 
#line 30
	std::shared_ptr<std::istringstream> in = {};
	in.reset(new std::istringstream(data1));
#line 33
	int32_t x = {};
#line 33
	int32_t y = {};
	string_t s = {};
	std::shared_ptr<std::istream> nate__reader = in;
#line 35
	*nate__reader >> x;
	*nate__reader >> std::ws >> y;
	*nate__reader >> std::ws >> s;
	(*nate__reader).ignore(std::numeric_limits<std::streamsize>::max(), '\n');
#line 36
	*output << (x); *output << " " << (y); *output << std::endl; 
	*output << (s); *output << std::endl; 
#line 39
	std::shared_ptr<std::ofstream> out = {};
	std::shared_ptr<std::ostream> out2 = out;
	*output << "Before: " << std::boolalpha ; *output << (((out && out->good()))); *output << " " << std::boolalpha ; *output << (((out && out->is_open()))); *output << std::endl; 
	out.reset(new std::ofstream("d:\\uit", std::ios::out));
	out2 = out;
	*output << std::endl; 
	if ((out && out->is_open()))
	{
#line 46
		*output << "Inside: " << std::boolalpha ; *output << (((out && out->good()))); *output << " " << std::boolalpha ; *output << (((out && out->is_open()))); *output << std::endl; 
		std::shared_ptr<std::ostream> nate__writer = out2;
#line 47
		*nate__writer << "Hello,planet "; 
		*nate__writer << 12; *nate__writer << " " << 13; *nate__writer << 14; 
		*nate__writer << 15; *nate__writer << std::endl; 
		nate__writer = output;
#line 50
		*nate__writer << "Hi "; 
		*output << "Hi "; 
		*nate__writer << "End" << std::endl; 
		*nate__writer << std::endl; 
		nate__writer = out2;
#line 54
		*nate__writer << "Hi there" << std::endl; 
		if (out) out->close();
	}
#line 56
	*output << "After: " << std::boolalpha ; *output << (((out && out->good()))); *output << " " << std::boolalpha ; *output << (((out && out->is_open()))); *output << std::endl; 
#line 58
	int32_t cnt = 2;
	cnt = 3;
#line 62
	auto const tmp__0__ = cnt;
#line 62
	switch (tmp__0__)
	{
#line 62
	case 10:
	case 11:
	{
#line 64
		*output << "case" << std::endl; 
		break;
	}
#line 65
	case 13:
	{
#line 66
		*output << "case 13" << std::endl; 
		break;
	}
	break;
	}
#line 69
	auto const tmp__1__ = cnt;
#line 69
	switch (tmp__1__)
	{
#line 69
	case 10:
	case 11:
	{
#line 71
		*output << "case" << std::endl; 
		break;
	}
#line 72
	case 13:
	{
#line 73
		*output << "case 13" << std::endl; 
		break;
	}
	default:
	{
		*output << "else" << std::endl; 
	}
	break;
	}
#line 78
	auto const tmp__2__ = cnt;
#line 78
	if ((tmp__2__ == cnt * 4))
	{
#line 79
		*output << "hello" << std::endl; 
	}
#line 80
	else if ((tmp__2__ == cnt + 4)
#line 80

					 || (tmp__2__ == cnt - 4))
	{
#line 82
		*output << "hi" << std::endl; 
		cnt = 2;
	}
#line 86
	auto const tmp__3__ = cnt;
#line 86
	if ((tmp__3__ == cnt * 4))
	{
#line 87
		*output << "hello" << std::endl; 
	}
#line 88
	else if ((tmp__3__ == cnt + 4)
#line 88

					 || (tmp__3__ == cnt - 4))
	{
#line 90
		*output << "hi" << std::endl; 
		cnt = 2;
	}
	else
	{
		*output << "else" << std::endl; 
	}
#line 96
	auto const tmp__4__ = cnt;
#line 96
	switch (tmp__4__)
	{
#line 96
	case 10:
	case 11:
	{
#line 98
		*output << "case" << std::endl; 
		break;
	}
	{
#line 100
		*output << "hello" << std::endl; 
		break;
	}
	{
#line 103
		*output << "hi" << std::endl; 
		cnt = 2;
		break;
	}
	break;
	}
#line 107
	auto const tmp__5__ = cnt;
#line 107
	switch (tmp__5__)
	{
#line 107
	case 10:
	case 11:
	{
#line 109
		*output << "case" << std::endl; 
		break;
	}
	{
#line 111
		*output << "hello" << std::endl; 
		break;
	}
	{
#line 114
		*output << "hi" << std::endl; 
		cnt = 2;
		break;
	}
	default:
	{
#line 110
		if ((tmp__5__ == cnt * 4))
		{
#line 111
			*output << "hello" << std::endl; 
		}
#line 112
		else if ((tmp__5__ == cnt + 4)
#line 112

						 || (tmp__5__ == cnt - 4))
		{
#line 114
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
#line 120
	while (true)
	{
#line 121
		int32_t cnt = 2;
		if (!(cnt != 2)) break;
		cnt = 3;
	}
	while (true)
	{
#line 125
		if (!(cnt != 3)) break;
		int32_t cnt = 2;
		cnt = 3;
	}
	const std::string tmp__6__ = "abcd⅑⅛⅐efg";
	auto tmp__8__ = tmp__6__.cbegin();
	for (auto tmp__7__ = tmp__6__.cbegin(); tmp__7__ != tmp__6__.cend(); tmp__7__=tmp__8__)
	{
		uint32_t item = utf8::next(tmp__8__,tmp__6__.cend());
#line 130
		*output << (Core::toString(item)); *output << std::endl; 
	}
	for (int32_t lop= 1;lop<=10; lop+=1)
	{
#line 132
		if (!(lop != 20)) break;
		int32_t cnt = 2;
		*output << ((lop * cnt)); *output << std::endl; 
	}
	for (int32_t lop= 1;lop<=10; lop+=2)
	{
#line 136
		if (!(lop != 20)) break;
		int32_t cnt = 2;
		*output << ((lop * cnt)); *output << std::endl; 
	}
	for (int32_t lop= 10;lop >= 1; lop -= 1)
	{
#line 141
		int32_t cnt = 2;
		*output << ((lop * cnt)); *output << std::endl; 
	}
	for (int32_t lop= 10;lop >= 1; lop -= 2)
	{
#line 145
		int32_t cnt = 2;
		if (!(lop != 20)) break;
		*output << ((lop * cnt)); *output << std::endl; 
	}
	if (cnt == 3)
	{
#line 150
		*output << "Hello, world!" << std::endl; 
		cnt = 4;
	}
	if (cnt != 3)
	{
#line 154
		float x = static_cast<float>(3.14);
		*output << (x); *output << (x); *output << " " << (x); 
	}
#line 156
	else
	{
#line 157
		int32_t x = 3;
		*output << (x); *output << " " << ((x + 1)); *output << std::endl; 
	}
	if (cnt < 3)
	{
#line 161
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))); *output << std::endl; 
	}
#line 163
	else if (false)
	{
#line 164
		int32_t x = 3;
		*output << (x); *output << " " << ((x + 1)); *output << std::endl; 
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
	else if (true)
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
	else
	{
#line 184
		int32_t x = 3;
		*output << "Hello, world!" << std::endl; 
	}
	if (cnt < 3)
	{
#line 188
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))); *output << std::endl; 
	}
#line 190
	else if (false)
	{
#line 191
		int32_t x = 3;
		*output << (x); *output << " " << ((x + 1)); *output << std::endl; 
	}
#line 193
	else if (true)
	{
#line 194
		int32_t x = 3;
		*output << "Hello, world!" << std::endl; 
	}
#line 196
	else
	{
#line 197
		int32_t y = 3;
		*output << "Bye, world!" << std::endl; 
	}
}
