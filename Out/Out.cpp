#include "C:\Users\ruud\source\repos\Nate\Nate\core\Core.h"
	}

#line 101 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\input\\nate.in"
		list_1.push_back("Hi there");
		list_1.push_back((std::to_string(666)));
#line 103
		list_1.insert(list_1.cbegin(), "Hello");
#line 168
			in.reset(new std::istringstream(data1));
#line 185
			out.reset(new std::ofstream("d:\\uit", std::ios::out));
#line 198
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
#line 8 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\core\\Data-input.ns"
#include <sstream>
#line 11 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\input\\nate.in"
struct shop
{
	shop()
		: beer_inventory(255)
		, name("shop")
	{}
#line 12
	int32_t beer_inventory;
	std::string name;
};
bool E_int_32_in_E_shop_or_on_wall_(int32_t bottles, const shop& store)
{
#line 16
	return ((store. beer_inventory)+ bottles)> 0;
}
#define NOMINMAX
#include <windows.h>
#line 18
int main(int argc, char** argv)
{
	output = std::shared_ptr<std::ostream>(&std::cout, [](void*) {});
	error = std::shared_ptr<std::ostream>(&std::cerr, [](void*) {});
	input = std::shared_ptr<std::istream>(&std::cin, [](void*) {});
	SetConsoleOutputCP(65001);
#line 19
	float x = {};
#line 19
	float z = {};
#line 19
	float a__2081_ = {};
	int32_t d = 5;
#line 20
	int32_t g = d;
	double f = 4.4;
	bool tr = true;
	x = 	a__2081_ = static_cast<float>(3);
#line 25
	struct rec_a
	{
		rec_a()
			: beer_inventory(255)
			, name("shop")
			, record()
		{}
#line 26
		int32_t beer_inventory;
		std::string name;
		shop record;
	};
	shop store = {};
	int32_t bottles = 44;
	*output << std::boolalpha ; *output << ((E_int_32_in_E_shop_or_on_wall_(bottles, store))); *output << std::endl; 
*input >> std::boolalpha >> tr >> std::noboolalpha >> std::skipws >> x >> std::noskipws >> z >> std::skipws;
*input >> d;
(*input).ignore(std::numeric_limits<std::streamsize>::max(), '\n');
#line 35
	*output << "Hello, world " << 3; *output << std::endl; 
	*output << (x); *output << (d); *output << (f); *output << " "; 
	*output << std::endl; 
#line 39
	*error << "Hello, world " << 3; *error << std::endl; 
	*error << (x); *error << (d); *error << (f); *error << " "; 
	*error << std::endl; 
#line 43
	if (d< 3)
	{
#line 44
		d = 4;
#line 43
	}
#line 46
	if ((d< 3)|| (d> 6))
	{
#line 47
		d = 5;
	}
#line 48
	else
	{
#line 49
		*output << (d); *output << std::endl; 
#line 46
	}
#line 51
	if (tr)
	{
#line 52
		tr = false;
	}
#line 53
	else if (!tr)
	{
#line 54
		if (tr)
		{
#line 55
			tr = false;
		}
#line 56
		else if (!tr)
		{
#line 57
			tr = true;
#line 54
		}
	}
#line 58
	else if (d> 3)
	{
#line 59
		tr = tr;
	}
#line 60
	else
	{
#line 61
		*output << std::boolalpha ; *output << (tr); *output << std::endl; 
#line 51
	}
#line 63
	{
		if (tr)
		{
#line 65
			tr = false;
		}
#line 66
		else if (!tr)
		{
#line 67
			tr = true;
#line 64
		}
#line 69
		int32_t count = 3;
		while (true)
		{
#line 70
			if (!(count> 0)) break;
			*output << (count); *output << std::endl; 
			count = count- 1;
		}
		count = 5;
		while (true)
		{
#line 76
			count = count- 1;
			if (!(count> 0)) break;
			*output << (("out: "+ (std::to_string(count)))); 
		}
#line 79
		*output << std::endl; 
#line 81
		for (int32_t ind= 3;ind<=5; ind+=1)
		{
#line 82
			*output << "out:" << (ind); *output << " " << (ind); *output << (ind); *output << " "; 
		}
#line 83
		*output << std::endl; 
#line 85
		for (int32_t ind= 3;ind<=20; ind+=1)
		{
#line 86
			if (!(count> 0)) break;
			*output << (ind); *output << std::endl; 
		}
	}
#line 89
	for (int8_t ind= 3;ind<=20; ind+=4)
	{
#line 90
		*output << static_cast<int>(ind); *output << std::endl; 
		if (!(ind< 15)) break;
		*output << ((std::pow(ind, 2))); *output << std::endl; 
	}
	for (int8_t ind= 3;ind<=20; ind+=4)
	{
#line 94
		if (!(ind< 15)) break;
		*output << static_cast<int>(ind); *output << std::endl; 
	}
	for (int8_t ind= 29;ind >= 3; ind -= 3)
	{
#line 97
		if (!(ind> 15)) break;
		*output << static_cast<int>(ind); *output << std::endl; 
	}
	std::list<std::string> list_1 = {};
#line 104
	*output << ((static_cast<int32_t>(list_1.size()))); *output << " " << ((static_cast<int32_t>(list_1.size()))); *output << std::endl; 
	auto const& tmp__15__ = list_1;
	for (auto tmp__16__ = tmp__15__.cbegin(); tmp__16__ != tmp__15__.cend(); ++tmp__16__)
	{
		auto const& item = *tmp__16__;
#line 107
		*output << (item); *output << std::endl; 
	}
	int32_t id_1 = 3;
	const std::string tmp__18__ = "abcd⅑⅛⅐efg";
	auto tmp__20__ = tmp__18__.cbegin();
	for (auto tmp__19__ = tmp__18__.cbegin(); tmp__19__ != tmp__18__.cend(); tmp__19__=tmp__20__)
	{
		uint32_t item = utf8::next(tmp__20__,tmp__18__.cend());
#line 111
		*output << (Core::toString(item)); *output << std::endl; 
		if ((static_cast<double>(id_1))== 3.4)
		{
#line 113
			*output << (id_1); *output << std::endl; 
#line 112
		}
#line 114
		if ((Core::toString(item))== "a")
		{
#line 115
			*output << "aaa" << std::endl; 
			item = static_cast<uint32_t>(64);
			*output << (Core::toString(item)); *output << std::endl; 
#line 114
		}
	}
#line 119
	std::string v3 = "hello";
	int32_t id7 = 12;
#line 122
	*output << "v3:  " << (v3); *output << std::endl; 
#line 125
	auto const tmp__11__ = id7;
#line 125
	switch (tmp__11__)
	{
#line 125
	case 12:
	case 24:
	{
#line 127
		*output << "hi" << std::endl; 
		id7 = 2;
		break;
	}
#line 129
	case 16:
	{
#line 130
		*output << "hello" << std::endl; 
		break;
	}
	default:
	{
#line 132
		*output << "else" << std::endl; 
		break;
	}
	}
#line 135
	auto const tmp__12__ = v3;
#line 135
	if ((tmp__12__ == "hi")

					 || (tmp__12__ == "hello"))
	{
#line 137
		*output << "hi" << std::endl; 
		id7 = 2;
	}
#line 139
	else if ((tmp__12__ == Core::upperCased(v3)))
	{
#line 140
		*output << "hello" << std::endl; 
	}
	else
	{
#line 142
		*output << std::endl; 
	}
#line 145
	auto const tmp__13__ = id7;
#line 149
	if ((tmp__13__ == (12* 34)+ id7)

					 || (tmp__13__ == 16))
	{
#line 151
		*output << "hello" << std::endl; 
	}
	else
	{
#line 145
		switch (tmp__13__)
		{
#line 145
		case 12:
		case 2+ ((3/ 4)* 6):
		{
#line 147
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
#line 155
	{
		std::ostringstream data1_temp;
#line 156
		data1_temp << 1; data1_temp << " " << 2; data1_temp << " sdsddsaaa "; 
		data1_temp << 3; data1_temp << " " << 4; data1_temp << " "; 
		data1_temp << "hi " << ((4* 5)); data1_temp << std::endl; 
		const std::string data1= data1_temp.str();
#line 161
		std::ostringstream data2_temp;
#line 161
		data2_temp << 1; data2_temp << (data1); data2_temp << std::endl; 
		const std::string data2= data2_temp.str();
#line 164
		*output << (data1); *output << std::endl; 
		*output << (data2); *output << std::endl; 
#line 167
		std::shared_ptr<std::istringstream> in = {};
#line 167
		std::shared_ptr<std::istringstream> in2 = {};
#line 170
		int32_t x2 = {};
#line 170
		int32_t y = {};
		std::string s = {};
		std::shared_ptr<std::istream> nate__reader = in;
#line 172
*nate__reader >> x2 >> std::skipws >> y >> std::skipws >> s;
(*nate__reader).ignore(std::numeric_limits<std::streamsize>::max(), '\n');
#line 173
		*output << (x2); *output << std::endl; 
		*output << (y); *output << std::endl; 
		*output << "*" << (s); *output << "*" << std::endl; 
*nate__reader >> x2 >> std::skipws >> y >> std::skipws >> s;
(*nate__reader).ignore(std::numeric_limits<std::streamsize>::max(), '\n');
#line 177
		*output << (x2); *output << std::endl; 
		*output << (y); *output << std::endl; 
		*output << "*" << (s); *output << "*" << std::endl; 
	}
	{
		std::shared_ptr<std::ofstream> out = {};
		std::shared_ptr<std::ostream> out2 = out;
		*output << "Before: " << std::boolalpha ; *output << (((out&& out->good()))); *output << " " << std::boolalpha ; *output << (((out&& out->is_open()))); *output << std::endl; 
#line 186
		out2 = out;
		*output << std::endl; 
		if ((out&& out->is_open()))
		{
#line 189
			*output << "Inside: " << std::boolalpha ; *output << (((out&& out->good()))); *output << " " << std::boolalpha ; *output << (((out&& out->is_open()))); *output << std::endl; 
			std::shared_ptr<std::ostream> nate__writer = out2;
#line 190
			*nate__writer << "Hello,planet "; 
			*nate__writer << ((Core::formatted(5.4, Core::Format{8, 1, 32, 265} ))); *nate__writer << " "; 
			*nate__writer << 45; *nate__writer << " " << 56; *nate__writer << " "; 
			nate__writer = output;
#line 193
			*nate__writer << "Hi "; 
			*output << "Hi " << 45; *output << std::endl; 
			*nate__writer << "End" << std::endl; 
			*nate__writer << std::endl; 
			nate__writer = out2;
#line 197
			*nate__writer << "Hi" << std::endl; 
#line 188
		}
#line 199
		*output << "After: " << std::boolalpha ; *output << (((out&& out->good()))); *output << " " << std::boolalpha ; *output << (((out&& out->is_open()))); *output << std::endl; 
	}
}
