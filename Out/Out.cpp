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
void why_so_E_text_(const string_t& serious)
{
#line 18
	*output << "why so " << (serious) << std::endl;
}
#undef NOMINMAX
#define NOMINMAX
#include <windows.h>
#line 20
int main(int argc, char** argv)
{
	output = std::shared_ptr<std::ostream>(&std::cout, [](void*) {});
	error = std::shared_ptr<std::ostream>(&std::cerr, [](void*) {});
	input = std::shared_ptr<std::istream>(&std::cin, [](void*) {});
	SetConsoleOutputCP(65001);
#line 21
	string_t hello = " world";
	why_so_E_text_("serious");
	struct Rec
	{
		Rec()
			: fld1(3)
			, fld2()
			, fld3(Complex<double>(1, (2 * i)))
			, fld4(Rational(0,11,152) + Rational(0,1,57))
		{}
#line 23
		int32_t fld1;
#line 23
		string_t fld2;
#line 23
		Complex<double> fld3;
#line 23
		Rational fld4;
	};

#line 29
	Rec rec = {};
	*output << ((rec . fld1)) << " " << ((rec . fld4)) << std::endl;
#line 32
	std::ostringstream data1_temp;
#line 32
	data1_temp << 1 << " " << 2 << " sdsddsaaa " << (hello) << ((static_cast<int32_t>(hello.size()))) << std::endl << 3 << " " << 4 << 5 << " " << 6 << 7 << " " << 8 << std::endl << "hi " << ((4 * 5)) << std::endl;
	const string_t data1= data1_temp.str();
#line 39
	std::ostringstream data2_temp;
#line 39
	data2_temp << 1 << (data1) << std::endl;
	const string_t data2= data2_temp.str();
#line 42
	*error << "Hello " << (hello) << std::endl;
#line 44
	std::shared_ptr<std::istringstream> in = {};
	in.reset(new std::istringstream(data1));
#line 47
	int32_t x = {};
#line 47
	int32_t y = {};
	string_t s = {};
	std::shared_ptr<std::istream> nate__reader = in;
#line 49
	*nate__reader >> x;
	*nate__reader >> std::ws >> y;
	*nate__reader >> std::ws >> s;
	(*nate__reader).ignore(std::numeric_limits<std::streamsize>::max(), '\n');
#line 50
	*output << (x) << " " << (y) << " " << ((x * y)) << " " << ((e * x)) << std::endl;
	*output << "hi there";
	*output << (s) << std::endl;
#line 54
	std::shared_ptr<std::ofstream> out = {};
	std::shared_ptr<std::ostream> out2 = out;
	*output << "Before: " << std::boolalpha  << (((out && out->good()))) << " " << std::boolalpha  << (((out && out->is_open()))) << std::endl;
	out.reset(new std::ofstream("d:\\uit", std::ios::out));
	out2 = out;
	*output << std::endl;
	if ((out && out->is_open()))
	{
#line 61
		*output << "Inside: " << std::boolalpha  << (((out && out->good()))) << " " << std::boolalpha  << (((out && out->is_open()))) << std::endl;
		std::shared_ptr<std::ostream> nate__writer = out2;
#line 62
		*nate__writer << "Hello,planet ";
		*nate__writer << 12 << " " << 13 << 14;
		*nate__writer << 15 << std::endl;
		nate__writer = output;
#line 65
		*nate__writer << "Hi ";
		*output << "Hi ";
		*nate__writer << "End" << std::endl;
		*nate__writer << std::endl;
		nate__writer = out2;
#line 69
		*nate__writer << "Hi there" << std::endl;
		if (out) out->close();
	}
#line 71
	*output << "After: " << std::boolalpha  << (((out && out->good()))) << " " << std::boolalpha  << (((out && out->is_open()))) << std::endl;
#line 73
	int32_t cnt = 2;
	cnt = 3;
#line 77
	auto const tmp__0__ = cnt;
#line 77
	switch (tmp__0__)
	{
#line 77
	case 10:
	case 11:
	{
#line 79
		*output << "case" << std::endl;
	}
	break;
#line 80
	case 13:
	{
#line 81
		*output << "case 13" << std::endl;
	}
	break;
	}
#line 84
	auto const tmp__1__ = cnt;
#line 84
	switch (tmp__1__)
	{
#line 84
	case 10:
	case 11:
	{
#line 86
		*output << "case" << std::endl;
	}
	break;
#line 87
	case 13:
	{
#line 88
		*output << "case 13" << std::endl;
	}
	break;
	default:
	{
#line 90
		*output << "else" << std::endl;
	}
	break;
	}
#line 93
	auto const tmp__2__ = cnt;
#line 93
	if ((tmp__2__ == cnt * 4))
	{
#line 94
		*output << "hello" << std::endl;
	}
#line 95
	else if ((tmp__2__ == cnt + 4)
#line 95

					 || (tmp__2__ == cnt - 4))
	{
#line 97
		*output << "hi" << std::endl;
		cnt = 2;
	}
#line 101
	auto const tmp__3__ = cnt;
#line 101
	if ((tmp__3__ == cnt * 4))
	{
#line 102
		*output << "hello" << std::endl;
	}
#line 103
	else if ((tmp__3__ == cnt + 4)
#line 103

					 || (tmp__3__ == cnt - 4))
	{
#line 105
		*output << "hi" << std::endl;
		cnt = 2;
	}
	else
	{
#line 108
		*output << "else" << std::endl;
	}
#line 111
	auto const tmp__4__ = cnt;
#line 111
	switch (tmp__4__)
	{
#line 111
	case 10:
	case 11:
	{
#line 113
		*output << "case" << std::endl;
	}
	break;
	{
#line 115
		*output << "hello" << std::endl;
	}
	break;
	{
#line 118
		*output << "hi" << std::endl;
		cnt = 2;
	}
	break;
	}
#line 122
	auto const tmp__5__ = cnt;
#line 122
	switch (tmp__5__)
	{
#line 122
	case 10:
	case 11:
	{
#line 124
		*output << "case" << std::endl;
	}
	break;
	{
#line 126
		*output << "hello" << std::endl;
	}
	break;
	{
#line 129
		*output << "hi" << std::endl;
		cnt = 2;
	}
	break;
	default:
	{
#line 125
		if ((tmp__5__ == cnt * 4))
		{
#line 126
			*output << "hello" << std::endl;
		}
#line 127
		else if ((tmp__5__ == cnt + 4)
#line 127

						 || (tmp__5__ == cnt - 4))
		{
#line 129
			*output << "hi" << std::endl;
			cnt = 2;
		}
		else
		{
#line 132
			*output << "else" << std::endl;
		}
	}
	break;
	}
#line 135
	while (true)
	{
#line 136
		int32_t cnt = 2;
		if (!(cnt != 2)) break;
		cnt = 3;
	}
	while (true)
	{
#line 140
		if (!(cnt != 3)) break;
		int32_t cnt = 2;
		cnt = 3;
	}
	const std::string tmp__6__ = "abcd⅑⅛⅐efg";
	auto tmp__8__ = tmp__6__.cbegin();
	for (auto tmp__7__ = tmp__6__.cbegin(); tmp__7__ != tmp__6__.cend(); tmp__7__=tmp__8__)
	{
		uint32_t item = utf8::next(tmp__8__,tmp__6__.cend());
#line 145
		*output << (Core::toString(item)) << std::endl;
	}
	for (int32_t lop= 1;lop<=10; lop+=1)
	{
#line 147
		if (!(lop != 20)) break;
		int32_t cnt = 2;
		*output << ((lop * cnt)) << std::endl;
	}
	for (int32_t lop= 1;lop<=10; lop+=2)
	{
#line 151
		if (!(lop != 20)) break;
		int32_t cnt = 2;
		*output << ((lop * cnt)) << std::endl;
	}
	for (int32_t lop= 10;lop >= 1; lop -= 1)
	{
#line 156
		int32_t cnt = 2;
		*output << ((lop * cnt)) << std::endl;
	}
	for (int32_t lop= 10;lop >= 1; lop -= 2)
	{
#line 160
		int32_t cnt = 2;
		if (!(lop != 20)) break;
		*output << ((lop * cnt)) << std::endl;
	}
	if (cnt == 3)
	{
#line 165
		*output << "Hello, world!" << std::endl;
		cnt = 4;
	}
	if (cnt != 3)
	{
#line 169
		float x = static_cast<float>(3.14);
		*output << (x) << (x) << " " << (x);
	}
#line 171
	else
	{
#line 172
		int32_t x = 3;
		*output << (x) << " " << ((x + 1)) << std::endl;
	}
	if (cnt < 3)
	{
#line 176
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))) << std::endl;
	}
#line 178
	else if (false)
	{
#line 179
		int32_t x = 3;
		*output << (x) << " " << ((x + 1)) << std::endl;
	}
	if (cnt < 3)
	{
#line 183
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))) << std::endl;
	}
#line 185
	else if (false)
	{
#line 186
		int32_t x = 3;
		*output << (x) << " " << ((x + 1)) << std::endl;
	}
#line 188
	else if (true)
	{
#line 189
		int32_t x = 3;
		*output << "Hello, world!" << std::endl;
	}
	if (cnt < 3)
	{
#line 193
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))) << std::endl;
	}
#line 195
	else if (false)
	{
#line 196
		int32_t x = 3;
		*output << (x) << " " << ((x + 1)) << std::endl;
	}
#line 198
	else
	{
#line 199
		int32_t x = 3;
		*output << "Hello, world!" << std::endl;
	}
	if (cnt < 3)
	{
#line 203
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))) << std::endl;
	}
#line 205
	else if (false)
	{
#line 206
		int32_t x = 3;
		*output << (x) << " " << ((x + 1)) << std::endl;
	}
#line 208
	else if (true)
	{
#line 209
		int32_t x = 3;
		*output << "Hello, world!" << std::endl;
	}
#line 211
	else
	{
#line 212
		int32_t y = 3;
		*output << "Bye, world!" << std::endl;
	}
}
