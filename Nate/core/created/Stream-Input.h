#include <cmath>
#pragma once
#include "Core.h"
#line 6
class Stream_Input_
{
public:
	virtual ~Stream_Input_() = default;
#line 7
	virtual bool E_me__is__ok__() const = 0;
	virtual  std::shared_ptr<std::istream> E_me__stream__() const = 0;
};

