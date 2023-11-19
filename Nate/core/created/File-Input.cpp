#include "C:\Users\ruud\source\repos\Nate\Nate\core\Core.h"
#include "C:\Users\ruud\source\repos\Nate\Nate\core\created\File-Input.h"
#line 8
#include <fstream>
#line 11
class File_Input::__impl
{
private:
	friend class File_Input;
	File_Input* me;
#line 12
	 std::shared_ptr<std::ifstream> stream = {};
public:
	__impl(File_Input* aMe) : me(aMe) {}
};

File_Input::File_Input()
	: _impl(new __impl(this)) {}
File_Input::~File_Input() { delete _impl; }
#line 14
std::shared_ptr<File_Input> File_Input::new_File_Input_()
{
#line 15
	return std::make_shared<File_Input>();
}
 std::shared_ptr<std::istream> File_Input::E_me__stream_() const
{
#line 18
	return _impl->stream;
}
bool File_Input::E_me__is_ok_() const
{
#line 21
	return _impl->stream&&_impl->stream->good();
}
bool File_Input::E_me__is_opened_() const
{
#line 24
	return _impl->stream->is_open();
}
void File_Input::open_E_O_me__from_file_E_text_(const string_t& file)
{
#line 27
	_impl->stream.reset(new std::ifstream(file, std::ios::in));
}
void File_Input::close_E_O_me__()
{
#line 30
	 if (_impl->stream)_impl->stream->close();
}
