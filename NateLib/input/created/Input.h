#pragma once
#include "C:\Users\ruud\source\repos\Nate\NateLib\core\Core.h"
#line 6 "C:\\Users\\ruud\\source\\repos\\Nate\\NateLib\\core\\Input.nd"
class Input
{
public:
	virtual ~Input() = default;
#line 7
	virtual bool E_me__is_ok_() const = 0;
	virtual  std::shared_ptr<std::istream> E_me__stream_() const = 0;
};

