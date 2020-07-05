#pragma once
#include "C:\Users\ruud\source\repos\Nate\Nate\core\Core.h"
#include <string>
#line 3 ".\\Hello.nd"
class Greetable
{
public:
  virtual ~Greetable() = default;
	struct Greet
	{
#line 5
	std::string greeting;
	int32_t count;
	Greet()
		: greeting()
		, count()
		{}
#line 8
	};

public:
#line 8
	virtual Greetable::Greet greet_get() const = 0;
#line 8
	virtual const Greetable::Greet& greet_set(const Greetable::Greet& value) = 0;
	virtual std::string ls_E_me__E_text_(const std::string& world) const = 0;
};

#include "C:\Users\ruud\source\repos\Nate\Nate\input\created\BaseObject.h"
#line 12 ".\\Hello.nd"
class Hi: public virtual Greetable, public BaseObject
{
public:
  virtual ~Hi();
  Hi();
private:
  class __impl;
  __impl* _impl;
  friend class __impl;
public:
	struct Datum
	{
#line 14
	std::string fun;
	int32_t count;
	Datum()
		: fun()
		, count()
		{}
#line 17
	};

private:
#line 17
	int32_t age = {};
public:
#line 17
	virtual int32_t age_get() const;
private:
	Datum datum = {};
public:
	virtual Hi::Datum datum_get() const;
	virtual const Hi::Datum& datum_set(const Hi::Datum& value);
	static std::shared_ptr<Hi> new_Hi2_();
	virtual std::string E_me__E_text_(const std::string& world) const;
	std::string ls_E_me__E_text_(const std::string& world) const override;
private:
#line 8
	Greet greet = {};
public:
#line 8
	Greetable::Greet greet_get() const override;
#line 8
	const Greetable::Greet& greet_set(const Greetable::Greet& value) override;
};

#line 25
class Hello: public Hi, public virtual Greetable
{
public:
  virtual ~Hello();
  Hello();
private:
  class __impl;
  __impl* _impl;
  friend class __impl;
public:
	struct Data
	{
#line 27
	std::string greet;
	int32_t count;
	Data()
		: greet()
		, count()
		{}
#line 30
	};

private:
#line 30
	std::string name = {};
public:
#line 30
	virtual std::string name_get() const;
#line 30
	virtual const std::string& name_set(const std::string& value);
private:
#line 30
	std::string address = {};
public:
#line 30
	virtual std::string address_get() const;
#line 30
	virtual const std::string& address_set(const std::string& value);
private:
	int32_t age = {};
public:
	int32_t age_get() const override;
private:
#line 32
	Data data = {};
public:
#line 32
	virtual Hello::Data data_get() const;
#line 32
	virtual const Hello::Data& data_set(const Hello::Data& value);
	static std::shared_ptr<Hello> new_Hello_();
	virtual std::shared_ptr<Hello> E_O_me__greeted_();
	std::string E_me__E_text_(const std::string& world) const override;
	static std::string hi_E_Hello_(const std::shared_ptr<Hello>& other);
	virtual std::string stream_out_E_me__() const;
	std::string ls_E_me__E_text_(const std::string& world) const override;
private:
#line 8
	Greet greet = {};
public:
#line 8
	Greetable::Greet greet_get() const override;
#line 8
	const Greetable::Greet& greet_set(const Greetable::Greet& value) override;
};

