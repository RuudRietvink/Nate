#include "C:\Users\ruud\source\repos\Nate\core\Core.h"
#include "C:\Users\ruud\source\repos\Nate\core\created\BaseObject.h"
class BaseObject::__impl
{
private:
  BaseObject* me;
public:
  __impl(BaseObject* aMe) : me(aMe) {}
};

BaseObject::BaseObject()
  : _impl(new __impl(this)) {}

BaseObject::~BaseObject() { delete _impl; }

	void BaseObject::initialize__E_O_BaseObject_()
	{
#line 5 "BaseObjects\\BaseObject.ns"
		int32_t a = {};
	}

