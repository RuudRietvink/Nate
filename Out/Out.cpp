#include "C:\Users\ruud\source\repos\Nate\Nate\core\Core.h"
#line 95 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\input\\nate.in"
		list_1.push_back("Hi there");
		list_1.push_back((std::to_string(666)));
#line 97
		list_1.insert(list_1.cbegin(), "Hello");
#line 165
			out.reset(new std::ofstream("d:\\uit", std::ios::out));
#line 178
				if (out) out->close();
#line 1 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\input\\nate.in"
std::shared_ptr<std::ostream> output;
#line 1
std::shared_ptr<std::ostream> error;
#line 1
std::shared_ptr<std::istream> input;
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
	struct shop
	{
		shop()
			: beer_inventory(255)
			, name("shop")
		{}
#line 19
		int32_t beer_inventory;
		std::string name;
	};
	struct rec_a
	{
		rec_a()
			: beer_inventory(255)
			, name("shop")
			, record()
		{}
#line 23
		int32_t beer_inventory;
		std::string name;
		shop record;
	};
*input >> std::boolalpha >> tr >> std::noboolalpha >> std::skipws >> x >> std::noskipws >> z >> std::skipws;
*input >> d;
(*input).ignore(std::numeric_limits<std::streamsize>::max(), '\n');
#line 29
	*output << "Hello, world " << 3; *output << std::endl; 
	*output << (x); *output << (d); *output << (f); *output << " "; 
	*output << std::endl; 
#line 33
	*error << "Hello, world " << 3; *error << std::endl; 
	*error << (x); *error << (d); *error << (f); *error << " "; 
	*error << std::endl; 
#line 37
	if (d< 3)
	{
#line 38
		d = 4;
#line 37
	}
#line 40
	if ((d< 3)|| (d> 6))
	{
#line 41
		d = 5;
	}
#line 42
	else
	{
#line 43
		*output << (d); *output << std::endl; 
#line 40
	}
#line 45
	if (tr)
	{
#line 46
		tr = false;
	}
#line 47
	else if (!tr)
	{
#line 48
		if (tr)
		{
#line 49
			tr = false;
		}
#line 50
		else if (!tr)
		{
#line 51
			tr = true;
#line 48
		}
	}
#line 52
	else if (d> 3)
	{
#line 53
		tr = tr;
	}
#line 54
	else
	{
#line 55
		*output << std::boolalpha ; *output << (tr); *output << std::endl; 
#line 45
	}
#line 57
	{
		if (tr)
		{
#line 59
			tr = false;
		}
#line 60
		else if (!tr)
		{
#line 61
			tr = true;
#line 58
		}
#line 63
		int32_t count = 3;
		while (true)
		{
#line 64
			if (!(count> 0)) break;
			*output << (count); *output << std::endl; 
			count = count- 1;
		}
		count = 5;
		while (true)
		{
#line 70
			count = count- 1;
			if (!(count> 0)) break;
			*output << (("out: "+ (std::to_string(count)))); 
		}
#line 73
		*output << std::endl; 
#line 75
		for (int32_t ind= 3;ind<=5; ind+=1)
		{
#line 76
			*output << "out:" << (ind); *output << " " << (ind); *output << (ind); *output << " "; 
		}
#line 77
		*output << std::endl; 
#line 79
		for (int32_t ind= 3;ind<=20; ind+=1)
		{
#line 80
			if (!(count> 0)) break;
			*output << (ind); *output << std::endl; 
		}
	}
#line 83
	for (int8_t ind= 3;ind<=20; ind+=4)
	{
#line 84
		*output << static_cast<int>(ind); *output << std::endl; 
		if (!(ind< 15)) break;
		*output << ((std::pow(ind, 2))); *output << std::endl; 
	}
	for (int8_t ind= 3;ind<=20; ind+=4)
	{
#line 88
		if (!(ind< 15)) break;
		*output << static_cast<int>(ind); *output << std::endl; 
	}
	for (int8_t ind= 29;ind >= 3; ind -= 3)
	{
#line 91
		if (!(ind> 15)) break;
		*output << static_cast<int>(ind); *output << std::endl; 
	}
	std::list<std::string> list_1 = {};
#line 98
	*output << ((static_cast<int32_t>(list_1.size()))); *output << " " << ((static_cast<int32_t>(list_1.size()))); *output << std::endl; 
	auto const& tmp__15__ = list_1;
	for (auto tmp__16__ = tmp__15__.cbegin(); tmp__16__ != tmp__15__.cend(); ++tmp__16__)
	{
		auto const& item = *tmp__16__;
#line 101
		*output << (item); *output << std::endl; 
	}
	int32_t id_1 = 3;
	const std::string tmp__18__ = "abcd⅑⅛⅐efg";
	auto tmp__20__ = tmp__18__.cbegin();
	for (auto tmp__19__ = tmp__18__.cbegin(); tmp__19__ != tmp__18__.cend(); tmp__19__=tmp__20__)
	{
		uint32_t item = utf8::next(tmp__20__,tmp__18__.cend());
#line 105
		*output << (Core::toString(item)); *output << std::endl; 
		if ((static_cast<double>(id_1))== 3.4)
		{
#line 107
			*output << (id_1); *output << std::endl; 
#line 106
		}
#line 108
		if ((Core::toString(item))== "a")
		{
#line 109
			*output << "aaa" << std::endl; 
			item = static_cast<uint32_t>(64);
			*output << (Core::toString(item)); *output << std::endl; 
#line 108
		}
	}
#line 113
	std::string v3 = "hello";
	int32_t id7 = 12;
#line 116
	*output << "v3:  " << (v3); *output << std::endl; 
#line 119
	auto const tmp__11__ = id7;
#line 119
	switch (tmp__11__)
	{
#line 119
	case 12:
	case 24:
	{
#line 121
		*output << "hi" << std::endl; 
		id7 = 2;
		break;
	}
#line 123
	case 16:
	{
#line 124
		*output << "hello" << std::endl; 
		break;
	}
	default:
	{
#line 126
		*output << "else" << std::endl; 
		break;
	}
	}
#line 129
	auto const tmp__12__ = v3;
#line 129
	if ((tmp__12__ == "hi")

					 || (tmp__12__ == "hello"))
	{
#line 131
		*output << "hi" << std::endl; 
		id7 = 2;
	}
#line 133
	else if ((tmp__12__ == Core::upperCased(v3)))
	{
#line 134
		*output << "hello" << std::endl; 
	}
	else
	{
#line 136
		*output << std::endl; 
	}
#line 139
	auto const tmp__13__ = id7;
#line 143
	if ((tmp__13__ == (12* 34)+ id7)

					 || (tmp__13__ == 16))
	{
#line 145
		*output << "hello" << std::endl; 
	}
	else
	{
#line 139
		switch (tmp__13__)
		{
#line 139
		case 12:
		case 2+ ((3/ 4)* 6):
		{
#line 141
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
#line 149
	{
		std::ostringstream data1_temp;
#line 150
		data1_temp << 1; data1_temp << " " << 2; data1_temp << " sdsddsaaa" << std::endl; 
		data1_temp << 3; data1_temp << " " << 4; data1_temp << " "; 
		data1_temp << "hi " << ((4* 5)); data1_temp << std::endl; 
		const std::string data1= data1_temp.str();
#line 155
		std::ostringstream data2_temp;
#line 155
		data2_temp << 1; data2_temp << (data1); data2_temp << std::endl; 
		const std::string data2= data2_temp.str();
#line 158
		*output << (data1); *output << std::endl; 
		*output << (data2); *output << std::endl; 
	}
	{
		std::shared_ptr<std::ofstream> out = {};
		std::shared_ptr<std::ostream> out2 = out;
		*output << "Before: " << std::boolalpha ; *output << (((out&& out->good()))); *output << " " << std::boolalpha ; *output << (((out&& out->is_open()))); *output << std::endl; 
#line 166
		out2 = out;
		*output << std::endl; 
		if ((out&& out->is_open()))
		{
#line 169
			*output << "Inside: " << std::boolalpha ; *output << (((out&& out->good()))); *output << " " << std::boolalpha ; *output << (((out&& out->is_open()))); *output << std::endl; 
			std::shared_ptr<std::ostream> nate__writer = out2;
#line 170
			*nate__writer << "Hello,planet "; 
			*nate__writer << ((Core::formatted(5.4, Core::Format{8, 1, 32, 265} ))); *nate__writer << " "; 
			*nate__writer << 45; *nate__writer << " " << 56; *nate__writer << " "; 
			nate__writer = output;
#line 173
			*nate__writer << "Hi "; 
			*output << "Hi " << 45; *output << std::endl; 
			*nate__writer << "End" << std::endl; 
			*nate__writer << std::endl; 
			nate__writer = out2;
#line 177
			*nate__writer << "Hi" << std::endl; 
#line 168
		}
#line 179
		*output << "After: " << std::boolalpha ; *output << (((out&& out->good()))); *output << " " << std::boolalpha ; *output << (((out&& out->is_open()))); *output << std::endl; 
	}
}
