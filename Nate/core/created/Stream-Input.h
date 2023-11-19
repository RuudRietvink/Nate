#pragma once
#include "C:\Users\ruud\source\repos\Nate\Nate\core\Core.h"
#line 6
class Stream_Input
{
public:
	virtual ~Stream_Input() = default;
#line 7
	virtual bool E_me__is_ok_() const = 0;
	virtual  std::shared_ptr<std::istream> E_me__stream_() const = 0;
};

