#pragma once
#include "C:\Users\ruud\source\repos\Nate\Nate\core\Core.h"
#include "C:\Users\ruud\source\repos\Nate\Nate\core\created\BaseObject.h"
#line 6 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\core\\Input.nd"
class Input: public BaseObject
{
public:
	virtual ~Input();
	Input();
private:
	class __impl;
	__impl* _impl;
	friend class __impl;
	public:
#line 7
	virtual bool E_me__is_ok_() const;
};

