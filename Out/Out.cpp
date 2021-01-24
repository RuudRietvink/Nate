#include "C:\Users\ruud\source\repos\Nate\Nate\core\Core.h"
std::shared_ptr<std::ostream> output = {};
std::shared_ptr<std::ostream> error = {};
#line 83 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\input\\nate.in"
	list_1.push_back("Hi there");
	list_1.push_back((std::to_string(666)));
#line 85
	list_1.insert(list_1.cbegin(), "Hello");
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
	*error << "Hello, world " << 3;*error << std::endl;
	*error << (x);*error << (d);*error << (f);*error << " ";
	*error << std::endl;
#line 25
	if (d< 3)
	{
#line 26
		d = 4;
#line 25
	}
#line 28
	if ((d< 3)|| (d> 6))
	{
#line 29
		d = 5;
	}
#line 30
	else
	{
#line 31
		*output << (d);*output << std::endl;
#line 28
	}
#line 33
	if (tr)
	{
#line 34
		tr = false;
	}
#line 35
	else if (!tr)
	{
#line 36
		if (tr)
		{
#line 37
			tr = false;
		}
#line 38
		else if (!tr)
		{
#line 39
			tr = true;
#line 36
		}
	}
#line 40
	else if (d> 3)
	{
#line 41
		tr = tr;
	}
#line 42
	else
	{
#line 43
		*output << std::boolalpha ;*output << (tr);*output << std::endl;
#line 33
	}
#line 45
	{
		if (tr)
		{
#line 47
			tr = false;
		}
#line 48
		else if (!tr)
		{
#line 49
			tr = true;
#line 46
		}
#line 51
		int32_t count = 3;
		while (true)
		{
#line 52
			if (!(count> 0)) break;
			*output << (count);*output << std::endl;
			count = count- 1;
		}
		count = 5;
		while (true)
		{
#line 58
			count = count- 1;
			if (!(count> 0)) break;
			*output << (("out: "+ (std::to_string(count))));
		}
#line 61
		*output << std::endl;
#line 63
		for (int32_t ind= 3;ind<=5;ind+=1)
		{
#line 64
			*output << "out:" << (ind);*output << " " << (ind);*output << (ind);*output << " ";
		}
#line 65
		*output << std::endl;
#line 67
		for (int32_t ind= 3;ind<=20;ind+=1)
		{
#line 68
			if (!(count> 0)) break;
			*output << (ind);*output << std::endl;
		}
	}
#line 71
	for (int8_t ind= 3;ind<=20;ind+=4)
	{
#line 72
		*output << static_cast<int>(ind);*output << std::endl;
		if (!(ind< 15)) break;
		*output << ((std::pow(ind, 2)));*output << std::endl;
	}
	for (int8_t ind= 3;ind<=20;ind+=4)
	{
#line 76
		if (!(ind< 15)) break;
		*output << static_cast<int>(ind);*output << std::endl;
	}
	for (int8_t ind= 29;ind >= 3;ind -= 3)
	{
#line 79
		if (!(ind> 15)) break;
		*output << static_cast<int>(ind);*output << std::endl;
	}
	std::list<std::string> list_1 = {};
#line 86
	*output << ((static_cast<int32_t>(list_1.size())));*output << " " << ((static_cast<int32_t>(list_1.size())));*output << std::endl;
	auto const& tmp__14__ = list_1;
	for (auto tmp__15__ = tmp__14__.cbegin(); tmp__15__ != tmp__14__.cend(); ++tmp__15__)
	{
		auto const& item = *tmp__15__;
#line 89
		*output << (item);*output << std::endl;
	}
	int32_t id_1 = 3;
	const std::string tmp__17__ = "abcd⅑⅛⅐efg";
	auto tmp__19__ = tmp__17__.cbegin();
	for (auto tmp__18__ = tmp__17__.cbegin(); tmp__18__ != tmp__17__.cend(); tmp__18__=tmp__19__)
	{
		uint32_t item = utf8::next(tmp__19__,tmp__17__.cend());
#line 93
		*output << (Core::toString(item));*output << std::endl;
		if ((static_cast<double>(id_1))== 3.4)
		{
#line 95
			*output << (id_1);*output << std::endl;
#line 94
		}
#line 96
		if ((Core::toString(item))== "a")
		{
#line 97
			*output << "aaa" << std::endl;
			item = static_cast<uint32_t>(64);
			*output << (Core::toString(item));*output << std::endl;
#line 96
		}
	}
#line 101
	std::string v3 = "hello";
	int32_t id7 = 12;
#line 104
	*output << "v3:  " << (v3);*output << std::endl;
#line 107
	auto const tmp__11__ = id7;
#line 107
	switch (tmp__11__)
	{
#line 107
	case 12:
	case 24:
	{
#line 109
		*output << "hi" << std::endl;
		id7 = 2;
		break;
	}
#line 111
	case 16:
	{
#line 112
		*output << "hello" << std::endl;
		break;
	}
	default:
	{
#line 114
		*output << "else" << std::endl;
		break;
	}
	}
#line 117
	auto const tmp__12__ = v3;
#line 117
	if ((tmp__12__ == "hi")

					 || (tmp__12__ == "hello"))
	{
#line 119
		*output << "hi" << std::endl;
		id7 = 2;
	}
#line 121
	else if ((tmp__12__ == Core::upperCased(v3)))
	{
#line 122
		*output << "hello" << std::endl;
	}
	else
	{
#line 124
		*output << std::endl;
	}
#line 127
	auto const tmp__13__ = id7;
#line 131
	if ((tmp__13__ == (12* 34)+ id7)

					 || (tmp__13__ == 16))
	{
#line 133
		*output << "hello" << std::endl;
	}
	else
	{
#line 127
		switch (tmp__13__)
		{
#line 127
		case 12:
		case 2+ ((3/ 4)* 6):
		{
#line 129
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
#line 137
	{
		std::ostringstream data1_temp;
#line 138
		data1_temp << 1;data1_temp << " " << 2;data1_temp << " sdsddsaaa" << std::endl;
		data1_temp << 3;data1_temp << " " << 4;data1_temp << " ";
		data1_temp << "hi " << ((4* 5));data1_temp << std::endl;
		const std::string data1= data1_temp.str();
#line 143
		std::ostringstream data2_temp;
#line 143
		data2_temp << 1;data2_temp << (data1);data2_temp << std::endl;
		const std::string data2= data2_temp.str();
#line 146
		*output << (data1);*output << std::endl;
		*output << (data2);*output << std::endl;
	}
}
