#include <cmath>
#include "Core.h"
#include "File-Input.h"
#line 1 "C:\\Users\\Ruud\\source\\repos\\Nate\\Nate\\input\\nate.in2"
std::shared_ptr<std::ostream> output_ = {};
#line 1
std::shared_ptr<std::ostream> error_ = {};
#line 1
std::shared_ptr<std::istream> input_ = {};
#line 27 ""
const float64_t pi_ = 3.14159265358979323;
const float64_t tau_ = 6.283185307179586;
const float64_t e_ = 2.718281828459045;
#line 8
#include <fstream>
#line 6
#include <sstream>
#line 10
#include "Complex.h"
#include <complex>
#line 15
const float64_t i_ = 1;
#line 6
#include "Rational.h"
#line 18
class Hi_: public BaseObject_
{
public:
	virtual ~Hi_();
	Hi_();
private:
	class __impl;
	__impl* _impl;
	friend class __impl;
public:
#line 19
	struct Datum_
	{
		Datum_()
			: fun_()
			, count_()
		{}
#line 19
		string_t fun_;
#line 19
		int32_t count_;
	};

#line 27
	static std::shared_ptr<Hi_> new__Hi2__();
	virtual string_t E_me__E_text__(const string_t& world_) const;
private:
	int32_t prop_hi_ = {};
public:
	virtual int32_t prop_hi__get() const;
	virtual const int32_t prop_hi__set(const int32_t value);
private:
	int32_t age_ = {};
public:
	virtual int32_t age__get() const;
private:
	Datum_ datum_ = {};
public:
	virtual Hi_::Datum_ datum__get() const;
	virtual const Hi_::Datum_& datum__set(const Hi_::Datum_& value);
};

#line 31
class Hi_::__impl
{
private:
	friend class Hi_;
	Hi_* me;
#line 32
	string_t local_ = {};
public:
	__impl(Hi_* aMe) : me(aMe) {}
#line 43
	string_t yoh__E_me__() const
	{
#line 44
		return "Hi " + local_;
	}
	static string_t yo__()
	{
#line 47
		return "Hi";
	}
	string_t E_O_me__E_text__(const string_t& world_)
	{
#line 50
		int32_t loc_ = -3 + me->prop_hi_;
		return ("ls " + (__impl::yo__())) + local_;
	}
};

Hi_::Hi_()
	: _impl(new __impl(this)) {}
Hi_::~Hi_() { delete _impl; }
#line 34
int32_t Hi_::age__get() const
{
	return age_;
}
const int32_t Hi_::prop_hi__set(const int32_t value)
{
	prop_hi_ = value;
	return prop_hi_;
}
#line 53
std::shared_ptr<Hi_> Hi_::new__Hi2__()
{
#line 54
	return std::make_shared<Hi_>();
}
string_t Hi_::E_me__E_text__(const string_t& world_) const
{
#line 57
	return (((((__impl::yo__()) + " ") + (std::to_string(prop_hi_))) + " ") + world_) + _impl->local_;
}
int32_t Hi_::prop_hi__get() const { return prop_hi_; }
Hi_::Datum_ Hi_::datum__get() const { return datum_; }
const Hi_::Datum_& Hi_::datum__set(const Hi_::Datum_& value) { return datum_ = value; }
#line 61
bool why__so__E_text__(const string_t& serious_)
{
#line 62
	*output_ << "why so " << (serious_) << "\n";
	return false;
}
#line 67
#include "main.h"
int main(int argc, char** argv)
{
	initMain(argc, argv);
#define NATE_PROGRAM_START
	std::shared_ptr<std::istream> nate__reader_ = {};
	std::shared_ptr<std::ostream> nate__writer_ = {};
#line 68
	string_t hello_ = " world";
	*output_ << std::boolalpha  << ((why__so__E_text__("serious"))) << "\n";
	struct Rec_
	{
		Rec_()
			: fld1_(3)
			, fld2_()
			, fld3_(Complex<float64_t>(1, (2)))
			, fld4_(Rational(0,11,152) + Rational(0,1,57))
		{}
#line 70
		int32_t fld1_;
#line 70
		string_t fld2_;
#line 70
		Complex<float64_t> fld3_;
#line 70
		Rational fld4_;
	};

#line 76
	Rec_ rec_ = {};
	*output_ << ((rec_ . fld1_)) << " " << ((rec_ . fld4_)) << "\n";
#line 79
	std::ostringstream data1__temp;
#line 79
	data1__temp << 1 << " " << 2 << " sdsddsaaa " << (hello_) << ((static_cast<int32_t>(hello_.size()))) << "\n" << 3 << " " << 4 << 5 << " " << 6 << 7 << " " << 8 << "\n" << "hi " << ((4 * 5)) << "\n";
	const string_t data1_= data1__temp.str();
#line 86
	std::ostringstream data2__temp;
#line 86
	data2__temp << 1 << (data1_) << "\n";
	const string_t data2_= data2__temp.str();
#line 89
	*error_ << "Hello " << (hello_) << "\n";
#line 92
	{
		std::shared_ptr<std::istringstream> in_ = {};
		in_.reset(new std::istringstream(data1_));
		int32_t x_ = {};
#line 95
		int32_t y_ = {};
		string_t s_ = {};
		nate__reader_ = in_;
#line 97
		*nate__reader_ >> x_;
		*nate__reader_ >> std::ws >> y_;
		*nate__reader_ >> std::ws >> s_;
		(*nate__reader_).ignore(std::numeric_limits<std::streamsize>::max(), '\n');
#line 98
		*output_ << (x_) << " " << (y_) << " " << ((1)) << " " << ((1)) << "\n";
		*output_ << "hi there";
		*output_ << (s_) << "\n";
	}
	{
		std::shared_ptr<std::ofstream> out_ = {};
		std::shared_ptr<std::ostream> out2_ = out_;
		*output_ << "Before: " << std::boolalpha  << (((out_ && out_->good()))) << " " << std::boolalpha  << (((out_ && out_->is_open()))) << "\n";
		out_.reset(new std::ofstream("d:\\uit", std::ios::out));
		out2_ = out_;
		*output_ << "\n";
		if ((out_ && out_->is_open()))
		{
#line 110
			*output_ << "Inside: " << std::boolalpha  << (((out_ && out_->good()))) << " " << std::boolalpha  << (((out_ && out_->is_open()))) << "\n";
			nate__writer_ = out2_;
#line 111
			*nate__writer_ << "Hello,planet ";
			*nate__writer_ << 12 << " " << 13 << 14;
			*nate__writer_ << 15 << "\n";
			nate__writer_ = output_;
#line 114
			*nate__writer_ << "Hi ";
			*output_ << "Hi ";
			*nate__writer_ << "End\n";
			*nate__writer_ << "\n";
			nate__writer_ = out2_;
#line 118
			*nate__writer_ << "Hi there\n";
			if (out_) out_->close();
		}
#line 120
		*output_ << "After: " << std::boolalpha  << (((out_ && out_->good()))) << " " << std::boolalpha  << (((out_ && out_->is_open()))) << "\n";
	}
	{
		std::shared_ptr<File_Input_> in_ = File_Input_::new__File_Input__();
		in_->open__E_O_me__from__file__E_text__("d:\\uit");
		*output_ << "\n";
		if (in_->E_me__is__opened__())
		{
#line 127
			*output_ << "Inside: " << std::boolalpha  << ((in_->E_me__is__ok__())) << " " << std::boolalpha  << ((in_->E_me__is__opened__())) << "\n";
			string_t text_ = {};
			nate__reader_ = (in_)->E_me__stream__();
#line 129
			std::getline(*nate__reader_, text_);
			in_->close__E_O_me__();
		}
#line 131
		*output_ << "After: " << std::boolalpha  << ((in_->E_me__is__ok__())) << " " << std::boolalpha  << ((in_->E_me__is__opened__())) << "\n";
	}
	int32_t cnt_ = 2;
	cnt_ = 3;
#line 137
	auto const tmp__0___ = cnt_;
#line 137
	switch (tmp__0___)
	{
#line 137
	case 10:
	case 11:
	{
#line 139
		*output_ << "case\n";
	}
	break;
#line 140
	case 13:
	{
#line 141
		*output_ << "case 13\n";
	}
	break;
	}
#line 144
	auto const tmp__1___ = cnt_ + 1;
#line 144
	switch (tmp__1___)
	{
#line 144
	case 10:
	case 11:
	{
#line 146
		*output_ << "case\n";
	}
	break;
#line 147
	case 13:
	{
#line 148
		*output_ << "case 13\n";
	}
	break;
	default:
	{
#line 150
		{
			*output_ << "else\n";
		}
#line 152
		*output_ << "default\n";
	}
	break;
	}
#line 155
	auto const tmp__2___ = cnt_ + 2;
#line 155
	if ((tmp__2___ == cnt_ * 4))
	{
#line 156
		*output_ << "hello\n";
	}
#line 157
	else if ((tmp__2___ == cnt_ + 4)
#line 157

					 || (tmp__2___ == cnt_ - 4))
	{
#line 159
		*output_ << "hi\n";
		cnt_ = 2;
	}
#line 163
	auto const tmp__3___ = cnt_ + 3;
#line 163
	if ((tmp__3___ == cnt_ * 4))
	{
#line 164
		*output_ << "hello\n";
	}
#line 165
	else if ((tmp__3___ == cnt_ + 4)
#line 165

					 || (tmp__3___ == cnt_ - 4))
	{
#line 167
		*output_ << "hi\n";
		cnt_ = 2;
	}
	else
	{
#line 170
		*output_ << "else\n";
	}
#line 173
	auto const tmp__4___ = cnt_ + 4;
#line 173
	switch (tmp__4___)
	{
#line 173
	case 10:
	case 11:
	{
#line 175
		*output_ << "case\n";
	}
	break;
	{
#line 177
		*output_ << "hello\n";
	}
	break;
	{
#line 180
		*output_ << "hi\n";
		cnt_ = 2;
	}
	break;
	}
#line 184
	auto const tmp__5___ = cnt_ + 5;
#line 184
	switch (tmp__5___)
	{
#line 184
	case 10:
#line 184
	case 11:
	{
#line 185
		*output_ << "case\n";
	}
	break;
	{
#line 187
		*output_ << "hello\n";
	}
	break;
	{
#line 189
		*output_ << "hi\n";
		cnt_ = 2;
	}
	break;
	default:
	{
#line 186
		if ((tmp__5___ == cnt_ * 4))
		{
#line 187
			*output_ << "hello\n";
		}
#line 188
		else if ((tmp__5___ == cnt_ + 4)
#line 188

						 || (tmp__5___ == cnt_ - 4))
		{
#line 189
			*output_ << "hi\n";
			cnt_ = 2;
		}
		else
		{
#line 192
			*output_ << "else\n";
		}
	}
	break;
	}
#line 195
	while (true)
	{
#line 196
		int32_t cnt_ = 2;
		if (!(cnt_ != 2)) break;
		cnt_ = 3;
	}
	while (true)
	{
#line 200
		if (!(cnt_ != 3)) break;
		int32_t cnt_ = 2;
		cnt_ = 3;
	}
	const std::string tmp__6__ = "abcd⅑⅛⅐efg";
	auto tmp__8__ = tmp__6__.cbegin();
	for (auto tmp__7__ = tmp__6__.cbegin(); tmp__7__ != tmp__6__.cend(); tmp__7__=tmp__8__)
	{
		uint32_t item_ = utf8::next(tmp__8__,tmp__6__.cend());
#line 205
		*output_ << (Core::toString(item_)) << "\n";
	}
	for (int32_t lop_= 1;lop_<=10; lop_+=1)
	{
#line 207
		if (!(lop_ != 20)) break;
		int32_t cnt_ = 2;
		*output_ << ((lop_ * cnt_)) << "\n";
	}
	for (int32_t lop_= 1;lop_<=10; lop_+=2)
	{
#line 211
		if (!(lop_ != 20)) break;
		int32_t cnt_ = 2;
		*output_ << ((lop_ * cnt_)) << "\n";
	}
	for (int32_t lop_= 10;lop_ >= 1; lop_ -= 1)
	{
#line 216
		int32_t cnt_ = 2;
		*output_ << ((lop_ * cnt_)) << "\n";
	}
	for (int32_t lop_= 10;lop_ >= 1; lop_ -= 2)
	{
#line 220
		int32_t cnt_ = 2;
		if (!(lop_ != 20)) break;
		*output_ << ((lop_ * cnt_)) << "\n";
	}
	if (cnt_ == 3)
	{
#line 225
		*output_ << "Hello, world!\n";
		cnt_ = 4;
	}
	if (cnt_ != 3)
	{
#line 229
		float32_t x_ = static_cast<float32_t>(3.14);
		*output_ << (x_) << (x_) << " " << (x_);
	}
#line 231
	else
	{
#line 232
		int32_t x_ = 3;
		*output_ << (x_) << " " << ((x_ + 1)) << "\n";
	}
	if (cnt_ < 3)
	{
#line 236
		float64_t x_ = static_cast<float64_t>(6);
		*output_ << ((std::pow(x_, x_))) << "\n";
	}
#line 238
	else if (false)
	{
#line 239
		int32_t x_ = 3;
		*output_ << (x_) << " " << ((x_ + 1)) << "\n";
	}
	if (cnt_ < 3)
	{
#line 243
		float64_t x_ = static_cast<float64_t>(6);
		*output_ << ((std::pow(x_, x_))) << "\n";
	}
#line 245
	else if (false)
	{
#line 246
		int32_t x_ = 3;
		*output_ << (x_) << " " << ((x_ + 1)) << "\n";
	}
#line 248
	else if (true)
	{
#line 249
		int32_t x_ = 3;
		*output_ << "Hello, world!\n";
	}
	if (cnt_ < 3)
	{
#line 253
		float64_t x_ = static_cast<float64_t>(6);
		*output_ << ((std::pow(x_, x_))) << "\n";
	}
#line 255
	else if (false)
	{
#line 256
		int32_t x_ = 3;
		*output_ << (x_) << " " << ((x_ + 1)) << "\n";
	}
#line 258
	else
	{
#line 259
		int32_t x_ = 3;
		*output_ << "Hello, world!\n";
	}
	if (cnt_ < 3)
	{
#line 263
		float64_t x_ = static_cast<float64_t>(6);
		*output_ << ((std::pow(x_, x_))) << "\n";
	}
#line 265
	else if (false)
	{
#line 266
		int32_t x_ = 3;
		*output_ << (x_) << " " << ((x_ + 1)) << "\n";
	}
#line 268
	else if (true)
	{
#line 269
		int32_t x_ = 3;
		*output_ << "Hello, world!\n";
	}
#line 271
	else
	{
#line 272
		int32_t y_ = 3;
		*output_ << "Bye, world!\n";
	}
}
