#pragma once
#include "C:\Users\ruud\source\repos\Nate\Nate\core\Core.h"
#include "C:\Users\ruud\source\repos\Nate\Nate\input\created\BaseObject.h"
#line 6 "C:\\Users\\ruud\\source\\repos\\Nate\\Nate\\core\\Text.ns"
#include <string>
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
		std::string greeting;
		int32_t count;
	};

#line 10
	virtual std::string ls_E_me__E_text_(const std::string& world) const = 0;
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
		std::string fun;
		int32_t count;
	};

#line 20
	static std::shared_ptr<Hi> new_Hi2_();
#line 22
	virtual std::string E_me__E_text_(const std::string& world) const;
	std::string ls_E_me__E_text_(const std::string& world) const override;
private:
	Greet greet = {};
public:
	Greetable::Greet greet_get() const override;
	const Greetable::Greet& greet_set(const Greetable::Greet& value) override;
private:
	int32_t age = {};
public:
	virtual int32_t age_get() const;
private:
	Datum datum = {};
public:
	virtual Hi::Datum datum_get() const;
	virtual const Hi::Datum& datum_set(const Hi::Datum& value);
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
#line 26
	struct Data
	{
		Data()
			: greet()
			, count()
		{}
#line 27
		std::string greet;
		int32_t count;
	};

#line 34
	static std::shared_ptr<Hello> new_Hello_();
#line 36
	virtual std::shared_ptr<Hello> E_O_me__greeted_();
#line 38
	std::string E_me__E_text_(const std::string& world) const override;
#line 40
	static std::string hi_E_Hello_(const std::shared_ptr<Hello>& other);
#line 42
	virtual std::string stream_out_E_me__() const;
#line 44
	std::string ls_E_me__E_text_(const std::string& world) const override;
private:
	std::string address = {};
public:
	virtual std::string address_get() const;
	virtual const std::string& address_set(const std::string& value);
private:
	std::string name = {};
public:
	virtual std::string name_get() const;
	virtual const std::string& name_set(const std::string& value);
private:
	Data data = {};
public:
	virtual Hello::Data data_get() const;
	virtual const Hello::Data& data_set(const Hello::Data& value);
private:
	int32_t age = {};
public:
	int32_t age_get() const override;
private:
	Greet greet = {};
public:
	Greetable::Greet greet_get() const override;
	const Greetable::Greet& greet_set(const Greetable::Greet& value) override;
};

