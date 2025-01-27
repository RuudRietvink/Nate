#undef NOMINMAX
#define NOMINMAX
#include <windows.h>
#include <cmath>
#include <stdfloat>
#pragma once
#include "C:\Users\ruud\source\repos\Nate\Nate\core\Core.h"
#include "C:\Users\Ruud\source\repos\Nate\NateTest\created\Stream-Input.h"
#include "C:\Users\Ruud\source\repos\Nate\NateTest\created\BaseObject.h"
#line 8
class File_Input_: public virtual Stream_Input_, public BaseObject_
{
public:
	virtual ~File_Input_();
	File_Input_();
private:
	class __impl;
	__impl* _impl;
	friend class __impl;
public:
#line 9
	static std::shared_ptr<File_Input_> new__File_Input__();
	virtual void open__E_O_me__from__file__E_text__(const string_t& file_);
	virtual bool E_me__is__opened__() const;
	virtual void close__E_O_me__();
	bool E_me__is__ok__() const override;
#line 13
	 std::shared_ptr<std::istream> E_me__stream__() const override;
};

