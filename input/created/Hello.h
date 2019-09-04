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
public:
static std::shared_ptr<Hello> new_Hello_();
std::string _E__E_(const std::string& world) const;
std::string stream_out__E_() const;
private:
std::unique_ptr<Hello> _impl;
};
