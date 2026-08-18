#include <cmath>
#include "Core.h"
#include "File-Input.h"
#line 8
#include <fstream>
#line 11
class File_Input_::__impl
{
private:
	friend class File_Input_;
	File_Input_* me;
#line 12
	 std::shared_ptr<std::ifstream> stream_ = {};
public:
	__impl(File_Input_* aMe) : me(aMe) {}
};

File_Input_::File_Input_()
	: _impl(new __impl(this)) {}
File_Input_::~File_Input_() { delete _impl; }
#line 14
std::shared_ptr<File_Input_> File_Input_::new__File_Input__()
{
#line 15
	return std::make_shared<File_Input_>();
}
 std::shared_ptr<std::istream> File_Input_::E_me__stream__() const
{
#line 18
	return _impl->stream_;
}
bool File_Input_::E_me__is__ok__() const
{
#line 21
	return _impl->stream_&&_impl->stream_->good();
}
bool File_Input_::E_me__is__opened__() const
{
#line 24
	return _impl->stream_->is_open();
}
void File_Input_::open__E_O_me__from__file__E_text__(const string_t& file_)
{
#line 27
	_impl->stream_.reset(new std::ifstream(file_, std::ios::in));
}
void File_Input_::close__E_O_me__()
{
#line 30
	 if (_impl->stream_)_impl->stream_->close();
}
