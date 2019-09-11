#pragma once
#include "C:\Users\ruud\source\repos\Nate\core\Core.h"
#include <cstdint>
#include <iostream>
#include <algorithm>
#include <memory>
extern std::shared_ptr<std::ostream> output;
extern std::shared_ptr<std::ostream> error;
#include <string>
class Hello
{
private:
  class __impl;
  __impl* _impl;
public:
  Hello();
  virtual ~Hello();
static std::shared_ptr<Hello> new_Hello_();
virtual std::string _E_Hello__E_text_(const std::string& world) const;
virtual std::string stream_out__E_Hello_() const;
};

