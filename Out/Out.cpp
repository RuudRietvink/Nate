#include "C:\Users\ruud\source\repos\Nate\Nate\core\Core.h"
#line 1 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\input\\nate.in2"
std::shared_ptr<std::ostream> output = {};
#line 1
std::shared_ptr<std::ostream> error = {};
#line 1
std::shared_ptr<std::istream> input = {};
#line 27 ""
#define NOMINMAX
#include <windows.h>
#include <cmath>
#line 32
const double pi = 3.14159265358979323;
#line 34
const double tau = 6.283185307179586;
#line 36
const double e = 2.718281828459045;
#line 8
#include <fstream>
#line 8
#include <sstream>
#line 12
#include "C:\Users\ruud\source\repos\Nate\Nate\core\Complex.h"
#include <complex>
#line 16
const double i = 1;
#line 6
#include "C:\Users\ruud\source\repos\Nate\Nate\core\Rational.h"
#line 17
bool why_so_E_text_(const string_t& serious)
{
#line 18
	*output << "why so " << (serious) << std::endl;
	return false;
}
#undef NOMINMAX
#define NOMINMAX
#include <windows.h>
#line 21
int main(int argc, char** argv)
{
	output = std::shared_ptr<std::ostream>(&std::cout, [](void*) {});
	error = std::shared_ptr<std::ostream>(&std::cerr, [](void*) {});
	input = std::shared_ptr<std::istream>(&std::cin, [](void*) {});
	SetConsoleOutputCP(65001);
#line 22
	string_t hello = " world";
	*output << std::boolalpha  << ((why_so_E_text_("serious"))) << std::endl;
	struct Rec
	{
		Rec()
			: fld1(3)
			, fld2()
			, fld3(Complex<double>(1, (2 * i)))
			, fld4(Rational(0,11,152) + Rational(0,1,57))
		{}
#line 24
		int32_t fld1;
#line 24
		string_t fld2;
#line 24
		Complex<double> fld3;
#line 24
		Rational fld4;
	};

#line 30
	Rec rec = {};
	*output << ((rec . fld1)) << " " << ((rec . fld4)) << std::endl;
#line 33
	std::ostringstream data1_temp;
#line 33
	data1_temp << 1 << " " << 2 << " sdsddsaaa " << (hello) << ((static_cast<int32_t>(hello.size()))) << std::endl << 3 << " " << 4 << 5 << " " << 6 << 7 << " " << 8 << std::endl << "hi " << ((4 * 5)) << std::endl;
	const string_t data1= data1_temp.str();
#line 40
	std::ostringstream data2_temp;
#line 40
	data2_temp << 1 << (data1) << std::endl;
	const string_t data2= data2_temp.str();
#line 43
	*error << "Hello " << (hello) << std::endl;
#line 45
	std::shared_ptr<std::istringstream> in = {};
	in.reset(new std::istringstream(data1));
#line 48
	int32_t x = {};
#line 48
	int32_t y = {};
	string_t s = {};
	std::shared_ptr<std::istream> nate__reader = in;
#line 50
	*nate__reader >> x;
	*nate__reader >> std::ws >> y;
	*nate__reader >> std::ws >> s;
	(*nate__reader).ignore(std::numeric_limits<std::streamsize>::max(), '\n');
#line 51
	*output << (x) << " " << (y) << " " << ((x * y)) << " " << ((e * x)) << std::endl;
	*output << "hi there";
	*output << (s) << std::endl;
#line 55
	std::shared_ptr<std::ofstream> out = {};
	std::shared_ptr<std::ostream> out2 = out;
	*output << "Before: " << std::boolalpha  << (((out && out->good()))) << " " << std::boolalpha  << (((out && out->is_open()))) << std::endl;
	out.reset(new std::ofstream("d:\\uit", std::ios::out));
	out2 = out;
	*output << std::endl;
	if ((out && out->is_open()))
	{
#line 62
		*output << "Inside: " << std::boolalpha  << (((out && out->good()))) << " " << std::boolalpha  << (((out && out->is_open()))) << std::endl;
		std::shared_ptr<std::ostream> nate__writer = out2;
#line 63
		*nate__writer << "Hello,planet ";
		*nate__writer << 12 << " " << 13 << 14;
		*nate__writer << 15 << std::endl;
		nate__writer = output;
#line 66
		*nate__writer << "Hi ";
		*output << "Hi ";
		*nate__writer << "End" << std::endl;
		*nate__writer << std::endl;
		nate__writer = out2;
#line 70
		*nate__writer << "Hi there" << std::endl;
		if (out) out->close();
	}
#line 72
	*output << "After: " << std::boolalpha  << (((out && out->good()))) << " " << std::boolalpha  << (((out && out->is_open()))) << std::endl;
#line 74
	int32_t cnt = 2;
	cnt = 3;
#line 78
	auto const tmp__0__ = cnt;
#line 78
	switch (tmp__0__)
	{
#line 78
	case 10:
	case 11:
	{
#line 80
		*output << "case" << std::endl;
	}
	break;
#line 81
	case 13:
	{
#line 82
		*output << "case 13" << std::endl;
	}
	break;
	}
#line 85
	auto const tmp__1__ = cnt;
#line 85
	switch (tmp__1__)
	{
#line 85
	case 10:
	case 11:
	{
#line 87
		*output << "case" << std::endl;
	}
	break;
#line 88
	case 13:
	{
#line 89
		*output << "case 13" << std::endl;
	}
	break;
	default:
	{
#line 91
		*output << "else" << std::endl;
	}
	break;
	}
#line 94
	auto const tmp__2__ = cnt;
#line 94
	if ((tmp__2__ == cnt * 4))
	{
#line 95
		*output << "hello" << std::endl;
	}
#line 96
	else if ((tmp__2__ == cnt + 4)
#line 96

					 || (tmp__2__ == cnt - 4))
	{
#line 98
		*output << "hi" << std::endl;
		cnt = 2;
	}
#line 102
	auto const tmp__3__ = cnt;
#line 102
	if ((tmp__3__ == cnt * 4))
	{
#line 103
		*output << "hello" << std::endl;
	}
#line 104
	else if ((tmp__3__ == cnt + 4)
#line 104

					 || (tmp__3__ == cnt - 4))
	{
#line 106
		*output << "hi" << std::endl;
		cnt = 2;
	}
	else
	{
#line 109
		*output << "else" << std::endl;
	}
#line 112
	auto const tmp__4__ = cnt;
#line 112
	switch (tmp__4__)
	{
#line 112
	case 10:
	case 11:
	{
#line 114
		*output << "case" << std::endl;
	}
	break;
	{
#line 116
		*output << "hello" << std::endl;
	}
	break;
	{
#line 119
		*output << "hi" << std::endl;
		cnt = 2;
	}
	break;
	}
#line 123
	auto const tmp__5__ = cnt;
#line 123
	switch (tmp__5__)
	{
#line 123
	case 10:
	case 11:
	{
#line 125
		*output << "case" << std::endl;
	}
	break;
	{
#line 127
		*output << "hello" << std::endl;
	}
	break;
	{
#line 130
		*output << "hi" << std::endl;
		cnt = 2;
	}
	break;
	default:
	{
#line 126
		if ((tmp__5__ == cnt * 4))
		{
#line 127
			*output << "hello" << std::endl;
		}
#line 128
		else if ((tmp__5__ == cnt + 4)
#line 128

						 || (tmp__5__ == cnt - 4))
		{
#line 130
			*output << "hi" << std::endl;
			cnt = 2;
		}
		else
		{
#line 133
			*output << "else" << std::endl;
		}
	}
	break;
	}
#line 136
	while (true)
	{
#line 137
		int32_t cnt = 2;
		if (!(cnt != 2)) break;
		cnt = 3;
	}
	while (true)
	{
#line 141
		if (!(cnt != 3)) break;
		int32_t cnt = 2;
		cnt = 3;
	}
	const std::string tmp__6__ = "abcd⅑⅛⅐efg";
	auto tmp__8__ = tmp__6__.cbegin();
	for (auto tmp__7__ = tmp__6__.cbegin(); tmp__7__ != tmp__6__.cend(); tmp__7__=tmp__8__)
	{
		uint32_t item = utf8::next(tmp__8__,tmp__6__.cend());
#line 146
		*output << (Core::toString(item)) << std::endl;
	}
	for (int32_t lop= 1;lop<=10; lop+=1)
	{
#line 148
		if (!(lop != 20)) break;
		int32_t cnt = 2;
		*output << ((lop * cnt)) << std::endl;
	}
	for (int32_t lop= 1;lop<=10; lop+=2)
	{
#line 152
		if (!(lop != 20)) break;
		int32_t cnt = 2;
		*output << ((lop * cnt)) << std::endl;
	}
	for (int32_t lop= 10;lop >= 1; lop -= 1)
	{
#line 157
		int32_t cnt = 2;
		*output << ((lop * cnt)) << std::endl;
	}
	for (int32_t lop= 10;lop >= 1; lop -= 2)
	{
#line 161
		int32_t cnt = 2;
		if (!(lop != 20)) break;
		*output << ((lop * cnt)) << std::endl;
	}
	if (cnt == 3)
	{
#line 166
		*output << "Hello, world!" << std::endl;
		cnt = 4;
	}
	if (cnt != 3)
	{
#line 170
		float x = static_cast<float>(3.14);
		*output << (x) << (x) << " " << (x);
	}
#line 172
	else
	{
#line 173
		int32_t x = 3;
		*output << (x) << " " << ((x + 1)) << std::endl;
	}
	if (cnt < 3)
	{
#line 177
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))) << std::endl;
	}
#line 179
	else if (false)
	{
#line 180
		int32_t x = 3;
		*output << (x) << " " << ((x + 1)) << std::endl;
	}
	if (cnt < 3)
	{
#line 184
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))) << std::endl;
	}
#line 186
	else if (false)
	{
#line 187
		int32_t x = 3;
		*output << (x) << " " << ((x + 1)) << std::endl;
	}
#line 189
	else if (true)
	{
#line 190
		int32_t x = 3;
		*output << "Hello, world!" << std::endl;
	}
	if (cnt < 3)
	{
#line 194
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))) << std::endl;
	}
#line 196
	else if (false)
	{
#line 197
		int32_t x = 3;
		*output << (x) << " " << ((x + 1)) << std::endl;
	}
#line 199
	else
	{
#line 200
		int32_t x = 3;
		*output << "Hello, world!" << std::endl;
	}
	if (cnt < 3)
	{
#line 204
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))) << std::endl;
	}
#line 206
	else if (false)
	{
#line 207
		int32_t x = 3;
		*output << (x) << " " << ((x + 1)) << std::endl;
	}
#line 209
	else if (true)
	{
#line 210
		int32_t x = 3;
		*output << "Hello, world!" << std::endl;
	}
#line 212
	else
	{
#line 213
		int32_t y = 3;
		*output << "Bye, world!" << std::endl;
	}
}
