#pragma once
#include "C:\Users\ruud\source\repos\Nate\core\Core.h"
#include <string>
#include "C:\Users\ruud\source\repos\Nate\input\created\BaseObject.h"
class Hello: public BaseObject
{
private:
  class __impl;
  __impl* _impl;
  friend class __impl;
public:
  Hello();
  virtual ~Hello();
#line 4 ".\\Hello.nd"
	struct Data
	{
	std::string greet;
#line 6
	int32_t count;
	Data()
		: greet()
		, count()
		{}
#line 8
	};

private:
	std::string name = {};
public:
	virtual std::string name_get() const;
	virtual const std::string& name_set(const std::string& value);
private:
	std::string address = {};
public:
	virtual std::string address_get() const;
	virtual const std::string& address_set(const std::string& value);
private:
	int32_t age = {};
public:
	int32_t age_get() const;
private:
	Data data = {};
public:
	virtual Hello::Data data_get() const;
	virtual const Hello::Data& data_set(const Hello::Data& value);
	static std::shared_ptr<Hello> new_Hello_();
	virtual std::shared_ptr<Hello> _E_O_Hello_greeted_();
	std::string _E_Hello__E_text_(const std::string& world) const;
	static std::string hi__E_Hello_(const std::shared_ptr<Hello>& other);
	virtual std::string stream_out__E_Hello_() const;
};

