#include "C:\Users\ruud\source\repos\Nate\Nate\core\Core.h"
std::shared_ptr<std::ostream> output = {};
std::shared_ptr<std::ostream> error = {};
#include "C:\Users\ruud\source\repos\Nate\Nate\core\Rational.h"
#include "C:\Users\ruud\source\repos\Nate\Nate\core\Complex.h"
#include <complex>
#define NOMINMAX
#include <windows.h>
#include <cmath>
#include <string>
#include <list>
#include <fstream>
#include <fstream>
#include <sstream>
#line 79 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\input\\nate.in"
	list_1.push_back("Hi there");
	list_1.push_back((std::to_string(666)));
#line 81
	list_1.insert(list_1.cbegin(), "Hello");
#line 16 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\core\\Complex.ns"
const double i = 1;
#line 32 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\core\\Math.ns"
const double pi = 3.14159265358979323;
#line 34
const double tau = 6.283185307179586;
#line 36
const double e = 2.718281828459045;
#define NOMINMAX
#include <windows.h>
#line 11 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\input\\nate.in"
int main(int argc, char** argv)
{
	output = std::shared_ptr<std::ostream>(&std::cout, [](void*) {});
	error = std::shared_ptr<std::ostream>(&std::cerr, [](void*) {});
	SetConsoleOutputCP(65001);
#line 12
	float x = {};
#line 12
	float z = {};
#line 12
	float a__2081_ = {};
	int32_t d = 5;
#line 13
	int32_t g = d;
	double f = 4.4;
	bool tr = true;
	x = 	a__2081_ = static_cast<float>(3);
	*output << "Hello, world " << 3;*output << std::endl;
	*output << (x);*output << (d);*output << (f);*output << " ";
	*output << std::endl;
#line 21
	if (d< 3)
	{
#line 22
		d = 4;
#line 24
	}
#line 24
	if ((d< 3)|| (d> 6))
	{
#line 25
		d = 5;
	}
#line 26
	else
	{
#line 27
		*output << (d);*output << std::endl;
#line 29
	}
#line 29
	if (tr)
	{
#line 30
		tr = false;
	}
#line 31
	else if (!tr)
	{
#line 32
		if (tr)
		{
#line 33
			tr = false;
		}
#line 34
		else if (!tr)
		{
#line 35
			tr = true;
		}
	}
#line 36
	else if (d> 3)
	{
#line 37
		tr = tr;
	}
#line 38
	else
	{
#line 39
		*output << std::boolalpha ;*output << (tr);*output << std::endl;
#line 41
	}
#line 41
	{
		if (tr)
		{
#line 43
			tr = false;
		}
#line 44
		else if (!tr)
		{
#line 45
			tr = true;
#line 47
		}
#line 47
		int32_t count = 3;
		while (true)
		{
#line 48
			if (!(count> 0)) break;
			*output << (count);*output << std::endl;
			count = count- 1;
		}
		count = 5;
		while (true)
		{
#line 54
			count = count- 1;
			if (!(count> 0)) break;
			*output << (("out: "+ (std::to_string(count))));
		}
#line 57
		*output << std::endl;
#line 59
		for (int32_t ind= 3;ind<=5;ind+=1)
		{
#line 60
			*output << "out:" << (ind);*output << " " << (ind);*output << (ind);*output << " ";
		}
#line 61
		*output << std::endl;
#line 63
		for (int32_t ind= 3;ind<=20;ind+=1)
		{
#line 64
			if (!(count> 0)) break;
			*output << (ind);*output << std::endl;
		}
	}
#line 67
	for (int8_t ind= 3;ind<=20;ind+=4)
	{
#line 68
		*output << static_cast<int>(ind);*output << std::endl;
		if (!(ind< 15)) break;
		*output << ((std::pow(ind, 2)));*output << std::endl;
	}
	for (int8_t ind= 3;ind<=20;ind+=4)
	{
#line 72
		if (!(ind< 15)) break;
		*output << static_cast<int>(ind);*output << std::endl;
	}
	for (int8_t ind= 29;ind >= 3;ind -= 3)
	{
#line 75
		if (!(ind> 15)) break;
		*output << static_cast<int>(ind);*output << std::endl;
	}
	std::list<std::string> list_1 = {};
#line 82
	*output << ((static_cast<int32_t>(list_1.size())));*output << " " << ((static_cast<int32_t>(list_1.size())));*output << std::endl;
	auto const& tmp__14__ = list_1;
	for (auto tmp__15__ = tmp__14__.cbegin(); tmp__15__ != tmp__14__.cend(); ++tmp__15__)
	{
		auto const& item = *tmp__15__;
#line 85
		*output << (item);*output << std::endl;
	}
	const std::string tmp__17__ = "abcd⅑⅛⅐efg";
	auto tmp__19__ = tmp__17__.cbegin();
	for (auto tmp__18__ = tmp__17__.cbegin(); tmp__18__ != tmp__17__.cend(); tmp__18__=tmp__19__)
	{
		uint32_t item = utf8::next(tmp__19__,tmp__17__.cend());
#line 88
		*output << (Core::toString(item));*output << std::endl;
		if (((Complex<double>(0, (i* d)))- 1)== (Complex<double>(3.4, 0)))
		{
#line 90
			*output << (((Complex<double>(0, (i* d)))- 1));*output << std::endl;
		}
#line 91
		if ((Core::toString(item))== "a")
		{
#line 92
			*output << "aaa" << std::endl;
			item = static_cast<uint32_t>(64);
			*output << (Core::toString(item));*output << std::endl;
#line 96
		}
	}
#line 96
	std::string v3 = "hello";
	int32_t id7 = 12;
#line 99
	*output << "v3:  " << (v3);*output << std::endl;
#line 102
	auto const tmp__11__ = id7;
#line 102
	switch (tmp__11__)
	{
#line 102
	case 12:
	case 24:
	{
#line 104
		*output << "hi" << std::endl;
		id7 = 2;
		break;
	}
#line 106
	case 16:
	{
#line 107
		*output << "hello" << std::endl;
		break;
	}
	default:
	{
#line 109
		*output << "else" << std::endl;
		break;
	}
	}
#line 112
	auto const tmp__12__ = v3;
#line 112
	if ((tmp__12__ == "hi")

					 || (tmp__12__ == "hello"))
	{
#line 114
		*output << "hi" << std::endl;
		id7 = 2;
	}
#line 116
	else if ((tmp__12__ == Core::upperCased(v3)))
	{
#line 117
		*output << "hello" << std::endl;
	}
	else
	{
#line 119
		*output << std::endl;
	}
#line 122
	auto const tmp__13__ = id7;
#line 126
	if ((tmp__13__ == (12* 34)+ id7)

					 || (tmp__13__ == 16))
	{
#line 128
		*output << "hello" << std::endl;
	}
	else
	{
#line 122
		switch (tmp__13__)
		{
#line 122
		case 12:
		case 2+ ((3/ 4)* 6):
		{
#line 124
			*output << "hi" << std::endl;
			id7 = 2;
			break;
		}
		default:
		{
			*output << "else" << std::endl;
			break;
		}
		}
	}
}
