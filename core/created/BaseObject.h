#pragma once
#include "C:\Users\ruud\source\repos\Nate\core\Core.h"
class BaseObject
{
private:
  class __impl;
  __impl* _impl;
  friend class __impl;
public:
  BaseObject();
  virtual ~BaseObject();
	virtual void initialize__E_O_BaseObject_();
};

