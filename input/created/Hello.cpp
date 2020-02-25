#include "C:\Users\ruud\source\repos\Nate\core\Core.h"
#include "C:\Users\ruud\source\repos\Nate\input\created\Hello.h"
#include "C:\Users\ruud\source\repos\Nate\input\created\BaseObject.h"
class Local: public BaseObject
{
public:
#line 4 "Hello.ns"
		int32_t localVar = 3;
		static std::shared_ptr<Local> new_Local_()
		{
#line 7
			std::shared_ptr<Local> result = {};
			result = std::make_shared<Local>();
#line 9
			result->initialize__E_O_BaseObject_();
			return result;
		}

		std::string _E_Local__E_text_(const std::string& world) const
		{
#line 13
			return ("Local "+ world)+ (std::to_string(localVar));
		}

};

class Hello::__impl
{
private:
  Hello* me;
public:
  __impl(Hello* aMe) : me(aMe) {}
#line 16
		std::string myVar = "Hi There";
		bool myBool = false;
#line 18
		Data myData = {};
		int32_t get__E_Hello_age_() const
		{
#line 39
			int32_t hi = 34;
#line 41
			if (myBool)
			{
#line 41
				std::cout << "Test " << (myVar);				std::cout << " " << ((myData. greet));				std::cout << " " << (((Local::new_Local_())->_E_Local__E_text_(" world")));				std::cout << std::endl;
			}
			return (me-> age_get()+ me->age)+ hi;
		}

		static std::string greetings_()
		{
#line 45
			return "Hello";
		}

		std::string _E_Hello_greetings_() const
		{
#line 48
			std::shared_ptr<Hello> other = Hello::new_Hello_();
			return "Hello "+ (Hello::hi__E_Hello_(other));
		}

};

Hello::Hello()
  : _impl(new __impl(this)) {}

Hello::~Hello() { delete _impl; }

	std::string Hello::name_get() const
	{
#line 22
		int32_t hi = 3;
		return name+ _impl->myVar;
	}

	const std::string& Hello::name_set(const std::string& value)
	{
#line 25
		int32_t hi = 5;
		std::string temp = ((name+ value)+ address)+ (std::to_string(hi));
#line 27
		name = temp;
#line 29
		if (_impl->myBool)
		{
#line 29
			name = name+ (_impl->myData. greet);
		}
		return name;
	}

	std::shared_ptr<Hello> Hello::new_Hello_()
	{
#line 33
		std::shared_ptr<Hello> result = {};
		result = std::make_shared<Hello>();
#line 35
		result->initialize__E_O_BaseObject_();
		return result;
	}

	std::string Hello::_E_Hello__E_text_(const std::string& world) const
	{
#line 52
		return ((__impl::greetings_())+ " ")+ world;
	}

	std::string Hello::hi__E_Hello_(const std::shared_ptr<Hello>& other)
	{
#line 55
		return other->_E_Hello__E_text_(" earth");
	}

	std::shared_ptr<Hello> Hello::_E_O_Hello_greeted_()
	{
#line 58
		_impl->myData. greet = _impl->_E_Hello_greetings_();
		std::shared_ptr<Hello> other = std::dynamic_pointer_cast<Hello>(shared_from_this());
#line 60
		return std::dynamic_pointer_cast<Hello>(shared_from_this());
	}

	std::string Hello::stream_out__E_Hello_() const
	{
#line 63
		return ((_impl->_E_Hello_greetings_())+ ", it's ")+ (this->_E_Hello__E_text_(" angry world"));
	}

	int32_t Hello::age_get() const { return age; }
	Hello::Data Hello::data_get() const { return data; }
	const Hello::Data& Hello::data_set(const Hello::Data& value) { return data = value; }
	std::string Hello::address_get() const { return address; }
	const std::string& Hello::address_set(const std::string& value) { return address = value; }
