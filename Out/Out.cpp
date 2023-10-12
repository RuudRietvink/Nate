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
	data1_temp << 1 << " " << 2 << " sdsddsaaa " << (hello) << ((static_cast<int32_t>(hello.size()))) << std::endl << 3 << " " << 4 << 5 << " " << 6 << 7 << " " << 8 << std::endl << "hi " << ((4 * 5)) << std::endl;
	const string_t data1= data1_temp.str();
#line 25
	std::ostringstream data2_temp;
#line 25
	data2_temp << 1 << (data1) << std::endl;
	const string_t data2= data2_temp.str();
#line 28
	*error << "Hello " << (hello) << std::endl;
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
	*output << (x) << " " << (y) << " ";
	*output << "hi there";
	*output << (s) << std::endl;
#line 40
	std::shared_ptr<std::ofstream> out = {};
	std::shared_ptr<std::ostream> out2 = out;
	*output << "Before: " << std::boolalpha  << (((out && out->good()))) << " " << std::boolalpha  << (((out && out->is_open()))) << std::endl;
	out.reset(new std::ofstream("d:\\uit", std::ios::out));
	out2 = out;
	*output << std::endl;
	if ((out && out->is_open()))
	{
#line 47
		*output << "Inside: " << std::boolalpha  << (((out && out->good()))) << " " << std::boolalpha  << (((out && out->is_open()))) << std::endl;
		std::shared_ptr<std::ostream> nate__writer = out2;
#line 48
		*nate__writer << "Hello,planet ";
		*nate__writer << 12 << " " << 13 << 14;
		*nate__writer << 15 << std::endl;
		nate__writer = output;
#line 51
		*nate__writer << "Hi ";
		*output << "Hi ";
		*nate__writer << "End" << std::endl;
		*nate__writer << std::endl;
		nate__writer = out2;
#line 55
		*nate__writer << "Hi there" << std::endl;
		if (out) out->close();
	}
#line 57
	*output << "After: " << std::boolalpha  << (((out && out->good()))) << " " << std::boolalpha  << (((out && out->is_open()))) << std::endl;
#line 59
	int32_t cnt = 2;
	cnt = 3;
#line 63
	auto const tmp__0__ = cnt;
#line 63
	switch (tmp__0__)
	{
#line 63
	case 10:
	case 11:
	{
#line 65
		*output << "case" << std::endl;
	}
	break;
#line 66
	case 13:
	{
#line 67
		*output << "case 13" << std::endl;
	}
	break;
	}
#line 70
	auto const tmp__1__ = cnt;
#line 70
	switch (tmp__1__)
	{
#line 70
	case 10:
	case 11:
	{
#line 72
		*output << "case" << std::endl;
	}
	break;
#line 73
	case 13:
	{
#line 74
		*output << "case 13" << std::endl;
	}
	break;
	default:
	{
#line 76
		*output << "else" << std::endl;
	}
	break;
	}
#line 79
	auto const tmp__2__ = cnt;
#line 79
	if ((tmp__2__ == cnt * 4))
	{
#line 80
		*output << "hello" << std::endl;
	}
#line 81
	else if ((tmp__2__ == cnt + 4)
#line 81

					 || (tmp__2__ == cnt - 4))
	{
#line 83
		*output << "hi" << std::endl;
		cnt = 2;
	}
#line 87
	auto const tmp__3__ = cnt;
#line 87
	if ((tmp__3__ == cnt * 4))
	{
#line 88
		*output << "hello" << std::endl;
	}
#line 89
	else if ((tmp__3__ == cnt + 4)
#line 89

					 || (tmp__3__ == cnt - 4))
	{
#line 91
		*output << "hi" << std::endl;
		cnt = 2;
	}
	else
	{
#line 94
		*output << "else" << std::endl;
	}
#line 97
	auto const tmp__4__ = cnt;
#line 97
	switch (tmp__4__)
	{
#line 97
	case 10:
	case 11:
	{
#line 99
		*output << "case" << std::endl;
	}
	break;
	{
#line 101
		*output << "hello" << std::endl;
	}
	break;
	{
#line 104
		*output << "hi" << std::endl;
		cnt = 2;
	}
	break;
	}
#line 108
	auto const tmp__5__ = cnt;
#line 108
	switch (tmp__5__)
	{
#line 108
	case 10:
	case 11:
	{
#line 110
		*output << "case" << std::endl;
	}
	break;
	{
#line 112
		*output << "hello" << std::endl;
	}
	break;
	{
#line 115
		*output << "hi" << std::endl;
		cnt = 2;
	}
	break;
	default:
	{
#line 111
		if ((tmp__5__ == cnt * 4))
		{
#line 112
			*output << "hello" << std::endl;
		}
#line 113
		else if ((tmp__5__ == cnt + 4)
#line 113

						 || (tmp__5__ == cnt - 4))
		{
#line 115
			*output << "hi" << std::endl;
			cnt = 2;
		}
		else
		{
#line 118
			*output << "else" << std::endl;
		}
	}
	break;
	}
#line 121
	while (true)
	{
#line 122
		int32_t cnt = 2;
		if (!(cnt != 2)) break;
		cnt = 3;
	}
	while (true)
	{
#line 126
		if (!(cnt != 3)) break;
		int32_t cnt = 2;
		cnt = 3;
	}
	const std::string tmp__6__ = "abcd⅑⅛⅐efg";
	auto tmp__8__ = tmp__6__.cbegin();
	for (auto tmp__7__ = tmp__6__.cbegin(); tmp__7__ != tmp__6__.cend(); tmp__7__=tmp__8__)
	{
		uint32_t item = utf8::next(tmp__8__,tmp__6__.cend());
#line 131
		*output << (Core::toString(item)) << std::endl;
	}
	for (int32_t lop= 1;lop<=10; lop+=1)
	{
#line 133
		if (!(lop != 20)) break;
		int32_t cnt = 2;
		*output << ((lop * cnt)) << std::endl;
	}
	for (int32_t lop= 1;lop<=10; lop+=2)
	{
#line 137
		if (!(lop != 20)) break;
		int32_t cnt = 2;
		*output << ((lop * cnt)) << std::endl;
	}
	for (int32_t lop= 10;lop >= 1; lop -= 1)
	{
#line 142
		int32_t cnt = 2;
		*output << ((lop * cnt)) << std::endl;
	}
	for (int32_t lop= 10;lop >= 1; lop -= 2)
	{
#line 146
		int32_t cnt = 2;
		if (!(lop != 20)) break;
		*output << ((lop * cnt)) << std::endl;
	}
	if (cnt == 3)
	{
#line 151
		*output << "Hello, world!" << std::endl;
		cnt = 4;
	}
	if (cnt != 3)
	{
#line 155
		float x = static_cast<float>(3.14);
		*output << (x) << (x) << " " << (x);
	}
#line 157
	else
	{
#line 158
		int32_t x = 3;
		*output << (x) << " " << ((x + 1)) << std::endl;
	}
	if (cnt < 3)
	{
#line 162
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))) << std::endl;
	}
#line 164
	else if (false)
	{
#line 165
		int32_t x = 3;
		*output << (x) << " " << ((x + 1)) << std::endl;
	}
	if (cnt < 3)
	{
#line 169
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))) << std::endl;
	}
#line 171
	else if (false)
	{
#line 172
		int32_t x = 3;
		*output << (x) << " " << ((x + 1)) << std::endl;
	}
#line 174
	else if (true)
	{
#line 175
		int32_t x = 3;
		*output << "Hello, world!" << std::endl;
	}
	if (cnt < 3)
	{
#line 179
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))) << std::endl;
	}
#line 181
	else if (false)
	{
#line 182
		int32_t x = 3;
		*output << (x) << " " << ((x + 1)) << std::endl;
	}
#line 184
	else
	{
#line 185
		int32_t x = 3;
		*output << "Hello, world!" << std::endl;
	}
	if (cnt < 3)
	{
#line 189
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))) << std::endl;
	}
#line 191
	else if (false)
	{
#line 192
		int32_t x = 3;
		*output << (x) << " " << ((x + 1)) << std::endl;
	}
#line 194
	else if (true)
	{
#line 195
		int32_t x = 3;
		*output << "Hello, world!" << std::endl;
	}
#line 197
	else
	{
#line 198
		int32_t y = 3;
		*output << "Bye, world!" << std::endl;
	}
}
