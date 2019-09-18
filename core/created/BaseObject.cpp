#include "C:\Users\ruud\source\repos\Nate\core\Core.h"
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

