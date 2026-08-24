#undef NOMINMAX
#define NOMINMAX
#include <windows.h>
#include <cmath>
#include <stdfloat>
#pragma once
#include "C:\Users\ruud\source\repos\Nate\NateLib\core\Core.h"
#line 6
class Stream_Input_
{
public:
	virtual ~Stream_Input_() = default;
#line 7
	virtual bool E_me__is__ok__() const = 0;
	virtual  std::shared_ptr<std::istream> E_me__stream__() const = 0;
};

