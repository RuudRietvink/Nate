#include "C:\Users\ruud\source\repos\Nate\Nate\core\Core.h"
#include "C:\Users\ruud\source\repos\Nate\Nate\core\created\BaseObject.h"
#line 3 "BaseObject.ns"
class BaseObject::__impl
{
private:
	friend class BaseObject;
	BaseObject* me;
public:
	__impl(BaseObject* aMe) : me(aMe) {}
};

BaseObject::BaseObject()
	: _impl(new __impl(this)) {}
BaseObject::~BaseObject() { delete _impl; }
#line 4
void BaseObject::initialize_E_O_me__()
{
}

