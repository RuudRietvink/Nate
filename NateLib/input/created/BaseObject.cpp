#include "BaseObject.h"

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
void BaseObject::initialize_E_O_me__()
{
}
