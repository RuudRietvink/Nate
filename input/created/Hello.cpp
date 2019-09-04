#include "C:\Users\ruud\source\repos\Nate\core\Core.h"
#include <cstdint>
#include <iostream>
#include <algorithm>
#include <memory>
extern std::shared_ptr<std::ostream> output;
extern std::shared_ptr<std::ostream> error;
#include "C:\Users\ruud\source\repos\Nate\input\created\Hello.h"
std::shared_ptr<Hello> Hello::new_Hello_(){
#line 5 "Hello.ns"
	return std::make_shared<Hello>();
}
std::string Hello::_E__E_(const std::string& world) const{
#line 8 "Hello.ns"
	return "Hello"+ (world);
}
std::string Hello::stream_out__E_() const{
#line 11 "Hello.ns"
	return "Hello, it's "+ (this->_E__E_(" angry world"));
}
