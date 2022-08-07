#include "C:\Users\ruud\source\repos\Nate\Nate\core\Core.h"
#include "C:\Users\ruud\source\repos\Nate\Nate\core\created\File-Input.h"
#line 8 "File-Input.ns"
#include <fstream>
#line 11
class File_Input::__impl
{
private:
	friend class File_Input;
	File_Input* me;
#line 12
	std::ifstream stream = {};
public:
	__impl(File_Input* aMe) : me(aMe) {}
};

File_Input::File_Input()
	: _impl(new __impl(this)) {}
File_Input::~File_Input() { delete _impl; }
#line 14
bool File_Input::E_me__is_opened_() const
{
#line 15
	return 
_impl->stream.is_open()
;
}

#line 19
bool File_Input::E_me__is_ok_() const
{
#line 20
	return 
_impl->stream.good()
;
}

#line 24
void File_Input::open_E_O_me__from_file_E_text_(const string_t& file)
{
#line 25
	
_impl->stream.open(file,std::ios::in)
;
}

#line 29
void File_Input::close_E_O_me__()
{
#line 30
	
if(_impl->stream)_impl->stream.close()
;
}

