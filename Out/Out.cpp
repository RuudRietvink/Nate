#include "C:\Users\ruud\source\repos\Nate\Nate\core\Core.h"
#include "C:\Users\ruud\source\repos\Nate\NateCompiler\created\BaseObject.h"
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
class Hi: public BaseObject
{
public:
	virtual ~Hi();
	Hi();
private:
	class __impl;
	__impl* _impl;
	friend class __impl;
	public:
#line 18
	struct Datum
	{
		Datum()
			: fun()
			, count()
		{}
#line 18
		string_t fun;
#line 18
		int32_t count;
	};

#line 26
	std::shared_ptr<Hi> new_Hi2_();
	string_t E_me__E_text_(const string_t& world) const;
private:
	Datum datum = {};
public:
	virtual Hi::Datum datum_get() const;
	virtual const Hi::Datum& datum_set(const Hi::Datum& value);
private:
	int32_t prop_hi = {};
public:
	virtual int32_t prop_hi_get() const;
	virtual const int32_t prop_hi_set(const int32_t value);
private:
	int32_t prop_hi = {};
public:
	virtual int32_t prop_hi_get() const;
	virtual const int32_t prop_hi_set(const int32_t value);
private:
	int32_t age = {};
public:
	virtual int32_t age_get() const;
};

#line 30
class Hi::__impl
{
private:
	friend class Hi;
	Hi* me;
public:
	__impl(Hi* aMe) : me(aMe) {}
#line 47
	static std::shared_ptr<Hi> new_Hi2_()
	{
#line 48
		return std::make_shared<Hi>();
	}
	string_t E_me__E_text_(const string_t& world) const
	{
#line 51
		return ((((__impl::yo_()) + " ") + (std::to_string(prop_hi))) + " ") + world;
	}
};

Hi::Hi()
	: _impl(new __impl(this)) {}
Hi::~Hi() { delete _impl; }
#line 32
#line 36
#line 40
static string_t yo_();
{
#line 41
	return "Hi";
}
virtual string_t E_O_me__E_text_(const string_t& world);
{
#line 44
	prop_hi = (-3) + me->prop_hi;
	return "ls " + (__impl::yo_());
}
Hi::Datum Hi::datum_get() const { return datum; }
const Hi::Datum& Hi::datum_set(const Hi::Datum& value) { return datum = value; }
int32_t Hi::prop_hi_get() const { return prop_hi; }
const int32_t Hi::prop_hi_set(const int32_t value) { return prop_hi = value; }
int32_t Hi::prop_hi_get() const { return prop_hi; }
const int32_t Hi::prop_hi_set(const int32_t value) { return prop_hi = value; }
#line 55
bool why_so_E_text_(const string_t& serious)
{
#line 56
	*output << "why so " << (serious) << std::endl;
	return false;
}
#undef NOMINMAX
#define NOMINMAX
#include <windows.h>
#line 61
int main(int argc, char** argv)
{
	output = std::shared_ptr<std::ostream>(&std::cout, [](void*) {});
	error = std::shared_ptr<std::ostream>(&std::cerr, [](void*) {});
	input = std::shared_ptr<std::istream>(&std::cin, [](void*) {});
	SetConsoleOutputCP(65001);
#line 62
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
#line 64
		int32_t fld1;
#line 64
		string_t fld2;
#line 64
		Complex<double> fld3;
#line 64
		Rational fld4;
	};

#line 70
	Rec rec = {};
	*output << ((rec . fld1)) << " " << ((rec . fld4)) << std::endl;
#line 73
	std::ostringstream data1_temp;
#line 73
	data1_temp << 1 << " " << 2 << " sdsddsaaa " << (hello) << ((static_cast<int32_t>(hello.size()))) << std::endl << 3 << " " << 4 << 5 << " " << 6 << 7 << " " << 8 << std::endl << "hi " << ((4 * 5)) << std::endl;
	const string_t data1= data1_temp.str();
#line 80
	std::ostringstream data2_temp;
#line 80
	data2_temp << 1 << (data1) << std::endl;
	const string_t data2= data2_temp.str();
#line 83
	*error << "Hello " << (hello) << std::endl;
#line 85
	std::shared_ptr<std::istringstream> in = {};
	in.reset(new std::istringstream(data1));
#line 88
	int32_t x = {};
#line 88
	int32_t y = {};
	string_t s = {};
	std::shared_ptr<std::istream> nate__reader = in;
#line 90
	*nate__reader >> x;
	*nate__reader >> std::ws >> y;
	*nate__reader >> std::ws >> s;
	(*nate__reader).ignore(std::numeric_limits<std::streamsize>::max(), '\n');
#line 91
	*output << (x) << " " << (y) << " " << ((x * y)) << " " << ((e * x)) << std::endl;
	*output << "hi there";
	*output << (s) << std::endl;
#line 95
	std::shared_ptr<std::ofstream> out = {};
	std::shared_ptr<std::ostream> out2 = out;
	*output << "Before: " << std::boolalpha  << (((out && out->good()))) << " " << std::boolalpha  << (((out && out->is_open()))) << std::endl;
	out.reset(new std::ofstream("d:\\uit", std::ios::out));
	out2 = out;
	*output << std::endl;
	if ((out && out->is_open()))
	{
#line 102
		*output << "Inside: " << std::boolalpha  << (((out && out->good()))) << " " << std::boolalpha  << (((out && out->is_open()))) << std::endl;
		std::shared_ptr<std::ostream> nate__writer = out2;
#line 103
		*nate__writer << "Hello,planet ";
		*nate__writer << 12 << " " << 13 << 14;
		*nate__writer << 15 << std::endl;
		nate__writer = output;
#line 106
		*nate__writer << "Hi ";
		*output << "Hi ";
		*nate__writer << "End" << std::endl;
		*nate__writer << std::endl;
		nate__writer = out2;
#line 110
		*nate__writer << "Hi there" << std::endl;
		if (out) out->close();
	}
#line 112
	*output << "After: " << std::boolalpha  << (((out && out->good()))) << " " << std::boolalpha  << (((out && out->is_open()))) << std::endl;
#line 114
	int32_t cnt = 2;
	cnt = 3;
#line 118
	auto const tmp__0__ = cnt;
#line 118
	switch (tmp__0__)
	{
#line 118
	case 10:
	case 11:
	{
#line 120
		*output << "case" << std::endl;
	}
	break;
#line 121
	case 13:
	{
#line 122
		*output << "case 13" << std::endl;
	}
	break;
	}
#line 125
	auto const tmp__1__ = cnt + 1;
#line 125
	switch (tmp__1__)
	{
#line 125
	case 10:
	case 11:
	{
#line 127
		*output << "case" << std::endl;
	}
	break;
#line 128
	case 13:
	{
#line 129
		*output << "case 13" << std::endl;
	}
	break;
	default:
	{
#line 131
		*output << "else" << std::endl;
	}
	break;
	}
#line 134
	auto const tmp__2__ = cnt + 2;
#line 134
	if ((tmp__2__ == cnt * 4))
	{
#line 135
		*output << "hello" << std::endl;
	}
#line 136
	else if ((tmp__2__ == cnt + 4)
#line 136

					 || (tmp__2__ == cnt - 4))
	{
#line 138
		*output << "hi" << std::endl;
		cnt = 2;
	}
#line 142
	auto const tmp__3__ = cnt + 3;
#line 142
	if ((tmp__3__ == cnt * 4))
	{
#line 143
		*output << "hello" << std::endl;
	}
#line 144
	else if ((tmp__3__ == cnt + 4)
#line 144

					 || (tmp__3__ == cnt - 4))
	{
#line 146
		*output << "hi" << std::endl;
		cnt = 2;
	}
	else
	{
#line 149
		*output << "else" << std::endl;
	}
#line 152
	auto const tmp__4__ = cnt + 4;
#line 152
	switch (tmp__4__)
	{
#line 152
	case 10:
	case 11:
	{
#line 154
		*output << "case" << std::endl;
	}
	break;
	{
#line 156
		*output << "hello" << std::endl;
	}
	break;
	{
#line 159
		*output << "hi" << std::endl;
		cnt = 2;
	}
	break;
	}
#line 163
	auto const tmp__5__ = cnt + 5;
#line 163
	switch (tmp__5__)
	{
#line 163
	case 10:
	case 11:
	{
#line 165
		*output << "case" << std::endl;
	}
	break;
	{
#line 167
		*output << "hello" << std::endl;
	}
	break;
	{
#line 170
		*output << "hi" << std::endl;
		cnt = 2;
	}
	break;
	default:
	{
#line 166
		if ((tmp__5__ == cnt * 4))
		{
#line 167
			*output << "hello" << std::endl;
		}
#line 168
		else if ((tmp__5__ == cnt + 4)
#line 168

						 || (tmp__5__ == cnt - 4))
		{
#line 170
			*output << "hi" << std::endl;
			cnt = 2;
		}
		else
		{
#line 173
			*output << "else" << std::endl;
		}
	}
	break;
	}
#line 176
	while (true)
	{
#line 177
		int32_t cnt = 2;
		if (!(cnt != 2)) break;
		cnt = 3;
	}
	while (true)
	{
#line 181
		if (!(cnt != 3)) break;
		int32_t cnt = 2;
		cnt = 3;
	}
	const std::string tmp__6__ = "abcd⅑⅛⅐efg";
	auto tmp__8__ = tmp__6__.cbegin();
	for (auto tmp__7__ = tmp__6__.cbegin(); tmp__7__ != tmp__6__.cend(); tmp__7__=tmp__8__)
	{
		uint32_t item = utf8::next(tmp__8__,tmp__6__.cend());
#line 186
		*output << (Core::toString(item)) << std::endl;
	}
	for (int32_t lop= 1;lop<=10; lop+=1)
	{
#line 188
		if (!(lop != 20)) break;
		int32_t cnt = 2;
		*output << ((lop * cnt)) << std::endl;
	}
	for (int32_t lop= 1;lop<=10; lop+=2)
	{
#line 192
		if (!(lop != 20)) break;
		int32_t cnt = 2;
		*output << ((lop * cnt)) << std::endl;
	}
	for (int32_t lop= 10;lop >= 1; lop -= 1)
	{
#line 197
		int32_t cnt = 2;
		*output << ((lop * cnt)) << std::endl;
	}
	for (int32_t lop= 10;lop >= 1; lop -= 2)
	{
#line 201
		int32_t cnt = 2;
		if (!(lop != 20)) break;
		*output << ((lop * cnt)) << std::endl;
	}
	if (cnt == 3)
	{
#line 206
		*output << "Hello, world!" << std::endl;
		cnt = 4;
	}
	if (cnt != 3)
	{
#line 210
		float x = static_cast<float>(3.14);
		*output << (x) << (x) << " " << (x);
	}
#line 212
	else
	{
#line 213
		int32_t x = 3;
		*output << (x) << " " << ((x + 1)) << std::endl;
	}
	if (cnt < 3)
	{
#line 217
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))) << std::endl;
	}
#line 219
	else if (false)
	{
#line 220
		int32_t x = 3;
		*output << (x) << " " << ((x + 1)) << std::endl;
	}
	if (cnt < 3)
	{
#line 224
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))) << std::endl;
	}
#line 226
	else if (false)
	{
#line 227
		int32_t x = 3;
		*output << (x) << " " << ((x + 1)) << std::endl;
	}
#line 229
	else if (true)
	{
#line 230
		int32_t x = 3;
		*output << "Hello, world!" << std::endl;
	}
	if (cnt < 3)
	{
#line 234
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))) << std::endl;
	}
#line 236
	else if (false)
	{
#line 237
		int32_t x = 3;
		*output << (x) << " " << ((x + 1)) << std::endl;
	}
#line 239
	else
	{
#line 240
		int32_t x = 3;
		*output << "Hello, world!" << std::endl;
	}
	if (cnt < 3)
	{
#line 244
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))) << std::endl;
	}
#line 246
	else if (false)
	{
#line 247
		int32_t x = 3;
		*output << (x) << " " << ((x + 1)) << std::endl;
	}
#line 249
	else if (true)
	{
#line 250
		int32_t x = 3;
		*output << "Hello, world!" << std::endl;
	}
#line 252
	else
	{
#line 253
		int32_t y = 3;
		*output << "Bye, world!" << std::endl;
	}
}
