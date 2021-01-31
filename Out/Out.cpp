#include "C:\Users\ruud\source\repos\Nate\Nate\core\Core.h"
	std::shared_ptr<std::ostream> output = {};
	std::shared_ptr<std::ostream> error = {};
	std::shared_ptr<std::istream> input = {};
#line 86 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\input\\nate.in"
		list_1.push_back("Hi there");
		list_1.push_back((std::to_string(666)));
#line 88
		list_1.insert(list_1.cbegin(), "Hello");
#line 156
			out.reset(new std::ofstream("d:\\uit", std::ios::out));
#line 169
				if (out) out->close();
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
#define NOMINMAX
#include <windows.h>
#line 11 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\input\\nate.in"
int main(int argc, char** argv)
{
	output = std::shared_ptr<std::ostream>(&std::cout, [](void*) {});
	error = std::shared_ptr<std::ostream>(&std::cerr, [](void*) {});
	input = std::shared_ptr<std::istream>(&std::cin, [](void*) {});
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
#line 18
*input>> std::boolalpha>> tr>> std::noboolalpha>> std::skipws>> x>> std::noskipws>> z>> std::skipws;
*input>> d;
(*input).ignore(std::numeric_limits<std::streamsize>::max(), '\n');
#line 20
	*output << "Hello, world " << 3;*output << std::endl;
	*output << (x);*output << (d);*output << (f);*output << " ";
	*output << std::endl;
#line 24
	*error << "Hello, world " << 3;*error << std::endl;
	*error << (x);*error << (d);*error << (f);*error << " ";
	*error << std::endl;
#line 28
	if (d< 3)
	{
#line 29
		d = 4;
#line 28
	}
#line 31
	if ((d< 3)|| (d> 6))
	{
#line 32
		d = 5;
	}
#line 33
	else
	{
#line 34
		*output << (d);*output << std::endl;
#line 31
	}
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
		if (tr)
		{
#line 40
			tr = false;
		}
#line 41
		else if (!tr)
		{
#line 42
			tr = true;
#line 39
		}
	}
#line 43
	else if (d> 3)
	{
#line 44
		tr = tr;
	}
#line 45
	else
	{
#line 46
		*output << std::boolalpha ;*output << (tr);*output << std::endl;
#line 36
	}
#line 48
	{
		if (tr)
		{
#line 50
			tr = false;
		}
#line 51
		else if (!tr)
		{
#line 52
			tr = true;
#line 49
		}
#line 54
		int32_t count = 3;
		while (true)
		{
#line 55
			if (!(count> 0)) break;
			*output << (count);*output << std::endl;
			count = count- 1;
		}
		count = 5;
		while (true)
		{
#line 61
			count = count- 1;
			if (!(count> 0)) break;
			*output << (("out: "+ (std::to_string(count))));
		}
#line 64
		*output << std::endl;
#line 66
		for (int32_t ind= 3;ind<=5;ind+=1)
		{
#line 67
			*output << "out:" << (ind);*output << " " << (ind);*output << (ind);*output << " ";
		}
#line 68
		*output << std::endl;
#line 70
		for (int32_t ind= 3;ind<=20;ind+=1)
		{
#line 71
			if (!(count> 0)) break;
			*output << (ind);*output << std::endl;
		}
	}
#line 74
	for (int8_t ind= 3;ind<=20;ind+=4)
	{
#line 75
		*output << static_cast<int>(ind);*output << std::endl;
		if (!(ind< 15)) break;
		*output << ((std::pow(ind, 2)));*output << std::endl;
	}
	for (int8_t ind= 3;ind<=20;ind+=4)
	{
#line 79
		if (!(ind< 15)) break;
		*output << static_cast<int>(ind);*output << std::endl;
	}
	for (int8_t ind= 29;ind >= 3;ind -= 3)
	{
#line 82
		if (!(ind> 15)) break;
		*output << static_cast<int>(ind);*output << std::endl;
	}
	std::list<std::string> list_1 = {};
#line 89
	*output << ((static_cast<int32_t>(list_1.size())));*output << " " << ((static_cast<int32_t>(list_1.size())));*output << std::endl;
	auto const& tmp__15__ = list_1;
	for (auto tmp__16__ = tmp__15__.cbegin(); tmp__16__ != tmp__15__.cend(); ++tmp__16__)
	{
		auto const& item = *tmp__16__;
#line 92
		*output << (item);*output << std::endl;
	}
	int32_t id_1 = 3;
	const std::string tmp__18__ = "abcd⅑⅛⅐efg";
	auto tmp__20__ = tmp__18__.cbegin();
	for (auto tmp__19__ = tmp__18__.cbegin(); tmp__19__ != tmp__18__.cend(); tmp__19__=tmp__20__)
	{
		uint32_t item = utf8::next(tmp__20__,tmp__18__.cend());
#line 96
		*output << (Core::toString(item));*output << std::endl;
		if ((static_cast<double>(id_1))== 3.4)
		{
#line 98
			*output << (id_1);*output << std::endl;
#line 97
		}
#line 99
		if ((Core::toString(item))== "a")
		{
#line 100
			*output << "aaa" << std::endl;
			item = static_cast<uint32_t>(64);
			*output << (Core::toString(item));*output << std::endl;
#line 99
		}
	}
#line 104
	std::string v3 = "hello";
	int32_t id7 = 12;
#line 107
	*output << "v3:  " << (v3);*output << std::endl;
#line 110
	auto const tmp__11__ = id7;
#line 110
	switch (tmp__11__)
	{
#line 110
	case 12:
	case 24:
	{
#line 112
		*output << "hi" << std::endl;
		id7 = 2;
		break;
	}
#line 114
	case 16:
	{
#line 115
		*output << "hello" << std::endl;
		break;
	}
	default:
	{
#line 117
		*output << "else" << std::endl;
		break;
	}
	}
#line 120
	auto const tmp__12__ = v3;
#line 120
	if ((tmp__12__ == "hi")

					 || (tmp__12__ == "hello"))
	{
#line 122
		*output << "hi" << std::endl;
		id7 = 2;
	}
#line 124
	else if ((tmp__12__ == Core::upperCased(v3)))
	{
#line 125
		*output << "hello" << std::endl;
	}
	else
	{
#line 127
		*output << std::endl;
	}
#line 130
	auto const tmp__13__ = id7;
#line 134
	if ((tmp__13__ == (12* 34)+ id7)

					 || (tmp__13__ == 16))
	{
#line 136
		*output << "hello" << std::endl;
	}
	else
	{
#line 130
		switch (tmp__13__)
		{
#line 130
		case 12:
		case 2+ ((3/ 4)* 6):
		{
#line 132
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
#line 140
	{
		std::ostringstream data1_temp;
#line 141
		data1_temp << 1;data1_temp << " " << 2;data1_temp << " sdsddsaaa" << std::endl;
		data1_temp << 3;data1_temp << " " << 4;data1_temp << " ";
		data1_temp << "hi " << ((4* 5));data1_temp << std::endl;
		const std::string data1= data1_temp.str();
#line 146
		std::ostringstream data2_temp;
#line 146
		data2_temp << 1;data2_temp << (data1);data2_temp << std::endl;
		const std::string data2= data2_temp.str();
#line 149
		*output << (data1);*output << std::endl;
		*output << (data2);*output << std::endl;
	}
	{
		std::shared_ptr<std::ofstream> out = {};
		std::shared_ptr<std::ostream> out2 = out;
		*output << "Before: " << std::boolalpha ;*output << (((out&& out->good())));*output << " " << std::boolalpha ;*output << (((out&& out->is_open())));*output << std::endl;
#line 157
		out2 = out;
		*output << std::endl;
		if ((out&& out->is_open()))
		{
#line 160
			*output << "Inside: " << std::boolalpha ;*output << (((out&& out->good())));*output << " " << std::boolalpha ;*output << (((out&& out->is_open())));*output << std::endl;
			std::shared_ptr<std::ostream> nate__writer = out2;
#line 161
			*nate__writer << "Hello,planet ";
			*nate__writer << ((Core::formatted(5.4, Core::Format{8, 1, 32, 265} )));*nate__writer << " ";
			*nate__writer << 45;*nate__writer << " " << 56;*nate__writer << " ";
			nate__writer = output;
#line 164
			*nate__writer << "Hi ";
			*output << "Hi " << 45;*output << std::endl;
			*nate__writer << "End" << std::endl;
			*nate__writer << std::endl;
			nate__writer = out2;
#line 168
			*nate__writer << "Hi" << std::endl;
#line 159
		}
#line 170
		*output << "After: " << std::boolalpha ;*output << (((out&& out->good())));*output << " " << std::boolalpha ;*output << (((out&& out->is_open())));*output << std::endl;
	}
}
