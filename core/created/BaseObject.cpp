#include "C:\Users\ruud\source\repos\Nate\core\Core.h"
#include <cstdint>
#include <iostream>
#include <algorithm>
#include <memory>
extern std::shared_ptr<std::ostream> output;
extern std::shared_ptr<std::ostream> error;
#include "C:\Users\ruud\source\repos\Nate\core\created\BaseObject.h"
class BaseObject::__impl
{
public:
  __impl() {}
};

BaseObject::BaseObject()
  : _impl(new __impl()) {}

BaseObject::~BaseObject() { delete _impl; }

void BaseObject::initialize__E_O_BaseObject_()
{
#line 5 "BaseObjects\\BaseObject.ns"
	int32_t a = {};
}

