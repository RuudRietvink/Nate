#include "C:\Users\ruud\source\repos\Nate\core\Core.h"
#include <cstdint>
#include <iostream>
#include <algorithm>
#include <memory>
extern std::shared_ptr<std::ostream> output;
extern std::shared_ptr<std::ostream> error;
#include "C:\Users\ruud\source\repos\Nate\input\created\Hello.h"
std::string Hello::_E__E_(const std::string& world){
#line 5 "Hello.ns"
	return "Hello"+ (world);
}
std::string Hello::stream_out__E_(){
#line 8 "Hello.ns"
	return "Hello, it's me.";
}
