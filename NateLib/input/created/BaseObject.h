#pragma once
#include "C:\Users\ruud\source\repos\Nate\NateLib\core\cpp\Core.h"
#line 2 "C:\\Users\\ruud\\source\\repos\\Nate\\NateLib\\core\\BaseObject.nd"
class BaseObject: public std::enable_shared_from_this<BaseObject>
{
public:
	virtual ~BaseObject();
	BaseObject();
private:
	class __impl;
	__impl* _impl;
	friend class __impl;
	public:
#line 3
	virtual void initialize_E_O_me__();
};

