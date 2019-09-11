#include "C:\Users\ruud\source\repos\Nate\core\Core.h"
#include <cstdint>
#include <iostream>
#include <algorithm>
#include <memory>
extern std::shared_ptr<std::ostream> output;
extern std::shared_ptr<std::ostream> error;
#include "C:\Users\ruud\source\repos\Nate\input\created\Hello.h"
class Hello::__impl
{
public:
  __impl() {}
};

Hello::Hello()
  : _impl(new __impl()) {}
Hello::~Hello() { delete _impl; }
std::shared_ptr<Hello> Hello::new_Hello_()
{
#line 6 "Hello.ns"
	auto me = std::make_shared<Hello>();
#line 6
	return me;
}

std::string Hello::_E_Hello__E_text_(const std::string& world) const
{
#line 9
	return "Hello"+ (world);
}

std::string Hello::stream_out__E_Hello_() const
{
#line 12
	return "Hello, it's "+ (this->_E_Hello__E_text_(" angry world"));
}

