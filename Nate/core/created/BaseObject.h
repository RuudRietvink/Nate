#include <cmath>
#pragma once
#include "Core.h"
#line 2
class BaseObject_: public std::enable_shared_from_this<BaseObject_>
{
public:
	virtual ~BaseObject_();
	BaseObject_();
private:
	class __impl;
	__impl* _impl;
	friend class __impl;
public:
#line 3
	virtual void initialize__E_O_me__();
};

