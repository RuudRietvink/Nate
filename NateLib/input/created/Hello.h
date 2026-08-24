#pragma once
#include "C:\Users\ruud\source\repos\Nate\NateLib\core\Core.h"
#include "C:\Users\ruud\source\repos\Nate\NateLib\input\created\BaseObject.h"
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

#line 11
	virtual string_t ls_E_O_me__E_text_(const string_t& world) = 0;
public:
	virtual Greetable::Greet greet_get() const = 0;
	virtual const Greetable::Greet& greet_set(const Greetable::Greet& value) = 0;
public:
	virtual int32_t prop_greetable_get() const = 0;
	virtual const int32_t prop_greetable_set(const int32_t value) = 0;
};

#line 13
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
#line 14
	struct Datum
	{
		Datum()
			: fun()
			, count()
		{}
#line 15
		string_t fun;
		int32_t count;
	};

#line 22
	static std::shared_ptr<Hi> new_Hi2_();
#line 24
	virtual string_t E_me__E_text_(const string_t& world) const;
#line 26
	string_t ls_E_O_me__E_text_(const string_t& world) override;
private:
	int32_t prop_hi = {};
public:
	virtual int32_t prop_hi_get() const;
	virtual const int32_t prop_hi_set(const int32_t value);
private:
	Datum datum = {};
public:
	virtual Hi::Datum datum_get() const;
	virtual const Hi::Datum& datum_set(const Hi::Datum& value);
private:
	Greet greet = {};
public:
	Greetable::Greet greet_get() const override;
	const Greetable::Greet& greet_set(const Greetable::Greet& value) override;
private:
	int32_t prop_greetable = {};
public:
	int32_t prop_greetable_get() const override;
	const int32_t prop_greetable_set(const int32_t value) override;
private:
	int32_t age = {};
public:
	virtual int32_t age_get() const;
};

#line 26
class Hello: public Hi
{
public:
	virtual ~Hello();
	Hello();
private:
	class __impl;
	__impl* _impl;
	friend class __impl;
	public:
#line 27
	struct Data
	{
		Data()
			: greet()
			, count()
		{}
#line 28
		string_t greet;
		int32_t count;
	};

#line 35
	static std::shared_ptr<Hello> new_Hello_();
#line 37
	virtual std::shared_ptr<Hello> E_O_me__greeted_();
#line 39
	string_t E_me__E_text_(const string_t& world) const override;
#line 41
	static string_t hi_E_Hello_(const std::shared_ptr<Hello>& other);
#line 43
	virtual string_t stream_out_E_me__() const;
#line 45
	string_t ls_E_O_me__E_text_(const string_t& world) override;
private:
	string_t name = {};
public:
	virtual string_t name_get() const;
	virtual const string_t& name_set(const string_t& value);
private:
	string_t address = {};
public:
	virtual string_t address_get() const;
	virtual const string_t& address_set(const string_t& value);
private:
	string_t prop_hello = {};
public:
	virtual string_t prop_hello_get() const;
	virtual const string_t& prop_hello_set(const string_t& value);
};

