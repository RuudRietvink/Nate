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
#line 12 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\core\\Complex.ns"
#include "C:\Users\ruud\source\repos\Nate\Nate\core\Complex.h"
#include <complex>
#line 16
const double i = 1;
#line 6 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\core\\Rational.ns"
#include "C:\Users\ruud\source\repos\Nate\Nate\core\Rational.h"
#undef NOMINMAX
#define NOMINMAX
#include <windows.h>
#line 17 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\input\\nate.in2"
int main(int argc, char** argv)
{
	output = std::shared_ptr<std::ostream>(&std::cout, [](void*) {});
	error = std::shared_ptr<std::ostream>(&std::cerr, [](void*) {});
	input = std::shared_ptr<std::istream>(&std::cin, [](void*) {});
	SetConsoleOutputCP(65001);
#line 18
	string_t hello = " world";
	struct Rec
	{
		Rec()
			: fld1(3)
			, fld2()
			, fld3((Complex<double>(1, (2 * i))) - 3)
			, fld4(Rational(0,3,4))
		{}
#line 20
		int32_t fld1 = 3;
		string_t fld2 = {};
		Complex<double> fld3 = (Complex<double>(1, (2 * i))) - 3;
		Rational fld4 = Rational(0,3,4);
	};

#line 25
	Rec rec = {};
	*output << ((rec . fld2)) << std::endl;
#line 28
	std::ostringstream data1_temp;
#line 28
	data1_temp << 1 << " " << 2 << " sdsddsaaa " << (hello) << ((static_cast<int32_t>(hello.size()))) << std::endl << 3 << " " << 4 << 5 << " " << 6 << 7 << " " << 8 << std::endl << "hi " << ((4 * 5)) << std::endl;
	const string_t data1= data1_temp.str();
#line 35
	std::ostringstream data2_temp;
#line 35
	data2_temp << 1 << (data1) << std::endl;
	const string_t data2= data2_temp.str();
#line 38
	*error << "Hello " << (hello) << std::endl;
#line 40
	std::shared_ptr<std::istringstream> in = {};
	in.reset(new std::istringstream(data1));
#line 43
	int32_t x = {};
#line 43
	int32_t y = {};
	string_t s = {};
	std::shared_ptr<std::istream> nate__reader = in;
#line 45
	*nate__reader >> x;
	*nate__reader >> std::ws >> y;
	*nate__reader >> std::ws >> s;
	(*nate__reader).ignore(std::numeric_limits<std::streamsize>::max(), '\n');
#line 46
	*output << (x) << " " << (y) << " " << ((x * y)) << " " << ((e * x)) << std::endl;
	*output << "hi there";
	*output << (s) << std::endl;
#line 50
	std::shared_ptr<std::ofstream> out = {};
	std::shared_ptr<std::ostream> out2 = out;
	*output << "Before: " << std::boolalpha  << (((out && out->good()))) << " " << std::boolalpha  << (((out && out->is_open()))) << std::endl;
	out.reset(new std::ofstream("d:\\uit", std::ios::out));
	out2 = out;
	*output << std::endl;
	if ((out && out->is_open()))
	{
#line 57
		*output << "Inside: " << std::boolalpha  << (((out && out->good()))) << " " << std::boolalpha  << (((out && out->is_open()))) << std::endl;
		std::shared_ptr<std::ostream> nate__writer = out2;
#line 58
		*nate__writer << "Hello,planet ";
		*nate__writer << 12 << " " << 13 << 14;
		*nate__writer << 15 << std::endl;
		nate__writer = output;
#line 61
		*nate__writer << "Hi ";
		*output << "Hi ";
		*nate__writer << "End" << std::endl;
		*nate__writer << std::endl;
		nate__writer = out2;
#line 65
		*nate__writer << "Hi there" << std::endl;
		if (out) out->close();
	}
#line 67
	*output << "After: " << std::boolalpha  << (((out && out->good()))) << " " << std::boolalpha  << (((out && out->is_open()))) << std::endl;
#line 69
	int32_t cnt = 2;
	cnt = 3;
#line 73
	auto const tmp__0__ = cnt;
#line 73
	switch (tmp__0__)
	{
#line 73
	case 10:
	case 11:
	{
#line 75
		*output << "case" << std::endl;
	}
	break;
#line 76
	case 13:
	{
#line 77
		*output << "case 13" << std::endl;
	}
	break;
	}
#line 80
	auto const tmp__1__ = cnt;
#line 80
	switch (tmp__1__)
	{
#line 80
	case 10:
	case 11:
	{
#line 82
		*output << "case" << std::endl;
	}
	break;
#line 83
	case 13:
	{
#line 84
		*output << "case 13" << std::endl;
	}
	break;
	default:
	{
#line 86
		*output << "else" << std::endl;
	}
	break;
	}
#line 89
	auto const tmp__2__ = cnt;
#line 89
	if ((tmp__2__ == cnt * 4))
	{
#line 90
		*output << "hello" << std::endl;
	}
#line 91
	else if ((tmp__2__ == cnt + 4)
#line 91

					 || (tmp__2__ == cnt - 4))
	{
#line 93
		*output << "hi" << std::endl;
		cnt = 2;
	}
#line 97
	auto const tmp__3__ = cnt;
#line 97
	if ((tmp__3__ == cnt * 4))
	{
#line 98
		*output << "hello" << std::endl;
	}
#line 99
	else if ((tmp__3__ == cnt + 4)
#line 99

					 || (tmp__3__ == cnt - 4))
	{
#line 101
		*output << "hi" << std::endl;
		cnt = 2;
	}
	else
	{
#line 104
		*output << "else" << std::endl;
	}
#line 107
	auto const tmp__4__ = cnt;
#line 107
	switch (tmp__4__)
	{
#line 107
	case 10:
	case 11:
	{
#line 109
		*output << "case" << std::endl;
	}
	break;
	{
#line 111
		*output << "hello" << std::endl;
	}
	break;
	{
#line 114
		*output << "hi" << std::endl;
		cnt = 2;
	}
	break;
	}
#line 118
	auto const tmp__5__ = cnt;
#line 118
	switch (tmp__5__)
	{
#line 118
	case 10:
	case 11:
	{
#line 120
		*output << "case" << std::endl;
	}
	break;
	{
#line 122
		*output << "hello" << std::endl;
	}
	break;
	{
#line 125
		*output << "hi" << std::endl;
		cnt = 2;
	}
	break;
	default:
	{
#line 121
		if ((tmp__5__ == cnt * 4))
		{
#line 122
			*output << "hello" << std::endl;
		}
#line 123
		else if ((tmp__5__ == cnt + 4)
#line 123

						 || (tmp__5__ == cnt - 4))
		{
#line 125
			*output << "hi" << std::endl;
			cnt = 2;
		}
		else
		{
#line 128
			*output << "else" << std::endl;
		}
	}
	break;
	}
#line 131
	while (true)
	{
#line 132
		int32_t cnt = 2;
		if (!(cnt != 2)) break;
		cnt = 3;
	}
	while (true)
	{
#line 136
		if (!(cnt != 3)) break;
		int32_t cnt = 2;
		cnt = 3;
	}
	const std::string tmp__6__ = "abcd⅑⅛⅐efg";
	auto tmp__8__ = tmp__6__.cbegin();
	for (auto tmp__7__ = tmp__6__.cbegin(); tmp__7__ != tmp__6__.cend(); tmp__7__=tmp__8__)
	{
		uint32_t item = utf8::next(tmp__8__,tmp__6__.cend());
#line 141
		*output << (Core::toString(item)) << std::endl;
	}
	for (int32_t lop= 1;lop<=10; lop+=1)
	{
#line 143
		if (!(lop != 20)) break;
		int32_t cnt = 2;
		*output << ((lop * cnt)) << std::endl;
	}
	for (int32_t lop= 1;lop<=10; lop+=2)
	{
#line 147
		if (!(lop != 20)) break;
		int32_t cnt = 2;
		*output << ((lop * cnt)) << std::endl;
	}
	for (int32_t lop= 10;lop >= 1; lop -= 1)
	{
#line 152
		int32_t cnt = 2;
		*output << ((lop * cnt)) << std::endl;
	}
	for (int32_t lop= 10;lop >= 1; lop -= 2)
	{
#line 156
		int32_t cnt = 2;
		if (!(lop != 20)) break;
		*output << ((lop * cnt)) << std::endl;
	}
	if (cnt == 3)
	{
#line 161
		*output << "Hello, world!" << std::endl;
		cnt = 4;
	}
	if (cnt != 3)
	{
#line 165
		float x = static_cast<float>(3.14);
		*output << (x) << (x) << " " << (x);
	}
#line 167
	else
	{
#line 168
		int32_t x = 3;
		*output << (x) << " " << ((x + 1)) << std::endl;
	}
	if (cnt < 3)
	{
#line 172
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))) << std::endl;
	}
#line 174
	else if (false)
	{
#line 175
		int32_t x = 3;
		*output << (x) << " " << ((x + 1)) << std::endl;
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
	else if (true)
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
	else
	{
#line 195
		int32_t x = 3;
		*output << "Hello, world!" << std::endl;
	}
	if (cnt < 3)
	{
#line 199
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))) << std::endl;
	}
#line 201
	else if (false)
	{
#line 202
		int32_t x = 3;
		*output << (x) << " " << ((x + 1)) << std::endl;
	}
#line 204
	else if (true)
	{
#line 205
		int32_t x = 3;
		*output << "Hello, world!" << std::endl;
	}
#line 207
	else
	{
#line 208
		int32_t y = 3;
		*output << "Bye, world!" << std::endl;
	}
}
