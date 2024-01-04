#include "C:\Users\ruud\source\repos\Nate\Nate\core\Core.h"
#include "C:\Users\ruud\source\repos\Nate\Nate\core\created\File-Input.h"
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
#line 18
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
#line 19
	struct Datum
	{
		Datum()
			: fun()
			, count()
		{}
#line 19
		string_t fun;
#line 19
		int32_t count;
	};

#line 27
	static std::shared_ptr<Hi> new_Hi2_();
	virtual string_t E_me__E_text_(const string_t& world) const;
private:
	int32_t prop_hi = {};
public:
	virtual int32_t prop_hi_get() const;
	virtual const int32_t prop_hi_set(const int32_t value);
private:
	Datum datum = {};
public:
	virtual Hi::Datum datum_get() const;
	virtual const Hi::Datum& datum_set(const Hi::Datum& value);
private:
	int32_t age = {};
public:
	virtual int32_t age_get() const;
};

#line 31
class Hi::__impl
{
private:
	friend class Hi;
	Hi* me;
#line 32
	string_t local = {};
public:
	__impl(Hi* aMe) : me(aMe) {}
#line 43
	string_t yoh_E_me__() const
	{
#line 44
		return "Hi " + local;
	}
	static string_t yo_()
	{
#line 47
		return "Hi";
	}
	string_t E_O_me__E_text_(const string_t& world)
	{
#line 50
		int32_t loc = (-3) + me->prop_hi;
		return ("ls " + (__impl::yo_())) + local;
	}
};

Hi::Hi()
	: _impl(new __impl(this)) {}
Hi::~Hi() { delete _impl; }
#line 34
int32_t Hi::age_get() const
{
	return age;
}
const int32_t Hi::prop_hi_set(const int32_t value)
{
	prop_hi = value;
	return prop_hi;
}
#line 53
std::shared_ptr<Hi> Hi::new_Hi2_()
{
#line 54
	return std::make_shared<Hi>();
}
string_t Hi::E_me__E_text_(const string_t& world) const
{
#line 57
	return (((((__impl::yo_()) + " ") + (std::to_string(prop_hi))) + " ") + world) + _impl->local;
}
int32_t Hi::prop_hi_get() const { return prop_hi; }
Hi::Datum Hi::datum_get() const { return datum; }
const Hi::Datum& Hi::datum_set(const Hi::Datum& value) { return datum = value; }
#line 61
bool why_so_E_text_(const string_t& serious)
{
#line 62
	*output << "why so " << (serious) << std::endl;
	return false;
}
#undef NOMINMAX
#define NOMINMAX
#include <windows.h>
#line 67
int main(int argc, char** argv)
{
	output = std::shared_ptr<std::ostream>(&std::cout, [](void*) {});
	error = std::shared_ptr<std::ostream>(&std::cerr, [](void*) {});
	input = std::shared_ptr<std::istream>(&std::cin, [](void*) {});
	SetConsoleOutputCP(65001);
#line 68
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
#line 70
		int32_t fld1;
#line 70
		string_t fld2;
#line 70
		Complex<double> fld3;
#line 70
		Rational fld4;
	};

#line 76
	Rec rec = {};
	*output << ((rec . fld1)) << " " << ((rec . fld4)) << std::endl;
#line 79
	std::ostringstream data1_temp;
#line 79
	data1_temp << 1 << " " << 2 << " sdsddsaaa " << (hello) << ((static_cast<int32_t>(hello.size()))) << std::endl << 3 << " " << 4 << 5 << " " << 6 << 7 << " " << 8 << std::endl << "hi " << ((4 * 5)) << std::endl;
	const string_t data1= data1_temp.str();
#line 86
	std::ostringstream data2_temp;
#line 86
	data2_temp << 1 << (data1) << std::endl;
	const string_t data2= data2_temp.str();
#line 89
	*error << "Hello " << (hello) << std::endl;
#line 92
	{
		std::shared_ptr<std::istringstream> in = {};
		in.reset(new std::istringstream(data1));
		int32_t x = {};
#line 95
		int32_t y = {};
		string_t s = {};
		std::shared_ptr<std::istream> nate__reader = in;
#line 97
		*nate__reader >> x;
		*nate__reader >> std::ws >> y;
		*nate__reader >> std::ws >> s;
		(*nate__reader).ignore(std::numeric_limits<std::streamsize>::max(), '\n');
#line 98
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
#line 110
			*output << "Inside: " << std::boolalpha  << (((out && out->good()))) << " " << std::boolalpha  << (((out && out->is_open()))) << std::endl;
			std::shared_ptr<std::ostream> nate__writer = out2;
#line 111
			*nate__writer << "Hello,planet ";
			*nate__writer << 12 << " " << 13 << 14;
			*nate__writer << 15 << std::endl;
			nate__writer = output;
#line 114
			*nate__writer << "Hi ";
			*output << "Hi ";
			*nate__writer << "End" << std::endl;
			*nate__writer << std::endl;
			nate__writer = out2;
#line 118
			*nate__writer << "Hi there" << std::endl;
			if (out) out->close();
		}
#line 120
		*output << "After: " << std::boolalpha  << (((out && out->good()))) << " " << std::boolalpha  << (((out && out->is_open()))) << std::endl;
	}
	{
		std::shared_ptr<File_Input> in = File_Input::new_File_Input_();
		in->open_E_O_me__from_file_E_text_("d:\\uit");
		*output << std::endl;
		if (in->E_me__is_opened_())
		{
#line 127
			*output << "Inside: " << std::boolalpha  << ((in->E_me__is_ok_())) << " " << std::boolalpha  << ((in->E_me__is_opened_())) << std::endl;
			string_t text = {};
			std::shared_ptr<std::istream> nate__reader = (in)->E_me__stream_();
#line 129
			std::getline(*nate__reader, text);
			in->close_E_O_me__();
		}
#line 131
		*output << "After: " << std::boolalpha  << ((in->E_me__is_ok_())) << " " << std::boolalpha  << ((in->E_me__is_opened_())) << std::endl;
	}
	int32_t cnt = 2;
	cnt = 3;
#line 137
	auto const tmp__0__ = cnt;
#line 137
	switch (tmp__0__)
	{
#line 137
	case 10:
	case 11:
	{
#line 139
		*output << "case" << std::endl;
	}
	break;
#line 140
	case 13:
	{
#line 141
		*output << "case 13" << std::endl;
	}
	break;
	}
#line 144
	auto const tmp__1__ = cnt + 1;
#line 144
	switch (tmp__1__)
	{
#line 144
	case 10:
	case 11:
	{
#line 146
		*output << "case" << std::endl;
	}
	break;
#line 147
	case 13:
	{
#line 148
		*output << "case 13" << std::endl;
	}
	break;
	default:
	{
#line 150
		{
			*output << "else" << std::endl;
		}
#line 152
		*output << "default" << std::endl;
	}
	break;
	}
#line 155
	auto const tmp__2__ = cnt + 2;
#line 155
	if ((tmp__2__ == cnt * 4))
	{
#line 156
		*output << "hello" << std::endl;
	}
#line 157
	else if ((tmp__2__ == cnt + 4)
#line 157

					 || (tmp__2__ == cnt - 4))
	{
#line 159
		*output << "hi" << std::endl;
		cnt = 2;
	}
#line 163
	auto const tmp__3__ = cnt + 3;
#line 163
	if ((tmp__3__ == cnt * 4))
	{
#line 164
		*output << "hello" << std::endl;
	}
#line 165
	else if ((tmp__3__ == cnt + 4)
#line 165

					 || (tmp__3__ == cnt - 4))
	{
#line 167
		*output << "hi" << std::endl;
		cnt = 2;
	}
	else
	{
#line 170
		*output << "else" << std::endl;
	}
#line 173
	auto const tmp__4__ = cnt + 4;
#line 173
	switch (tmp__4__)
	{
#line 173
	case 10:
	case 11:
	{
#line 175
		*output << "case" << std::endl;
	}
	break;
	{
#line 177
		*output << "hello" << std::endl;
	}
	break;
	{
#line 180
		*output << "hi" << std::endl;
		cnt = 2;
	}
	break;
	}
#line 184
	auto const tmp__5__ = cnt + 5;
#line 184
	switch (tmp__5__)
	{
#line 184
	case 10:
#line 184
	case 11:
	{
#line 185
		*output << "case" << std::endl;
	}
	break;
	{
#line 187
		*output << "hello" << std::endl;
	}
	break;
	{
#line 189
		*output << "hi" << std::endl;
		cnt = 2;
	}
	break;
	default:
	{
#line 186
		if ((tmp__5__ == cnt * 4))
		{
#line 187
			*output << "hello" << std::endl;
		}
#line 188
		else if ((tmp__5__ == cnt + 4)
#line 188

						 || (tmp__5__ == cnt - 4))
		{
#line 189
			*output << "hi" << std::endl;
			cnt = 2;
		}
		else
		{
#line 192
			*output << "else" << std::endl;
		}
	}
	break;
	}
#line 195
	while (true)
	{
#line 196
		int32_t cnt = 2;
		if (!(cnt != 2)) break;
		cnt = 3;
	}
	while (true)
	{
#line 200
		if (!(cnt != 3)) break;
		int32_t cnt = 2;
		cnt = 3;
	}
	const std::string tmp__6__ = "abcd⅑⅛⅐efg";
	auto tmp__8__ = tmp__6__.cbegin();
	for (auto tmp__7__ = tmp__6__.cbegin(); tmp__7__ != tmp__6__.cend(); tmp__7__=tmp__8__)
	{
		uint32_t item = utf8::next(tmp__8__,tmp__6__.cend());
#line 205
		*output << (Core::toString(item)) << std::endl;
	}
	for (int32_t lop= 1;lop<=10; lop+=1)
	{
#line 207
		if (!(lop != 20)) break;
		int32_t cnt = 2;
		*output << ((lop * cnt)) << std::endl;
	}
	for (int32_t lop= 1;lop<=10; lop+=2)
	{
#line 211
		if (!(lop != 20)) break;
		int32_t cnt = 2;
		*output << ((lop * cnt)) << std::endl;
	}
	for (int32_t lop= 10;lop >= 1; lop -= 1)
	{
#line 216
		int32_t cnt = 2;
		*output << ((lop * cnt)) << std::endl;
	}
	for (int32_t lop= 10;lop >= 1; lop -= 2)
	{
#line 220
		int32_t cnt = 2;
		if (!(lop != 20)) break;
		*output << ((lop * cnt)) << std::endl;
	}
	if (cnt == 3)
	{
#line 225
		*output << "Hello, world!" << std::endl;
		cnt = 4;
	}
	if (cnt != 3)
	{
#line 229
		float x = static_cast<float>(3.14);
		*output << (x) << (x) << " " << (x);
	}
#line 231
	else
	{
#line 232
		int32_t x = 3;
		*output << (x) << " " << ((x + 1)) << std::endl;
	}
	if (cnt < 3)
	{
#line 236
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))) << std::endl;
	}
#line 238
	else if (false)
	{
#line 239
		int32_t x = 3;
		*output << (x) << " " << ((x + 1)) << std::endl;
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
	else if (true)
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
	else
	{
#line 259
		int32_t x = 3;
		*output << "Hello, world!" << std::endl;
	}
	if (cnt < 3)
	{
#line 263
		double x = static_cast<double>(6);
		*output << ((std::pow(x, x))) << std::endl;
	}
#line 265
	else if (false)
	{
#line 266
		int32_t x = 3;
		*output << (x) << " " << ((x + 1)) << std::endl;
	}
#line 268
	else if (true)
	{
#line 269
		int32_t x = 3;
		*output << "Hello, world!" << std::endl;
	}
#line 271
	else
	{
#line 272
		int32_t y = 3;
		*output << "Bye, world!" << std::endl;
	}
}
