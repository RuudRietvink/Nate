#pragma once
#include "C:\Users\ruud\source\repos\Nate\core\Core.h"

class BaseObject: public std::enable_shared_from_this<BaseObject>
{
public:
  virtual ~BaseObject();
  BaseObject();
private:
  class __impl;
  __impl* _impl;
  friend class __impl;
	virtual void initialize_E_O_me__();
};

