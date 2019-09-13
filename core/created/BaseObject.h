#pragma once
#include "C:\Users\ruud\source\repos\Nate\core\Core.h"
#include <cstdint>
#include <iostream>
#include <algorithm>
#include <memory>
extern std::shared_ptr<std::ostream> output;
extern std::shared_ptr<std::ostream> error;
class BaseObject
{
private:
  class __impl;
  __impl* _impl;
public:
  BaseObject();
  virtual ~BaseObject();
	virtual void initialize__E_O_BaseObject_();
};

