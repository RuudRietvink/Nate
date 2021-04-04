#pragma once
#include "C:\Users\ruud\source\repos\Nate\Nate\core\Core.h"
#include "C:\Users\ruud\source\repos\Nate\Nate\input\created\BaseObject.h"
#line 6 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\core\\Text.ns"
#include <string>
typedef std::string string_t;
#line 3 ".\\Hello.nd"
class Greetable
{
public:
	virtual ~Greetable() = default;
#line 4
	struct Greet
	{
		Greet()
			: greeting()
			, count()
		{}
#line 5
		string_t greeting;
		int32_t count;
	};

#line 10
	virtual string_t ls_E_me__E_text_(const string_t& world) const = 0;
public:
	virtual Greetable::Greet greet_get() const = 0;
	virtual const Greetable::Greet& greet_set(const Greetable::Greet& value) = 0;
};

#line 12
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
#line 13
	struct Datum
	{
		Datum()
			: fun()
			, count()
		{}
#line 14
		string_t fun;
		int32_t count;
	};

#line 20
	static std::shared_ptr<Hi> new_Hi2_();
#line 22
	virtual string_t E_me__E_text_(const string_t& world) const;
#line 24
	string_t ls_E_me__E_text_(const string_t& world) const override;
private:
	Datum datum = {};
public:
	virtual Hi::Datum datum_get() const;
	virtual const Hi::Datum& datum_set(const Hi::Datum& value);
private:
	int32_t age = {};
public:
	virtual int32_t age_get() const;
private:
	Greet greet = {};
public:
	Greetable::Greet greet_get() const override;
	const Greetable::Greet& greet_set(const Greetable::Greet& value) override;
};

#line 24
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
#line 25
	struct Data
	{
		Data()
			: greet()
			, count()
		{}
#line 26
		string_t greet;
		int32_t count;
	};

#line 33
	static std::shared_ptr<Hello> new_Hello_();
#line 35
	virtual std::shared_ptr<Hello> E_O_me__greeted_();
#line 37
	string_t E_me__E_text_(const string_t& world) const override;
#line 39
	static string_t hi_E_Hello_(const std::shared_ptr<Hello>& other);
#line 41
	virtual string_t stream_out_E_me__() const;
#line 43
	string_t ls_E_me__E_text_(const string_t& world) const override;
private:
	string_t address = {};
public:
	virtual string_t address_get() const;
	virtual const string_t& address_set(const string_t& value);
private:
	Greet greet = {};
public:
	Greetable::Greet greet_get() const override;
	const Greetable::Greet& greet_set(const Greetable::Greet& value) override;
private:
	int32_t age = {};
public:
	int32_t age_get() const override;
private:
	Data data = {};
public:
	virtual Hello::Data data_get() const;
	virtual const Hello::Data& data_set(const Hello::Data& value);
private:
	string_t name = {};
public:
	virtual string_t name_get() const;
	virtual const string_t& name_set(const string_t& value);
};

