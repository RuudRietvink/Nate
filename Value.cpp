#include "Value.h"

Value::Value()
{}

const std::string& Value::name() const
{
	return args().front().word();
}

void Value::addValue(const std::string& aWord)
{
	code() += aWord;
}

std::string Value::toCodeWord(const std::string& aWord) const
{
	return aWord;
}

std::ostream& operator<<(std::ostream& aStream, const Value& aValue)
{
	aStream << "Value(";
  operator<<(aStream, static_cast<const Method&>(aValue)) << ")";
	return aStream;
}