#include "C:\Users\ruud\source\repos\Nate\core\Core.h"
#include "C:\Users\ruud\source\repos\Nate\input\created\Hello.h"
class Hello::__impl
{
private:
  Hello* me;
public:
  __impl(Hello* aMe) : me(aMe) {}
		int32_t get__E_Hello_age_() const
		{
#line 1
			int32_t hi = 34;
#line 14
			std::cout << "Test ";
			return me-> age_get();
		}

		static std::string greetings_()
		{
#line 18
			return "Hello";
		}

		std::string _E_Hello_greetings_() const
		{
#line 1
			std::shared_ptr<Hello> other = Hello::new_Hello_();
#line 22
			return "Hello "+ (Hello::hi__E_Hello_(other));
		}

};

Hello::Hello()
  : _impl(new __impl(this)) {}

Hello::~Hello() { delete _impl; }

	const std::string& Hello::name_get() const
	{
#line 6 "Hello.ns"
		return name;
	}

	std::shared_ptr<Hello> Hello::new_Hello_()
	{
#line 9
		auto me = std::make_shared<Hello>();
		return me;
	}

	std::string Hello::_E_Hello__E_text_(const std::string& world) const
	{
#line 25
		return ((__impl::greetings_())+ " ")+ world;
	}

	std::string Hello::hi__E_Hello_(const std::shared_ptr<Hello>& other)
	{
#line 28
		return other->_E_Hello__E_text_(" earth");
	}

	std::string Hello::stream_out__E_Hello_() const
	{
#line 31
		return ((_impl->_E_Hello_greetings_())+ ", it's ")+ (this->_E_Hello__E_text_(" angry world"));
	}

	const int32_t Hello::age_get() const { return age; }
	const int32_t Hello::age_set(const int32_t value) { return age = value; }
	const std::string& Hello::name_set(const std::string& value) { return name = value; }
	const Hello::Data& Hello::data_get() const { return data; }
	const Hello::Data& Hello::data_set(const Hello::Data& value) { return data = value; }
	const std::string& Hello::address_get() const { return address; }
	const std::string& Hello::address_set(const std::string& value) { return address = value; }
