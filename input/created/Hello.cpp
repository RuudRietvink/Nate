#include "C:\Users\ruud\source\repos\Nate\core\Core.h"
#include "C:\Users\ruud\source\repos\Nate\input\created\Hello.h"
class Hello::__impl
{
private:
  Hello* me;
public:
  __impl(Hello* aMe) : me(aMe) {}
#line 4 "Hello.ns"
		std::string myVar = "Hi There";
		bool myBool = false;
#line 6
		Data myData = {};
		int32_t get__E_Hello_age_() const
		{
#line 25
			int32_t hi = 34;
#line 27
			if (myBool)
			{
#line 27
				std::cout << "Test " << (myVar);				std::cout << " " << ((myData. greet));				std::cout << std::endl;
			}
			return (me-> age_get()+ me->age)+ hi;
		}

		static std::string greetings_()
		{
#line 31
			return "Hello";
		}

		std::string _E_Hello_greetings_() const
		{
#line 34
			std::shared_ptr<Hello> other = Hello::new_Hello_();
			return "Hello "+ (Hello::hi__E_Hello_(other));
		}

};

Hello::Hello()
  : _impl(new __impl(this)) {}

Hello::~Hello() { delete _impl; }

	std::string Hello::name_get() const
	{
#line 10
		int32_t hi = 3;
		return name+ _impl->myVar;
	}

	const std::string& Hello::name_set(const std::string& value)
	{
#line 13
		int32_t hi = 5;
		std::string temp = ((name+ value)+ address)+ (std::to_string(hi));
#line 15
		name = temp;
#line 17
		if (_impl->myBool)
		{
#line 17
			name = name+ (_impl->myData. greet);
		}
		return name;
	}

	std::shared_ptr<Hello> Hello::new_Hello_()
	{
#line 21
		auto me = std::make_shared<Hello>();
		return me;
	}

	std::string Hello::_E_Hello__E_text_(const std::string& world) const
	{
#line 38
		return ((__impl::greetings_())+ " ")+ world;
	}

	std::string Hello::hi__E_Hello_(const std::shared_ptr<Hello>& other)
	{
#line 41
		return other->_E_Hello__E_text_(" earth");
	}

	std::string Hello::stream_out__E_Hello_() const
	{
#line 44
		return ((_impl->_E_Hello_greetings_())+ ", it's ")+ (this->_E_Hello__E_text_(" angry world"));
	}

	std::string Hello::address_get() const { return address; }
	const std::string& Hello::address_set(const std::string& value) { return address = value; }
	int32_t Hello::age_get() const { return age; }
	Hello::Data Hello::data_get() const { return data; }
	const Hello::Data& Hello::data_set(const Hello::Data& value) { return data = value; }
