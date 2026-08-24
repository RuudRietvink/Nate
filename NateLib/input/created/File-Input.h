#pragma once
#include "C:\Users\ruud\source\repos\Nate\NateLib\core\cpp\Core.h"
#include "C:\Users\ruud\source\repos\Nate\NateLib\input\created\Input.h"
#include "C:\Users\ruud\source\repos\Nate\NateLib\input\created\BaseObject.h"
#line 8 "C:\\Users\\ruud\\source\\repos\\Nate\\NateLib\\core\\File-Input.nd"
class File_Input: public virtual Input, public BaseObject
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
	static std::shared_ptr<File_Input> new_File_Input_();
	virtual void open_E_O_me__from_file_E_text_(const string_t& file);
	virtual bool E_me__is_opened_() const;
	virtual void close_E_O_me__();
	bool E_me__is_ok_() const override;
#line 13
	 std::shared_ptr<std::istream> E_me__stream_() const override;
};

