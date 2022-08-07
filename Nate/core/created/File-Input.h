#pragma once
#include "C:\Users\ruud\source\repos\Nate\Nate\core\Core.h"
#include "C:\Users\ruud\source\repos\Nate\Nate\core\created\Input.h"
#line 8 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\core\\File-Input.nd"
class File_Input: public Input
{
public:
	virtual ~File_Input();
	File_Input();
private:
	class __impl;
	__impl* _impl;
	friend class __impl;
	public:
#line 9
	virtual void open_E_O_me__from_file_E_text_(const string_t& file);
	virtual bool E_me__is_opened_() const;
	virtual void close_E_O_me__();
	bool E_me__is_ok_() const override;
};

