#include "C:\Users\ruud\source\repos\Nate\Nate\core\Core.h"
#include "C:\Users\ruud\source\repos\Nate\Nate\input\created\Hello.h"
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
#line 8 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\core\\Data-input.ns"
#include <sstream>
#line 13 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\input\\nate.in"
struct shop
{
	shop()
		: beer_inventory(255)
		, name("shop")
	{}
#line 14
	int32_t beer_inventory;
	std::string name;
};

#line 17
bool E_int_32_in_E_shop_or_on_wall_(int32_t bottles, const shop& store)
{
#line 18
	return ((store. beer_inventory)+ bottles)> 0;
}

#define NOMINMAX
#include <windows.h>
#line 20
int main(int argc, char** argv)
{
	output = std::shared_ptr<std::ostream>(&std::cout, [](void*) {});
	error = std::shared_ptr<std::ostream>(&std::cerr, [](void*) {});
	input = std::shared_ptr<std::istream>(&std::cin, [](void*) {});
	SetConsoleOutputCP(65001);
#line 21
	float x = {};
#line 21
	float z = {};
#line 21
	float a__2081_ = {};
	int32_t d = 5;
#line 22
	int32_t g = d;
	double f = 4.4;
	bool tr = true;
	x = 	a__2081_ = static_cast<float>(3);
#line 27
	struct rec_a
	{
		rec_a()
			: beer_inventory(255)
			, name("shop")
			, record()
		{}
#line 28
		int32_t beer_inventory;
		std::string name;
		shop record;
	};

#line 32
	shop store = {};
	int32_t bottles = 44;
	*output << std::boolalpha ; *output << ((E_int_32_in_E_shop_or_on_wall_(bottles, store))); *output << std::endl; 
*input >> std::boolalpha >> tr >> std::noboolalpha >> std::skipws >> x >> std::noskipws >> z >> std::skipws;
*input >> d;
(*input).ignore(std::numeric_limits<std::streamsize>::max(), '\n');
#line 37
	*output << "Hello, world " << 3; *output << std::endl; 
	*output << (x); *output << (d); *output << (f); *output << " "; 
	*output << std::endl; 
#line 41
	*error << "Hello, world " << 3; *error << std::endl; 
	*error << (x); *error << (d); *error << (f); *error << " "; 
	*error << std::endl; 
#line 45
	if (d< 3)
	{
#line 46
		d = 4;
#line 45
	}
#line 48
	if ((d< 3)|| (d> 6))
	{
#line 49
		d = 5;
	}
#line 50
	else
	{
#line 51
		*output << (d); *output << std::endl; 
#line 48
	}
#line 53
	if (tr)
	{
#line 54
		tr = false;
	}
#line 55
	else if (!tr)
	{
#line 56
		if (tr)
		{
#line 57
			tr = false;
		}
#line 58
		else if (!tr)
		{
#line 59
			tr = true;
#line 56
		}
	}
#line 60
	else if (d> 3)
	{
#line 61
		tr = tr;
	}
#line 62
	else
	{
#line 63
		*output << std::boolalpha ; *output << (tr); *output << std::endl; 
#line 53
	}
#line 65
	{
		if (tr)
		{
#line 67
			tr = false;
		}
#line 68
		else if (!tr)
		{
#line 69
			tr = true;
#line 66
		}
#line 71
		int32_t count = 3;
		while (true)
		{
#line 72
			if (!(count> 0)) break;
			*output << (count); *output << std::endl; 
			count = count- 1;
		}
		count = 5;
		while (true)
		{
#line 78
			count = count- 1;
			if (!(count> 0)) break;
			*output << (("out: "+ (std::to_string(count)))); 
		}
#line 81
		*output << std::endl; 
#line 83
		for (int32_t ind= 3;ind<=5; ind+=1)
		{
#line 84
			*output << "out:" << (ind); *output << " " << (ind); *output << (ind); *output << " "; 
		}
#line 85
		*output << std::endl; 
#line 87
		for (int32_t ind= 3;ind<=20; ind+=1)
		{
#line 88
			if (!(count> 0)) break;
			*output << (ind); *output << std::endl; 
		}
	}
#line 91
	for (int8_t ind= 3;ind<=20; ind+=4)
	{
#line 92
		*output << static_cast<int>(ind); *output << std::endl; 
		if (!(ind< 15)) break;
		*output << ((std::pow(ind, 2))); *output << std::endl; 
	}
	for (int8_t ind= 3;ind<=20; ind+=4)
	{
#line 96
		if (!(ind< 15)) break;
		*output << static_cast<int>(ind); *output << std::endl; 
	}
	for (int8_t ind= 29;ind >= 3; ind -= 3)
	{
#line 99
		if (!(ind> 15)) break;
		*output << static_cast<int>(ind); *output << std::endl; 
	}
	std::list<std::string> list_1 = {};
	list_1.push_back("Hi there");
	list_1.push_back((std::to_string(666)));
	list_1.insert(list_1.cbegin(), "Hello");
	*output << ((static_cast<int32_t>(list_1.size()))); *output << " " << ((static_cast<int32_t>(list_1.size()))); *output << std::endl; 
	auto const& tmp__17__ = list_1;
	for (auto tmp__18__ = tmp__17__.cbegin(); tmp__18__ != tmp__17__.cend(); ++tmp__18__)
	{
		auto const& item = *tmp__18__;
#line 109
		*output << (item); *output << std::endl; 
	}
	int32_t id_1 = 3;
	const std::string tmp__20__ = "abcd⅑⅛⅐efg";
	auto tmp__22__ = tmp__20__.cbegin();
	for (auto tmp__21__ = tmp__20__.cbegin(); tmp__21__ != tmp__20__.cend(); tmp__21__=tmp__22__)
	{
		uint32_t item = utf8::next(tmp__22__,tmp__20__.cend());
#line 113
		*output << (Core::toString(item)); *output << std::endl; 
		if ((static_cast<double>(id_1))== 3.4)
		{
#line 115
			*output << (id_1); *output << std::endl; 
#line 114
		}
#line 116
		if ((Core::toString(item))== "a")
		{
#line 117
			*output << "aaa" << std::endl; 
			item = static_cast<uint32_t>(64);
			*output << (Core::toString(item)); *output << std::endl; 
#line 116
		}
	}
#line 121
	std::string v3 = "hello";
	int32_t id7 = 12;
#line 124
	*output << "v3:  " << (v3); *output << std::endl; 
#line 127
	auto const tmp__13__ = id7;
#line 127
	switch (tmp__13__)
	{
#line 127
	case 12:
	case 24:
	{
#line 129
		*output << "hi" << std::endl; 
		id7 = 2;
		break;
	}
#line 131
	case 16:
	{
#line 132
		*output << "hello" << std::endl; 
		break;
	}
	default:
	{
#line 134
		*output << "else" << std::endl; 
		break;
	}
	}
#line 137
	auto const tmp__14__ = v3;
#line 137
	if ((tmp__14__ == "hi")

					 || (tmp__14__ == "hello"))
	{
#line 139
		*output << "hi" << std::endl; 
		id7 = 2;
	}
#line 141
	else if ((tmp__14__ == Core::upperCased(v3)))
	{
#line 142
		*output << "hello" << std::endl; 
	}
	else
	{
#line 144
		*output << std::endl; 
	}
#line 147
	auto const tmp__15__ = id7;
#line 151
	if ((tmp__15__ == (12* 34)+ id7)

					 || (tmp__15__ == 16))
	{
#line 153
		*output << "hello" << std::endl; 
	}
	else
	{
#line 147
		switch (tmp__15__)
		{
#line 147
		case 12:
		case 2+ ((3/ 4)* 6):
		{
#line 149
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
#line 157
	{
		std::ostringstream data1_temp;
#line 158
		data1_temp << 1; data1_temp << " " << 2; data1_temp << " sdsddsaaa "; 
		data1_temp << 3; data1_temp << " " << 4; data1_temp << " "; 
		data1_temp << "hi " << ((4* 5)); data1_temp << std::endl; 
		const std::string data1= data1_temp.str();
#line 163
		std::ostringstream data2_temp;
#line 163
		data2_temp << 1; data2_temp << (data1); data2_temp << std::endl; 
		const std::string data2= data2_temp.str();
#line 166
		*output << (data1); *output << std::endl; 
		*output << (data2); *output << std::endl; 
#line 169
		std::shared_ptr<std::istringstream> in = {};
#line 169
		std::shared_ptr<std::istringstream> in2 = {};
		in.reset(new std::istringstream(data1));
#line 172
		int32_t x2 = {};
#line 172
		int32_t y = {};
		std::string s = {};
		std::shared_ptr<std::istream> nate__reader = in;
#line 174
*nate__reader >> x2 >> std::skipws >> y >> std::skipws >> s;
(*nate__reader).ignore(std::numeric_limits<std::streamsize>::max(), '\n');
#line 175
		*output << (x2); *output << std::endl; 
		*output << (y); *output << std::endl; 
		*output << "*" << (s); *output << "*" << std::endl; 
*nate__reader >> x2 >> std::skipws >> y >> std::skipws >> s;
(*nate__reader).ignore(std::numeric_limits<std::streamsize>::max(), '\n');
#line 179
		*output << (x2); *output << std::endl; 
		*output << (y); *output << std::endl; 
		*output << "*" << (s); *output << "*" << std::endl; 
	}
	{
		std::shared_ptr<std::ofstream> out = {};
		std::shared_ptr<std::ostream> out2 = out;
		*output << "Before: " << std::boolalpha ; *output << (((out&& out->good()))); *output << " " << std::boolalpha ; *output << (((out&& out->is_open()))); *output << std::endl; 
		out.reset(new std::ofstream("d:\\uit", std::ios::out));
		out2 = out;
		*output << std::endl; 
		if ((out&& out->is_open()))
		{
#line 191
			*output << "Inside: " << std::boolalpha ; *output << (((out&& out->good()))); *output << " " << std::boolalpha ; *output << (((out&& out->is_open()))); *output << std::endl; 
			std::shared_ptr<std::ostream> nate__writer = out2;
#line 192
			*nate__writer << "Hello,planet "; 
			*nate__writer << ((Core::formatted(5.4, Core::Format{8, 1, 32, 265} ))); *nate__writer << " "; 
			*nate__writer << 45; *nate__writer << " " << 56; *nate__writer << " "; 
			nate__writer = output;
#line 195
			*nate__writer << "Hi "; 
			*output << "Hi " << 45; *output << std::endl; 
			*nate__writer << "End" << std::endl; 
			*nate__writer << std::endl; 
			nate__writer = out2;
#line 199
			*nate__writer << "Hi" << std::endl; 
			if (out) out->close();
#line 190
		}
#line 201
		*output << "After: " << std::boolalpha ; *output << (((out&& out->good()))); *output << " " << std::boolalpha ; *output << (((out&& out->is_open()))); *output << std::endl; 
	}
}
