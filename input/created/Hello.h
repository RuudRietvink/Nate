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

	PROP_REF_(std::string, std::string&, name)
	PROP_REF_(std::string, std::string&, address)
	PROP_NUMBER_(int32_t, age)
	PROP_RECORD_(Data, Data&, data)
	static std::shared_ptr<Hello> new_Hello_();
	virtual std::string _E_Hello__E_text_(const std::string& world) const;
	static std::string hi__E_Hello_(const std::shared_ptr<Hello>& other);
	virtual std::string stream_out__E_Hello_() const;
};

