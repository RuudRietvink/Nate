#include <cmath>
#include "Core.h"
#include "BaseObject.h"
#line 3
class BaseObject_::__impl
{
private:
	friend class BaseObject_;
	BaseObject_* me;
public:
	__impl(BaseObject_* aMe) : me(aMe) {}
};

BaseObject_::BaseObject_()
	: _impl(new __impl(this)) {}
BaseObject_::~BaseObject_() { delete _impl; }
#line 4
void BaseObject_::initialize__E_O_me__()
{
}
