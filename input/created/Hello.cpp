#include "C:\Users\ruud\source\repos\Nate\core\Core.h"
#include "C:\Users\ruud\source\repos\Nate\input\created\Hello.h"
#line 3 "Hello.ns"
class Local: public BaseObject
{
public:
		int32_t localVar = 3;
		static std::shared_ptr<Local> new_Local_()
		{
#line 7
			std::shared_ptr<Local> result = {};
			result.reset(new decltype(result)::element_type());
#line 9
			result->initialize_E_O_me__();
			return result;
		}

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
  Hi* me;
public:
  __impl(Hi* aMe) : me(aMe) {}
		static std::string yo_()
		{
#line 29
			return "Hi";
		}

};

Hi::Hi()
  : _impl(new __impl(this)) {}

Hi::~Hi() { delete _impl; }

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

	std::shared_ptr<Hi> Hi::new_Hi2_()
	{
#line 26
		return std::make_shared<Hi>();
	}

	std::string Hi::E_me__E_text_(const std::string& world) const
	{
#line 32
		return ((((__impl::yo_())+ " ")+ (std::to_string(age)))+ " ")+ world;
	}

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
  Hello* me;
public:
  __impl(Hello* aMe) : me(aMe) {}
		std::string myVar = "Hi There";
#line 39
		bool myBool = false;
		Data myData = {};
		int32_t get_E_me__age_() const
		{
#line 63
			int32_t hi = 34;
#line 65
			if (myBool)
			{
#line 65
				std::cout << "Test " << (myVar);				std::cout << " " << ((myData. greet));				std::cout << " " << (((Local::new_Local_())->E_me__E_text_(" world")));				std::cout << std::endl;
			}
			return (me-> age_get()+ me->age)+ hi;
		}

		static std::string greetings_()
		{
#line 69
			return "Hello";
		}

		std::string E_me__greetings_() const
		{
#line 72
			std::shared_ptr<Hello> other = Hello::new_Hello_();
			return "Hello "+ (Hello::hi_E_Hello_(other));
		}

};

Hello::Hello()
  : _impl(new __impl(this)) {}

Hello::~Hello() { delete _impl; }

	std::string Hello::name_get() const
	{
#line 44
		int32_t hi = 3;
		return name+ _impl->myVar;
	}

	const std::string& Hello::name_set(const std::string& value)
	{
#line 47
		int32_t hi = 5;
		std::string temp = ((name+ value)+ address)+ (std::to_string(hi));
#line 49
		name = temp;
#line 51
		if (_impl->myBool)
		{
#line 51
			name = name+ (_impl->myData. greet);
		}
		return name;
	}

	const Greetable::Greet& Hello::greet_set(const Greetable::Greet& value)
	{
#line 56
		greet = value;
		return greet;
	}

	std::shared_ptr<Hello> Hello::new_Hello_()
	{
#line 60
		return std::make_shared<Hello>();
	}

	std::string Hello::E_me__E_text_(const std::string& world) const
	{
#line 76
		return ((__impl::greetings_())+ " ")+ world;
	}

	std::string Hello::hi_E_Hello_(const std::shared_ptr<Hello>& other)
	{
#line 79
		return other->E_me__E_text_(" earth");
	}

	std::shared_ptr<Hello> Hello::E_O_me__greeted_()
	{
#line 82
		_impl->myData. greet = _impl->E_me__greetings_();
		std::shared_ptr<Hello> other = std::dynamic_pointer_cast<Hello>(shared_from_this());
#line 84
		return std::dynamic_pointer_cast<Hello>(shared_from_this());
	}

	std::string Hello::stream_out_E_me__() const
	{
#line 87
		return ((_impl->E_me__greetings_())+ ", it's ")+ (this->E_me__E_text_(" angry world"));
	}

	std::string Hello::ls_E_me__E_text_(const std::string& world) const
	{
#line 90
		return "ls "+ (_impl->E_me__greetings_());
	}

	int32_t Hello::age_get() const { return age; }
	Greetable::Greet Hello::greet_get() const { return greet; }
	std::string Hello::address_get() const { return address; }
	const std::string& Hello::address_set(const std::string& value) { return address = value; }
	Hello::Data Hello::data_get() const { return data; }
	const Hello::Data& Hello::data_set(const Hello::Data& value) { return data = value; }
