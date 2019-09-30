#include "C:\Users\ruud\source\repos\Nate\core\Core.h"
#include "C:\Users\ruud\source\repos\Nate\input\created\Hello.h"
class Hello::__impl
{
public:
  __impl() {}
	static std::string greetings_()
	{
#line 9
		return "Hello";
	}

	std::string _E_Hello_greetings_() const
	{
#line 12
		std::shared_ptr<Hello> other = Hello::new_Hello_();
		return "Hello "+ (Hello::hi__E_Hello_((other)));
	}

};

Hello::Hello()
  : _impl(new __impl()) {}

Hello::~Hello() { delete _impl; }

std::shared_ptr<Hello> Hello::new_Hello_()
{
#line 6 "Hello.ns"
	auto me = std::make_shared<Hello>();
#line 6
	return me;
}

std::string Hello::_E_Hello__E_text_(const std::string& world) const
{
#line 16
	return ((__impl::greetings_())+ " ")+ (world);
}

std::string Hello::hi__E_Hello_(const std::shared_ptr<Hello>& other)
{
#line 19
	return (other)->_E_Hello__E_text_(" earth");
}

std::string Hello::stream_out__E_Hello_() const
{
#line 22
	return ((_impl->_E_Hello_greetings_())+ ", it's ")+ (this->_E_Hello__E_text_(" angry world"));
}

PROP_GET(Hello, std::string&, name)
PROP_SET(Hello, std::string&, name)
PROP_GET(Hello, int32_t, age)
PROP_SET(Hello, int32_t, age)
PROP_GET(Hello, std::string&, address)
PROP_SET(Hello, std::string&, address)
PROP_GET(Hello, Hello::Data&, data)
PROP_SET(Hello, Hello::Data&, data)
