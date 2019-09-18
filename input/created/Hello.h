#pragma once
#include "C:\Users\ruud\source\repos\Nate\core\Core.h"
#include <string>
#include "C:\Users\ruud\source\repos\Nate\input\created\BaseObject.h"
class Hello: public BaseObject
{
private:
  class __impl;
  __impl* _impl;
public:
  Hello();
  virtual ~Hello();
#line 5 ".\\Hello.nd"
	struct Data
	{
#line 5
	std::string greet;
	int32_t count;
	Data()
		: greet()
		, count()
		{}
	};

	const std::string& get_name() const;
	const std::string& get_address() const;
	int32_t get_age() const;
	static std::shared_ptr<Hello> new_Hello_();
	virtual std::string _E_Hello__E_text_(const std::string& world) const;
	static std::string hi__E_Hello_(const std::shared_ptr<Hello>& other);
	virtual std::string stream_out__E_Hello_() const;
};

