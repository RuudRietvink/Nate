#pragma once
#include "C:\Users\ruud\source\repos\Nate\NateLib\core\cpp\Core.h"
#line 1 "C:\\Users\\ruud\\source\\repos\\Nate\\NateLib\\core\\IFile.nd"
struct File_Property
{
	File_Property()
		: readable()
		, writable()
		, size()
		, creation_time()
		, path()
	{}
#line 2
	bool readable;
	bool writable;
	int64_t size;
	int64_t creation_time;
	string_t path;
};

#line 8
struct File_Filter
{
	File_Filter()
		: filter_value()
		, filter_index()
	{}
#line 9
	File_Property filter_value;
	int32_t filter_index;
};

#line 12
class IFile
{
public:
	virtual ~IFile() = default;
#line 17
	virtual bool E_me__is_readable_() const = 0;
	virtual bool E_me__is_writeable_() const = 0;
public:
	virtual string_t suffix_get() const = 0;
	virtual const string_t& suffix_set(const string_t& value) = 0;
public:
	virtual string_t path_get() const = 0;
	virtual const string_t& path_set(const string_t& value) = 0;
public:
	virtual string_t name_get() const = 0;
	virtual const string_t& name_set(const string_t& value) = 0;
};

