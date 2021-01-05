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
#line 78 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\input\\nate.in"
	list_1.push_back("Hi there");
	list_1.push_back((std::to_string(666)));
#line 80
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
	if (tr)
	{
#line 42
		tr = false;
	}
#line 43
	else if (!tr)
	{
#line 44
		tr = true;
#line 46
	}
#line 46
	int32_t count = 3;
	while (true)
	{
#line 47
		if (!(count> 0)) break;
		*output << (count);*output << std::endl;
		count = count- 1;
	}
	count = 5;
	while (true)
	{
#line 53
		count = count- 1;
		if (!(count> 0)) break;
		*output << (("out: "+ (std::to_string(count))));
	}
#line 56
	*output << std::endl;
#line 58
	for (int32_t ind= 3;ind<=5;ind+=1)
	{
#line 59
		*output << "out:" << (ind);*output << " " << (ind);*output << (ind);*output << " ";
	}
#line 60
	*output << std::endl;
#line 62
	for (int32_t ind= 3;ind<=20;ind+=1)
	{
#line 63
		if (!(count> 0)) break;
		*output << (ind);*output << std::endl;
	}
	for (int8_t ind= 3;ind<=20;ind+=4)
	{
#line 67
		*output << static_cast<int>(ind);*output << std::endl;
		if (!(ind< 15)) break;
		*output << ((std::pow(ind, 2)));*output << std::endl;
	}
	for (int8_t ind= 3;ind<=20;ind+=4)
	{
#line 71
		if (!(ind< 15)) break;
		*output << static_cast<int>(ind);*output << std::endl;
	}
	for (int8_t ind= 29;ind >= 3;ind -= 3)
	{
#line 74
		if (!(ind> 15)) break;
		*output << static_cast<int>(ind);*output << std::endl;
	}
	std::list<std::string> list_1 = {};
#line 81
	*output << ((static_cast<int32_t>(list_1.size())));*output << " " << ((static_cast<int32_t>(list_1.size())));*output << std::endl;
	auto const& tmp__11__ = list_1;
	for (auto tmp__12__ = tmp__11__.cbegin(); tmp__12__ != tmp__11__.cend(); ++tmp__12__)
	{
		auto const& item = *tmp__12__;
#line 84
		*output << (item);*output << std::endl;
	}
	const std::string tmp__14__ = "abcd⅑⅛⅐efg";
	auto tmp__16__ = tmp__14__.cbegin();
	for (auto tmp__15__ = tmp__14__.cbegin(); tmp__15__ != tmp__14__.cend(); tmp__15__=tmp__16__)
	{
		uint32_t item = utf8::next(tmp__16__,tmp__14__.cend());
#line 87
		*output << (Core::toString(item));*output << std::endl;
		if (((Complex<double>(0, (i* d)))- 1)== (Complex<double>(3.4, 0)))
		{
#line 89
			*output << (((Complex<double>(0, (i* d)))- 1));*output << std::endl;
		}
#line 90
		if ((Core::toString(item))== "a")
		{
#line 91
			*output << "aaa" << std::endl;
			item = static_cast<uint32_t>(64);
			*output << (Core::toString(item));*output << std::endl;
#line 604
		}
	}
}
