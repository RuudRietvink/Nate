#include "C:\Users\ruud\source\repos\Nate\Nate\core\Core.h"
#include "C:\Users\ruud\source\repos\Nate\Nate\input\created\Hello.h"
#line 3 "Hello.ns"
class Local: public BaseObject
{
public:
	static std::shared_ptr<Local> new_Local_()
	{
#line 7
		std::shared_ptr<Local> result = {};
		result.reset(new decltype(result)::element_type());
		result->initialize_E_O_me__();
		return result;
	}

#line 12
	std::string E_me__E_text_(const std::string& world) const
	{
#line 13
		return ("Local "+ world)+ (std::to_string(localVar));
	}

};

#line 15
class Hi::__impl
{
private:
	friend class Hi;
	Hi* me;
public:
	__impl(Hi* aMe) : me(aMe) {}
};

Hi::Hi()
	: _impl(new __impl(this)) {}
Hi::~Hi() { delete _impl; }
#line 17
int32_t Hi::age_get() const
{
#line 18
	return age;
}

const Greetable::Greet& Hi::greet_set(const Greetable::Greet& value)
{
#line 22
	greet = value;
	return greet;
}

#line 25
std::shared_ptr<Hi> Hi::new_Hi2_()
{
#line 26
	return std::make_shared<Hi>();
}

#line 28
static std::string yo_()
{
#line 29
	return "Hi";
}

#line 31
std::string Hi::E_me__E_text_(const std::string& world) const
{
#line 32
	return ((((__impl::yo_())+ " ")+ (std::to_string(age)))+ " ")+ world;
}

#line 34
std::string Hi::ls_E_me__E_text_(const std::string& world) const
{
#line 35
	return "ls "+ (__impl::yo_());
}

Hi::Datum Hi::datum_get() const { return datum; }
const Hi::Datum& Hi::datum_set(const Hi::Datum& value) { return datum = value; }
Greetable::Greet Hi::greet_get() const { return greet; }
#line 37
class Hello::__impl
{
private:
	friend class Hello;
	Hello* me;
#line 38
	std::string myVar = "Hi There";
	bool myBool = false;
	Data myData = {};
public:
	__impl(Hello* aMe) : me(aMe) {}
};

Hello::Hello()
	: _impl(new __impl(this)) {}
Hello::~Hello() { delete _impl; }
#line 43
std::string Hello::name_get() const
{
#line 44
	int32_t hi = 3;
	return name+ _impl->myVar;
}

#line 46
const std::string& Hello::name_set(const std::string& value)
{
#line 47
	int32_t hi = 5;
	std::string temp = ((name+ value)+ address)+ (std::to_string(hi));
	name = temp;
	if (myBool)
	{
#line 51
		name = name+ (_impl->myData. greet);
#line 50
	}
#line 52
	return name;
}

const Greetable::Greet& Hello::greet_set(const Greetable::Greet& value)
{
#line 56
	greet = value;
	return greet;
}

#line 59
std::shared_ptr<Hello> Hello::new_Hello_()
{
#line 60
	return std::make_shared<Hello>();
}

#line 62
int32_t get_E_me__age_() const
{
#line 63
	int32_t hi = 34;
	if (myBool)
	{
#line 65
		*output << "Test " << (myVar); *output << " " << ((myData. greet)); *output << " " << (((Local::new_Local_())->E_me__E_text_(" world"))); *output << std::endl; 
#line 64
	}
#line 66
	return (me-> age_get()+ me->age)+ hi;
}

#line 68
static std::string greetings_()
{
#line 69
	return "Hello";
}

#line 71
std::string E_me__greetings_() const
{
#line 72
	std::shared_ptr<Hello> other = Hello::new_Hello_();
	return "Hello "+ (Hello::hi_E_Hello_(other));
}

#line 75
std::string Hello::E_me__E_text_(const std::string& world) const
{
#line 76
	return ((__impl::greetings_())+ " ")+ world;
}

#line 78
std::string Hello::hi_E_Hello_(const std::shared_ptr<Hello>& other)
{
#line 79
	return other->E_me__E_text_(" earth");
}

#line 81
std::shared_ptr<Hello> Hello::E_O_me__greeted_()
{
#line 82
	_impl->myData. greet = _impl->E_me__greetings_();
	std::shared_ptr<Hello> other = std::dynamic_pointer_cast<Hello>(shared_from_this());
	return std::dynamic_pointer_cast<Hello>(shared_from_this());
}

#line 86
std::string Hello::stream_out_E_me__() const
{
#line 87
	return ((_impl->E_me__greetings_())+ ", it's ")+ (this->E_me__E_text_(" angry world"));
}

#line 89
std::string Hello::ls_E_me__E_text_(const std::string& world) const
{
#line 90
	return "ls "+ (_impl->E_me__greetings_());
}

int32_t Hello::age_get() const { return age; }
std::string Hello::address_get() const { return address; }
const std::string& Hello::address_set(const std::string& value) { return address = value; }
Hello::Data Hello::data_get() const { return data; }
const Hello::Data& Hello::data_set(const Hello::Data& value) { return data = value; }
Greetable::Greet Hello::greet_get() const { return greet; }
