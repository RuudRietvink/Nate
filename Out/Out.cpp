#include "C:\Users\ruud\source\repos\Nate\Nate\core\Core.h"
#include "C:\Users\ruud\source\repos\Nate\Nate\input\created\File-Input.h"
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
#define NOMINMAX
#include <windows.h>
#line 531 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\input\\nate.in"
int main(int argc, char** argv)
{
	output = std::shared_ptr<std::ostream>(&std::cout, [](void*) {});
	error = std::shared_ptr<std::ostream>(&std::cerr, [](void*) {});
	input = std::shared_ptr<std::istream>(&std::cin, [](void*) {});
	SetConsoleOutputCP(65001);
#line 550
	std::shared_ptr<File_Input> in = File_Input::new_File_Input_();
	in->open_E_O_me__from_file_E_text_("d:\\uit");
	if (in->E_me__is_opened_())
	{
#line 553
		string_t s = {};
#line 553
		string_t t = {};
		std::shared_ptr<std::istream> nate__reader = (in)->E_me__stream_();
#line 554
std::getline(*nate__reader, s);
		*output << (s); *output << std::endl; 
		*output << (t); *output << std::endl; 
*nate__reader >> s;
std::getline(*nate__reader >> std::ws, t);
#line 558
		*output << (s); *output << std::endl; 
		*output << (t); *output << std::endl; 
*nate__reader >> s;
*nate__reader >> std::ws >> t;
(*nate__reader).ignore(std::numeric_limits<std::streamsize>::max(), '\n');
#line 561
		*output << (s); *output << std::endl; 
		*output << (t); *output << std::endl; 
#line 552
	}
}
