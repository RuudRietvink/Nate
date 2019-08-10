#include "Complex.h"

bool Complex::isImaginaryLetter(uint32_t aLetter)
{	
	return (aLetter == 'i' || aLetter == 0x1D456 /*𝑖*/);
}

bool Complex::checkAndRemoveImaginaryLetter(std::string& aString)
{
  auto last = Core::lastOf(aString);
  auto lastChar = utf8::peek_next(last, aString.cend());
  bool result = Complex::isImaginaryLetter(lastChar);

	if (result)
  {
    aString.erase(last, aString.cend());
  }

	return result;
}