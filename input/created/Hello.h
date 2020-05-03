#pragma once
#include "C:\Users\ruud\source\repos\Nate\core\Core.h"
#include <string>

class Greetable
{
public:
  virtual ~Greetable() {}
#line 4 ".\\Hello.nd"
	struct Greet
	{
	std::string greeting;
#line 6
	int32_t count;
	Greet()
		: greeting()
		, count()
		{}
#line 8
	};

public:
	virtual Greetable::Greet greet_get() const = 0;
	virtual const Greetable::Greet& greet_set(const Greetable::Greet& value) = 0;
	virtual std::string ls_E_me__E_text_(const std::string& world) const = 0;
};

#include "C:\Users\ruud\source\repos\Nate\input\created\BaseObject.h"

class Hi: public BaseObject
{
public:
  virtual ~Hi();
  Hi();
private:
  class __impl;
  __impl* _impl;
  friend class __impl;
#line 13
	struct Datum
	{
	std::string fun;
#line 15
	int32_t count;
	Datum()
		: fun()
		, count()
		{}
#line 17
	};

private:
	int32_t age = {};
public:
	virtual int32_t age_get() const;
private:
	Datum datum = {};
public:
	virtual Hi::Datum datum_get() const;
	virtual const Hi::Datum& datum_set(const Hi::Datum& value);
	static std::shared_ptr<Hi> new_Hi_();
	virtual std::string E_me__E_text_(const std::string& world) const;
};


class Hello: public Hi, public Greetable
{
public:
  virtual ~Hello();
  Hello();
private:
  class __impl;
  __impl* _impl;
  friend class __impl;
#line 25
	struct Data
	{
	std::string greet;
#line 27
	int32_t count;
	Data()
		: greet()
		, count()
		{}
#line 29
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
	virtual std::shared_ptr<Hello> E_O_me__greeted_();
	std::string E_me__E_text_(const std::string& world) const;
	static std::string hi_E_Hello_(const std::shared_ptr<Hello>& other);
	virtual std::string stream_out_E_me__() const;
private:
	Greet greet = {};
public:
	virtual Greetable::Greet greet_get() const;
	virtual const Greetable::Greet& greet_set(const Greetable::Greet& value);
};

