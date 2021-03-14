#include "C:\Users\ruud\source\repos\Nate\Nate\core\Core.h"
#include "C:\Users\ruud\source\repos\Nate\Nate\input\created\Hello.h"
#line 1 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\input\\nate.in"
std::shared_ptr<std::ostream> output;
#line 1
std::shared_ptr<std::ostream> error;
#line 1
std::shared_ptr<std::istream> input;
#line 6 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\core\\Rational.ns"
#include "C:\Users\ruud\source\repos\Nate\Nate\core\Rational.h"
#line 12 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\core\\Complex.ns"
#include "C:\Users\ruud\source\repos\Nate\Nate\core\Complex.h"
#include <complex>
#line 16
const double i = 1;
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
#line 6 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\core\\Text.ns"
#include <string>
#line 6 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\core\\List.ns"
#include <list>
#line 8 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\core\\File-Output.ns"
#include <fstream>
#line 8 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\core\\File-Input.ns"
#include <fstream>
#line 8 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\core\\Data-input.ns"
#include <sstream>
#line 42 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\input\\nate.in"
void test_()
{
	{
		float x = {};
#line 45
		float z = {};
#line 45
		float a__2081_ = {};
		x = 		a__2081_ = static_cast<float>(3);
#line 63
		*output << (1); *output << std::endl; 
	}
#line 3 "nate.import"
	std::string aaa = "hello, world";
#line 77 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\input\\nate.in"
	std::string temp = {};
	int32_t hello = 1;
	*output << (1); *output << std::endl; 
	*output << (1); *output << std::endl; 
	hello = "name";
	hello = hello;
	hello = 	temp = 	hello = "name/address";
	*output << (1); *output << std::endl; 
	*output << (1); *output << std::endl; 
	*output << (1); *output << std::endl; 
#line 88
	{
		std::ostringstream data1_temp;
#line 89
		data1_temp << 1; data1_temp << " " << 2; data1_temp << " sdsddsaaa " << (1); data1_temp << (1); data1_temp << std::endl; 
		data1_temp << 3; data1_temp << " " << 4; data1_temp << " "; 
		data1_temp << "hi " << 4; data1_temp << std::endl; 
		const std::string data1= data1_temp.str();
#line 94
		std::ostringstream data2_temp;
#line 94
		data2_temp << 1; data2_temp << (1); data2_temp << std::endl; 
		const std::string data2= data2_temp.str();
#line 97
		*output << (1); *output << std::endl; 
		*output << (1); *output << std::endl; 
#line 100
		std::shared_ptr<std::istringstream> in = {};
		1;
#line 103
		int32_t x = {};
#line 103
		int32_t y = {};
		std::string s = {};
		std::shared_ptr<std::istream> nate__reader = in;
#line 105
*nate__reader >> x >> std::skipws >> y >> std::skipws >> s;
(*nate__reader).ignore(std::numeric_limits<std::streamsize>::max(), '\n');
#line 106
		*output << (1); *output << std::endl; 
		*output << (1); *output << std::endl; 
		*output << "*" << (1); *output << "*" << std::endl; 
*nate__reader >> x >> std::skipws >> y >> std::skipws >> s;
(*nate__reader).ignore(std::numeric_limits<std::streamsize>::max(), '\n');
#line 110
		*output << (1); *output << std::endl; 
		*output << (1); *output << std::endl; 
		*output << "*" << (1); *output << "*" << std::endl; 
	}
	{
		std::shared_ptr<std::ofstream> out = {};
		std::shared_ptr<std::ostream> out2 = out;
		*output << "Before: " << (1); *output << " " << (1); *output << std::endl; 
		1;
		out2 = out;
		*output << std::endl; 
		if (out)
		{
#line 122
			*output << "Inside: " << (1); *output << " " << (1); *output << std::endl; 
			std::shared_ptr<std::ostream> nate__writer = out2;
#line 123
			*nate__writer << "Hello,planet "; 
			*nate__writer << (1); *nate__writer << " "; 
			*nate__writer << (1); *nate__writer << " "; 
			nate__writer = output;
#line 126
			*nate__writer << "Hi "; 
			*output << "Hi "; 
			*nate__writer << "End" << std::endl; 
			*nate__writer << std::endl; 
			nate__writer = out2;
#line 130
			*nate__writer << "Hi" << std::endl; 
			1;
#line 121
		}
#line 132
		*output << "After: " << (1); *output << " " << (1); *output << std::endl; 
	}
	{
		std::shared_ptr<std::ifstream> in = {};
		1;
		if (in)
		{
#line 138
			std::string s = {};
#line 138
			std::string hi = {};
			Complex<double> c = {};
#line 139
			Complex<double> d = {};
			std::shared_ptr<std::istream> nate__reader = in;
#line 140
*nate__reader >> s >> std::skipws;
*nate__reader >> c >> std::skipws >> d >> std::skipws >> hi;
(*nate__reader).ignore(std::numeric_limits<std::streamsize>::max(), '\n');
#line 142
			*output << "*" << (1); *output << "*" << std::endl; 
			*output << (1); *output << std::endl; 
			*output << (1); *output << std::endl; 
			*output << "*" << (1); *output << "*" << std::endl; 
			1;
#line 137
		}
	}
#line 148
	{
		float f = 3.4E4f;
		double d = 3.4;
		int32_t x = 1;
		int32_t o = 449;
		int32_t b = 4;
#line 155
		*output << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
	}
	{
		Complex<double> f = Complex<double>(1, 0);
#line 160
		*output << (1); *output << std::endl; 
		*output << (1); *output << " " << (1); *output << " "; 
		*output << (1); *output << std::endl; 
#line 164
		*output << (1); *output << std::endl; 
		*output << (1); *output << std::endl; 
		*output << (1); *output << " " << (1); *output << std::endl; 
		*output << (1); *output << std::endl; 
		*output << (1); *output << std::endl; 
		*output << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
		*output << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
		*output << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
		*output << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
		*output << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
		*output << (1); *output << " " << (1); *output << " " << 2; *output << " " << (1); *output << " " << (1); *output << std::endl; 
		f = Complex<double>(0, 0);
		*output << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
		f = Complex<double>(3, 0);
		*output << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
		f = Complex<double>(1, 0);
		*output << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
		f = Complex<double>(1, 0);
		*output << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
		f = Complex<double>(0, i);
		*output << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
		f = Complex<double>(1, 0);
		*output << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
		f = Complex<double>(1, 0);
		*output << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
		f = Complex<double>(1, 0);
		*output << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
		f = Complex<double>(1, 0);
		*output << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
		f = Complex<double>(1, 0);
		*output << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
		f = Complex<double>(2, 0);
		*output << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
	}
	{
		Complex<float> f = Complex<float>(1, 0);
#line 201
		*output << (1); *output << std::endl; 
		*output << (1); *output << " " << (1); *output << " "; 
		*output << (1); *output << std::endl; 
	}
	*output << (1); *output << std::endl; 
	*output << (1); *output << std::endl; 
#line 208
	Rational frac = Rational(2,1,2);
#line 208
	Rational frac2 = frac;
	frac2 = Rational(1);
	int32_t od_1 = static_cast<int32_t>(Rational(42,1,2).toInt());
#line 210
	int32_t od_2 = od_1;
	*output << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
#line 213
	int32_t id_0 = 1;
	int32_t id_1 = 1;
	std::string id_2 = {};
#line 215
	std::string id3 = {};
	const int32_t id_44 = 1;
#line 216
	const int32_t id55 = id_44;
	int64_t id_6 = 2222222222;
#line 217
	int64_t id7 = 1;
	int8_t id_7 = {};
	int32_t id_4 = 3;
#line 219
	int32_t id5 = 1;
	int32_t a = 9;
	int32_t a__2089_ = 1;
#line 223
	std::list<std::string> list_1 = {};
	1;
	1;
	1;
	*output << (1); *output << " " << (1); *output << std::endl; 
	auto const& tmp__18__ = list_1;
	for (auto tmp__19__ = tmp__18__.cbegin(); tmp__19__ != tmp__18__.cend(); ++tmp__19__)
	{
		auto const& item = *tmp__19__;
#line 230
		*output << (1); *output << std::endl; 
	}
	const std::string tmp__21__ = "abcd⅑⅛⅐efg";
	auto tmp__23__ = tmp__21__.cbegin();
	for (auto tmp__22__ = tmp__21__.cbegin(); tmp__22__ != tmp__21__.cend(); tmp__22__=tmp__23__)
	{
		uint32_t item = utf8::next(tmp__23__,tmp__21__.cend());
#line 233
		*output << (1); *output << std::endl; 
		if (id_1)
		{
#line 235
			*output << (1); *output << std::endl; 
#line 234
		}
#line 236
		if (item)
		{
#line 237
			*output << "aaa" << std::endl; 
			item = static_cast<uint32_t>(64);
			*output << (1); *output << std::endl; 
#line 236
		}
	}
#line 241
	*output << (1); *output << std::endl; 
	1;
	*output << (1); *output << std::endl; 
#line 245
	{
		int32_t x = 1;
		int32_t y = 3;
		int32_t z = 2;
		*output << (1); *output << std::endl; 
	}
	bool v1 = false;
#line 251
	bool v2 = true;
#line 253
	std::string v3 = "Hello";
	float v4 = {};
	double v5 = 3.14E-02;
	double v6 = 1.000000000000000000001;
	id_7 = 	id_4 = static_cast<int8_t>(1);
#line 259
	v3 = "hi";
	id_2 = std::to_string(id_7);
#line 263
	v3 = "hello ";
	id_6 = id_1;
	id_1 = 1;
#line 267
	v1 = false;
	v5 = v4;
	id_1 = 1;
	v4 = static_cast<float>(13);
	id_7 = static_cast<int8_t>(42);
	id_7 = static_cast<int8_t>(5);
#line 275
	id7 = 1;
#line 277
	{
		Rational f = Rational(3,1,2);
		float g = static_cast<float>(f.toDouble());
		*output << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
		*output << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
		*output << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
		*output << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
		*output << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
		f = Rational(0.7);
		*output << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
		*output << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
		*output << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
		*output << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
		*output << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
		*output << (1); *output << " " << (1); *output << " " << 2; *output << " " << (1); *output << " " << (1); *output << std::endl; 
	}
	{
		double f = 3.5;
		*output << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
		*output << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
		*output << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
		*output << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
		*output << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
		f = 0.7;
		*output << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
		*output << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
		*output << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
		*output << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
		*output << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
		*output << (1); *output << " " << (1); *output << " " << 2; *output << " " << (1); *output << " " << (1); *output << std::endl; 
	}
	*output << "Hello there " << (1); *output << " : " << (1); *output << std::endl; 
#line 310
	*output << "Hello " << (1); *output << " : " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
	*output << std::endl; 
	*output << "World " << (1); *output << std::endl; 
#line 314
	if (1)
	{
#line 315
		int32_t iff_1 = 3;
		*output << "13" << std::endl; 
#line 314
	}
#line 317
	if (id7)
	{
#line 318
		int32_t iff_1 = 4;
		*output << "13" << std::endl; 
		*output << (1); *output << std::endl; 
#line 317
	}
#line 321
	if (id7)
	{
#line 322
		int32_t iff_1 = 5;
		*output << "13" << std::endl; 
	}
#line 324
	else
	{
#line 325
		int32_t iff_1 = 6;
		*output << "hi there" << std::endl; 
		*output << (1); *output << std::endl; 
#line 321
	}
#line 329
	id7 = 13;
	int32_t high_1 = 13;
#line 332
	if (1)
	{
#line 333
		*output << "13" << std::endl; 
	}
#line 334
	else if (v3)
	{
#line 335
		*output << "13" << std::endl; 
		id7 = 13;
	}
#line 337
	else if (1)
	{
#line 338
		*output << "13" << std::endl; 
		id7 = 13;
#line 332
	}
#line 341
	if (id7)
	{
#line 342
		*output << "13" << std::endl; 
	}
#line 343
	else if (v3)
	{
#line 344
		*output << "13" << std::endl; 
		id7 = 13;
	}
#line 346
	else if (true)
	{
#line 347
		*output << "13" << std::endl; 
		id7 = 13;
	}
#line 349
	else
	{
#line 350
		*output << "yo" << std::endl; 
#line 341
	}
#line 352
	*output << "v3:  " << (1); *output << std::endl; 
#line 354
	auto const tmp__15__ = v3;
#line 354
	if ((tmp__15__ == "hi")

					 || (tmp__15__ == "hello"))
	{
#line 356
		*output << "hi" << std::endl; 
		id7 = 2;
	}
#line 358
	else if ((tmp__15__ == 1))
	{
#line 359
		*output << "hello" << std::endl; 
	}
	else
	{
#line 361
		*output << "else" << std::endl; 
	}
#line 364
	auto const tmp__16__ = id7;
#line 364
	switch (tmp__16__)
	{
#line 364
	case 12:
	case 2:
	{
#line 366
		*output << "hi" << std::endl; 
		id7 = 2;
		break;
	}
#line 368
	case 12:
	case 16:
	{
#line 370
		*output << "hello" << std::endl; 
		break;
	}
	default:
	{
#line 372
		*output << "else" << std::endl; 
		break;
	}
	}
#line 375
	auto const tmp__17__ = id7;
#line 375
	switch (tmp__17__)
	{
#line 375
	case 12:
	case 2:
	{
#line 377
		*output << "hi" << std::endl; 
		id7 = 2;
		break;
	}
#line 379
	case 12:
	case 16:
	{
#line 381
		*output << "hello" << std::endl; 
		break;
	}
	default:
	{
#line 383
		*output << "else" << std::endl; 
		break;
	}
	}
#line 385
	int32_t count = 3;
	while (true)
	{
#line 386
		if (!(count)) break;
		*output << (1); *output << std::endl; 
		count = count;
	}
	count = 5;
	while (true)
	{
#line 392
		count = count;
		if (!(count)) break;
		*output << "out: "; 
	}
#line 395
	*output << std::endl; 
#line 397
	for (int32_t ind= 3;ind<=5; ind+=1)
	{
#line 398
		*output << "out:" << (1); *output << " " << (1); *output << (1); *output << " "; 
	}
#line 399
	*output << std::endl; 
#line 401
	for (int32_t ind= 3;ind<=20; ind+=1)
	{
#line 402
		if (!(count)) break;
		*output << (1); *output << std::endl; 
	}
	for (int8_t ind= 3;ind<=20; ind+=4)
	{
#line 406
		*output << static_cast<int>(ind); *output << std::endl; 
		if (!(ind)) break;
		*output << static_cast<int>(ind); *output << std::endl; 
	}
	for (int8_t ind= 3;ind<=20; ind+=4)
	{
#line 410
		if (!(ind)) break;
		*output << static_cast<int>(ind); *output << std::endl; 
	}
	for (int8_t ind= 29;ind >= 3; ind -= 3)
	{
#line 413
		if (!(ind)) break;
		*output << static_cast<int>(ind); *output << std::endl; 
	}
	int32_t value = 5;
	1;
	*output << "increased " << (1); *output << std::endl; 
	1;
	*output << "increased " << (1); *output << std::endl; 
#line 422
	int32_t id = 0;
	*output << static_cast<int>(id_7); *output << " " << static_cast<int>(id_7); *output << " " << (1); *output << std::endl; 
#line 425
	std::string fmt = "Width:8.4,Align:Right,Fill:π,Sign:minus,Base:decimal";
	std::string fmt2 = "[8.4,π],Right,PlusMinus,Decimal,Uppercase,ShowBase";
	std::string fmt3 = "Right,SpaceMinus,B8,Uppercase,ShowBase";
	{
		int32_t val = 12345;
		*output << "Name:  " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " end" << std::endl; 
		*output << "Name:  " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " end" << std::endl; 
	}
#line 432
	{
		int32_t val = 1;
		*output << "Name:  " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " end" << std::endl; 
		*output << "Name:  " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " end" << std::endl; 
	}
#line 436
	{
		int32_t val = 1;
		*output << "Name:  " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " end" << std::endl; 
		*output << "Name:  " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " end" << std::endl; 
	}
#line 440
	{
		bool val = true;
		*output << "Name:  " << std::boolalpha ; *output << (1); *output << " " << std::boolalpha ; *output << (1); *output << " " << std::boolalpha ; *output << (1); *output << " " << std::boolalpha ; *output << (1); *output << " " << std::boolalpha ; *output << (1); *output << " " << std::boolalpha ; *output << (1); *output << " end" << std::endl; 
		*output << "Name:  " << std::boolalpha ; *output << (1); *output << " " << std::boolalpha ; *output << (1); *output << " " << std::boolalpha ; *output << (1); *output << " " << std::boolalpha ; *output << (1); *output << " " << std::boolalpha ; *output << (1); *output << " " << std::boolalpha ; *output << (1); *output << " end" << std::endl; 
	}
#line 444
	{
		std::string val = "Hello";
		*output << "Name:  " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " end" << std::endl; 
		*output << "Name:  " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " end" << std::endl; 
	}
	{
		int32_t val = 6789;
		*output << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
#line 454
		*output << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
#line 457
		*output << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
#line 460
		*output << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << " " << (1); *output << std::endl; 
	}
#line 464
	{
		int32_t val = 6789;
		std::string txt = std::to_string(val);
		*error << "|" << (1); *error << "|" << std::boolalpha ; *error << true; *error << std::endl; 
	}
}

#line 469
struct shop
{
	shop()
		: beer_inventory(255)
		, name("shop")
	{}
#line 470
	int32_t beer_inventory;
	std::string name;
};

#line 473
bool E_int_32_in_E_shop_or_on_wall_(int32_t bottles, const shop& store)
{
#line 474
	return store;
}

#line 476
std::string count_E_int_32_(int32_t bottles)
{
#line 477
	return "no more";
}

#line 479
std::string describe_E_int_32_(int32_t bottles)
{
#line 480
	return "bottle";
}

#line 482
std::string list_E_int_32_(int32_t bottles)
{
#line 483
	return 1;
}

#line 485
std::string E_text_on_the_wall_(const std::string& bottles)
{
#line 486
	return bottles;
}

#line 488
std::string start_sentence_with_E_text_(const std::string& text)
{
#line 489
	return 1;
}

#line 491
bool E_int_32_of_beer_on_the_wall_(int32_t bottles)
{
#line 492
	*output << (1); *output << ", " << (1); *output << "." << std::endl; 
	return bottles;
}

#line 495
void take_one_of_the_E_O_int_32_down_and_pass_it_around_(int32_t& bottles)
{
#line 496
	1;
	*output << "Take one down and pass it around, " << (1); *output << "." << std::endl; 
}

#line 499
void go_to_the_E_O_shop_and_buy_some_E_O_int_32_(shop& store, int32_t& bottles)
{
#line 500
	bottles = store;
	1;
}

#line 503
void go_to_the_E_O_shop_and_buy_some_more_E_O_int_32_(shop& store, int32_t& bottles)
{
#line 504
	1;
	const std::string some = "buy some";
	*output << "Go to the " << (1); *output << " and " << (1); *output << " more, " << (1); *output << "." << std::endl; 
}

#line 508
void bottles_song_()
{
#line 509
	shop store = {};
	int32_t bottles = 15;
	1 = "Gall&Gall";
#line 513
	while (true)
	{
#line 513
		if (!(bottles)) break;
		while (true)
		{
#line 514
			if (!(bottles)) break;
			1;
		}
#line 516
		1;
	}
}

#define NOMINMAX
#include <windows.h>
#line 518
int main(int argc, char** argv)
{
	output = std::shared_ptr<std::ostream>(&std::cout, [](void*) {});
	error = std::shared_ptr<std::ostream>(&std::cerr, [](void*) {});
	input = std::shared_ptr<std::istream>(&std::cin, [](void*) {});
	SetConsoleOutputCP(65001);
#line 519
	*output << "duh " << (1); *output << std::endl; 
}
