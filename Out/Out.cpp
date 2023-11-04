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
	static std::shared_ptr<Hi> new_Hi2_();
	virtual string_t E_me__E_text_(const string_t& world) const;
private:
	int32_t age = {};
public:
	virtual int32_t age_get() const;
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
};

#line 30
class Hi::__impl
{
private:
	friend class Hi;
	Hi* me;
#line 31
	string_t local = {};
public:
	__impl(Hi* aMe) : me(aMe) {}
#line 42
	string_t yoh_E_me__() const
	{
#line 43
		return "Hi " + local;
	}
	static string_t yo_()
	{
#line 46
		return "Hi";
	}
	string_t E_O_me__E_text_(const string_t& world)
	{
#line 49
		int32_t loc = (-3) + me->prop_hi;
		return ("ls " + (__impl::yo_())) + local;
	}
};

Hi::Hi()
	: _impl(new __impl(this)) {}
Hi::~Hi() { delete _impl; }
#line 33
int32_t Hi::age_get() const
{
	return age;
}
const int32_t Hi::prop_hi_set(const int32_t value)
{
	prop_hi = value;
	return prop_hi;
}
#line 52
std::shared_ptr<Hi> Hi::new_Hi2_()
{
#line 53
	return std::make_shared<Hi>();
}
string_t Hi::E_me__E_text_(const string_t& world) const
{
#line 56
	return (((((__impl::yo_()) + " ") + (std::to_string(prop_hi))) + " ") + world) + _impl->local;
}
Hi::Datum Hi::datum_get() const { return datum; }
const Hi::Datum& Hi::datum_set(const Hi::Datum& value) { return datum = value; }
int32_t Hi::prop_hi_get() const { return prop_hi; }
#line 60
bool why_so_E_text_(const string_t& serious)
{
#line 61
	*output << "why so " << (serious) << std::endl;
	return false;
}
#undef NOMINMAX
#define NOMINMAX
#include <windows.h>
#line 66
int main(int argc, char** argv)
{
	output = std::shared_ptr<std::ostream>(&std::cout, [](void*) {});
	error = std::shared_ptr<std::ostream>(&std::cerr, [](void*) {});
	input = std::shared_ptr<std::istream>(&std::cin, [](void*) {});
	SetConsoleOutputCP(65001);
#line 67
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
#line 69
		int32_t fld1;
#line 69
		string_t fld2;
#line 69
		Complex<double> fld3;
#line 69
		Rational fld4;
	};

#line 75
	Rec rec = {};
	*output << ((rec . fld1)) << " " << ((rec . fld4)) << std::endl;
#line 78
	std::ostringstream data1_temp;
#line 78
	data1_temp << 1 << " " << 2 << " sdsddsaaa " << (hello) << ((static_cast<int32_t>(hello.size()))) << std::endl << 3 << " " << 4 << 5 << " " << 6 << 7 << " " << 8 << std::endl << "hi " << ((4 * 5)) << std::endl;
	const string_t data1= data1_temp.str();
#line 85
	std::ostringstream data2_temp;
#line 85
	data2_temp << 1 << (data1) << std::endl;
	const string_t data2= data2_temp.str();
#line 88
	*error << "Hello " << (hello) << std::endl;
#line 91
	{
		std::shared_ptr<std::istringstream> in = {};
		in.reset(new std::istringstream(data1));
		int32_t x = {};
#line 94
		int32_t y = {};
		string_t s = {};
		std::shared_ptr<std::istream> nate__reader = in;
#line 96
		*nate__reader >> x;
		*nate__reader >> std::ws >> y;
		*nate__reader >> std::ws >> s;
		(*nate__reader).ignore(std::numeric_limits<std::streamsize>::max(), '\n');
#line 97
		*output << (x) << " " << (y) << " " << ((x * y)) << " " << ((e * x)) << std::endl;
		*output << "hi there";
		*output << (s) << std::endl;
	}
	{
		std::shared_ptr<std::ofstream> out = {};
		std::shared_ptr<std::ostream> out2 = out;
		*output << "Before: " << std::boolalpha  << (((out && out->good()))) << " " << std::boolalpha  << (((out && out->is_open()))) << std::endl;
		out.reset(new std::ofstream("d:\\uit", std::ios::out));
		out2 = out;
		*output << std::endl;
		if ((out && out->is_open()))
		{
#line 109
			*output << "Inside: " << std::boolalpha  << (((out && out->good()))) << " " << std::boolalpha  << (((out && out->is_open()))) << std::endl;
			std::shared_ptr<std::ostream> nate__writer = out2;
#line 110
			*nate__writer << "Hello,planet ";
			*nate__writer << 12 << " " << 13 << 14;
			*nate__writer << 15 << std::endl;
			nate__writer = output;
#line 113
			*nate__writer << "Hi ";
			*output << "Hi ";
			*nate__writer << "End" << std::endl;
			*nate__writer << std::endl;
			nate__writer = out2;
#line 117
			*nate__writer << "Hi there" << std::endl;
			if (out) out->close();
		}
#line 119
		*output << "After: " << std::boolalpha  << (((out && out->good()))) << " " << std::boolalpha  << (((out && out->is_open()))) << std::endl;
	}
	int32_t cnt = 2;
	cnt = 3;
#line 125
	auto const tmp__0__ = cnt;
#line 125
	switch (tmp__0__)
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
	}
#line 132
	auto const tmp__1__ = cnt + 1;
#line 132
	switch (tmp__1__)
	{
#line 132
	case 10:
	case 11:
	{
#line 134
		*output << "case" << std::endl;
	}
	break;
#line 135
	case 13:
	{
#line 136
		*output << "case 13" << std::endl;
	}
	break;
	default:
	{
#line 138
		{
			*output << "else" << std::endl;
		}
#line 140
		*output << "default" << std::endl;
	}
	break;
	}
#line 143
	auto const tmp__2__ = cnt + 2;
#line 143
	if ((tmp__2__ == cnt * 4))
	{
#line 144
		*output << "hello" << std::endl;
	}
#line 145
	else if ((tmp__2__ == cnt + 4)
#line 145

					 || (tmp__2__ == cnt - 4))
	{
#line 147
		*output << "hi" << std::endl;
		cnt = 2;
	}
#line 151
	auto const tmp__3__ = cnt + 3;
#line 151
	if ((tmp__3__ == cnt * 4))
	{
#line 152
		*output << "hello" << std::endl;
	}
#line 153
	else if ((tmp__3__ == cnt + 4)
#line 153

					 || (tmp__3__ == cnt - 4))
	{
#line 155
		*output << "hi" << std::endl;
		cnt = 2;
	}
	else
	{
#line 158
		*output << "else" << std::endl;
	}
#line 161
	auto const tmp__4__ = cnt + 4;
#line 161
	switch (tmp__4__)
	{
#line 161
	case 10:
	case 11:
	{
#line 163
		*output << "case" << std::endl;
	}
	break;
	{
#line 165
		*output << "hello" << std::endl;
	}
	break;
	{
#line 168
		*output << "hi" << std::endl;
		cnt = 2;
	}
	break;
	}
#line 172
	auto const tmp__5__ = cnt + 5;
#line 172
	switch (tmp__5__)
	{
#line 172
	case 10:
	case 11:
	{
#line 174
		*output << "case" << std::endl;
	}
	break;
	{
#line 176
		*output << "hello" << std::endl;
	}
	break;
	{
#line 179
		*output << "hi" << std::endl;
		cnt = 2;
	}
	break;
	default:
	{
#line 175
		if ((tmp__5__ == cnt * 4))
		{
#line 176
			*output << "hello" << std::endl;
		}
#line 177
		else if ((tmp__5__ == cnt + 4)
#line 177

						 || (tmp__5__ == cnt - 4))
		{
#line 179
			*output << "hi" << std::endl;
			cnt = 2;
		}
		else
		{
#line 182
			*output << "else" << std::endl;
		}
	}
	break;
	}
#line 185
	while (true)
	{
#line 186
		int32_t cnt = 2;
		if (!(cnt != 2)) break;
		cnt = 3;
	}
	while (true)
	{
#line 190
		if (!(cnt != 3)) break;
		int32_t cnt = 2;
		cnt = 3;
	}
	const std::string tmp__6__ = "abcd⅑⅛⅐efg";
	auto tmp__8__ = tmp__6__.cbegin();
	for (auto tmp__7__ = tmp__6__.cbegin(); tmp__7__ != tmp__6__.cend(); tmp__7__=tmp__8__)
	{
		uint32_t item = utf8::next(tmp__8__,tmp__6__.cend());
#line 195
		*output << (Core::toString(item)) << std::endl;
	}
	for (int32_t lop= 1;lop<=10; lop+=1)
	{
#line 197
		if (!(lop != 20)) break;
		int32_t cnt = 2;
		*output << ((lop * cnt)) << std::endl;
	}
	for (int32_t lop= 1;lop<=10; lop+=2)
	{
#line 201
		if (!(lop != 20)) break;
		int32_t cnt = 2;
		*output << ((lop * cnt)) << std::endl;
	}
	for (int32_t lop= 10;lop >= 1; lop -= 1)
	{
#line 206
		int32_t cnt = 2;
		*output << ((lop * cnt)) << std::endl;
	}
	for (int32_t lop= 10;lop >= 1; lop -= 2)
	{
#line 210
		int32_t cnt = 2;
		if (!(lop != 20)) break;
		*output << ((lop * cnt)) << std::endl;
	}
	if (cnt == 3)
	{
#line 215
		*output << "Hello, world!" << std::endl;
		cnt = 4;
	}
	if (cnt != 3)
	{
#line 219
		float x = static_cast<float>(3.14);
		*output << (x) << (x) << " " << (x);
	}
#line 221
	else
	{
#line 222
		int32_t x = 3;
		*output << (x) << " " << ((x + 1)) << std::endl;
	}
	if (cnt < 3)
	{
#line 226
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))) << std::endl;
	}
#line 228
	else if (false)
	{
#line 229
		int32_t x = 3;
		*output << (x) << " " << ((x + 1)) << std::endl;
	}
	if (cnt < 3)
	{
#line 233
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))) << std::endl;
	}
#line 235
	else if (false)
	{
#line 236
		int32_t x = 3;
		*output << (x) << " " << ((x + 1)) << std::endl;
	}
#line 238
	else if (true)
	{
#line 239
		int32_t x = 3;
		*output << "Hello, world!" << std::endl;
	}
	if (cnt < 3)
	{
#line 243
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))) << std::endl;
	}
#line 245
	else if (false)
	{
#line 246
		int32_t x = 3;
		*output << (x) << " " << ((x + 1)) << std::endl;
	}
#line 248
	else
	{
#line 249
		int32_t x = 3;
		*output << "Hello, world!" << std::endl;
	}
	if (cnt < 3)
	{
#line 253
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))) << std::endl;
	}
#line 255
	else if (false)
	{
#line 256
		int32_t x = 3;
		*output << (x) << " " << ((x + 1)) << std::endl;
	}
#line 258
	else if (true)
	{
#line 259
		int32_t x = 3;
		*output << "Hello, world!" << std::endl;
	}
#line 261
	else
	{
#line 262
		int32_t y = 3;
		*output << "Bye, world!" << std::endl;
	}
}
