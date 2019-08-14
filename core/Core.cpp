#include "Core.h"
#include "../utf8.h"
#include "UpperLower.h"

#define NOMINMAX  
#include <windows.h>
#include <stringapiset.h>
#include <algorithm>
#include <locale>
#include <cctype>
#include <vector>
#include <iostream>
#include <iterator>
#include <errno.h>

namespace
{
	uint32_t upcase(uint32_t aChar)
	{
		uint32_t result = aChar;
	  auto iter = std::find(lowers.cbegin(), lowers.cend(), aChar);
		if (iter != lowers.cend())
		{
			result = uppers[std::distance(lowers.cbegin(), iter)];
		}

		return result;
	}
}

void Core::parseBaseNumber(std::string& aString)
{
	int base = 10;
  if (aString.size() > 2 && aString[0] == '0')
  {
		switch (aString[1])
		{
		case 'b': case 'B':
			base = 2;
			break;
		case 'o': case 'O':
			base = 8;
			break;
		case 'x': case 'X':
			base = 16;
			break;
		default:
			break;
		}

		if (base != 10)
		{
			auto number = aString.substr(2); 
			int64_t temp;
			bool ok = strtoi64(number.c_str(), temp, base);
			if (ok)
			{
				aString = std::to_string(temp);
			}
		}
  }
}

std::string Core::parseNumber(const std::string& aString)
{
	std::string result = replaceAll(aString, "⏨", "E");
	result = replaceAll(result, "*^", "E");
	result = replaceAll(result, "ю", "E");
	return result;
}

std::wstring Core::u16(const std::string& str)
{
	std::wstring convertedString;
	int requiredSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, 0, 0);
	if (requiredSize > 0)
	{
		std::vector<wchar_t> buffer(requiredSize);
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &buffer[0], requiredSize);
		convertedString.assign(buffer.begin(), buffer.end() - 1);
	}

	return convertedString;
}

std::string Core::upperCased(const std::string& aString)
{
  std::string result;
	auto inserter = std::back_inserter(result);
  for (auto iter = cbegin(aString); iter != cend(aString); ++iter)
	{
		inserter = utf8::append(upcase(*iter), inserter);
	}
  return result;
}

std::string Core::firstOf(const std::string& aString)
{
	std::string result;

	if (!aString.empty())
	{
		std::string::const_iterator iter = aString.cbegin();
		utf8::next(iter, aString.cend());
		result = std::string(aString.cbegin(), iter);
	}

	return result;
}

std::string Core::restOf(const std::string& aString)
{
	std::string result;

	if (!aString.empty())
	{
		std::string::const_iterator iter = aString.cbegin();
		utf8::next(iter, aString.cend());
		result = std::string(iter, aString.cend());
	}

	return result;
}

std::string::const_iterator Core::lastOf(const std::string& aString)
{
  auto iter = aString.cbegin();
  auto prev = iter;
  while (iter != aString.cend())
	{
		prev = iter;
		utf8::next(iter, aString.cend());
	}

	return prev;
}

std::string Core::replaceAll(const std::string& aString, const std::string& aFrom, const std::string& aTo)
{
  std::string result = aString;

  if (!aFrom.empty())
  {
    size_t start = 0;
    while ((start = result.find(aFrom, start)) != std::string::npos)
    {
      result.replace(start, aFrom.length(), aTo);
      start += aTo.length();
    }
  }

  return result;
}

std::string Core::replaceOne(const std::string& aString, const std::string& aFrom, const std::string& aTo)
{
  std::string result = aString;

  if (!aFrom.empty())
  {
    size_t start = 0;
    if ((start = result.find(aFrom, start)) != std::string::npos)
    {
      result.replace(start, aFrom.length(), aTo);
    }
  }

  return result;
}

utf8::iterator<std::string::const_iterator> Core::cbegin(const std::string& aString)
{
	return utf8::iterator<std::string::const_iterator>(aString.cbegin(), aString.cbegin(), aString.cend());
}

utf8::iterator<std::string::const_iterator> Core::cend(const std::string& aString)
{
	return utf8::iterator<std::string::const_iterator>(aString.cend(), aString.cbegin(), aString.cend());
}

utf8::iterator<std::string::const_iterator> Core::find(const std::string& aString,
																								       uint32_t aChar)
{
	utf8::iterator<std::string::const_iterator> iter(cbegin(aString));
	utf8::iterator<std::string::const_iterator> end(cend(aString));
		
	for (; iter != end; ++iter)
	{
		if (*iter == aChar)
		{
			return iter;
		}
	}

	return end;
}

bool Core::strtoi32(const char* aString, int32_t& aResult, int aBase)
{
	int64_t temp;
	bool ok = strtoi64(aString, temp, aBase);

	if (temp > std::numeric_limits<int32_t>::max())
	{
		ok = false;
	}
	else
	{
		aResult = static_cast<int32_t>(temp);
	}
		
	return ok;
}

bool Core::strtoi64(const char* aString, int64_t& aResult, int aBase)
{
	char *endptr = NULL;
  errno = 0;

  aResult = strtoll(aString, &endptr, aBase);
	//std::cerr << errno << " " << aString << " " << aResult << " " << (endptr - aString) << " " << (int) *endptr << std::endl;
  return !(endptr == aString || *endptr != 0 ||
					 errno != 0 || aResult == LLONG_MAX);
}

bool Core::strtodbl(const char* aString, double& aResult)
{
	char c;
	int scanned = sscanf_s(aString, "%lG%c", &aResult, &c, 1);
	return (scanned == 1);
}

int Core::positionIn(const std::string& aString, uint32_t aChar)
{
	int result = -1;

	auto iter = Core::find(aString, aChar);
	if (iter != Core::cend(aString))
	{
		result = static_cast<int>(std::distance(Core::cbegin(aString), iter));
	}

	return result;
}

bool Core::numberFrom(utf8::iterator<std::string::const_iterator>& aIter,
											const utf8::iterator<std::string::const_iterator>& aEnd,
											int32_t& aNumber)
{
	return numberFrom(aIter, aEnd, "0123456789", aNumber);
}

bool Core::numberFrom(utf8::iterator<std::string::const_iterator>& aIter,
											const utf8::iterator<std::string::const_iterator>& aEnd,
											const std::string& aDigits,
											int32_t& aNumber)
{
	bool ok = false;
	int pos = 0;
	aNumber = 0;
	std::string buf;

	while (aIter != aEnd && pos >= 0)
	{
		pos = positionIn(aDigits, *aIter);
		if (pos >= 0) 
		{
			buf.push_back('0' + pos);
			++aIter;
		}
	}

	if (!buf.empty())
	{
		ok = strtoi32(buf.c_str(), aNumber);
	}

	return ok;
}

bool Core::numberFrom(const std::string& aString,
											const std::string& aDigits,
											int32_t& aNumber)
{
	auto iter = cbegin(aString);
	return numberFrom(iter, cend(aString), aDigits, aNumber);
}

namespace
{
	void toString(std::back_insert_iterator<std::string>& aIter,
								int32_t aNumber,
								const std::string& aDigits)
	{

		if (aNumber > 0)
		{
			auto digIter = aDigits.cbegin();
			utf8::advance(digIter, aNumber % 10, aDigits.cend());
			toString(aIter, aNumber / 10, aDigits);
			utf8::append(utf8::next(digIter, aDigits.cend()), aIter);
		}
	}
}

std::string Core::toString(int32_t aNumber,
												   const std::string& aDigits)
{
	std::string result;
	auto back = std::back_inserter(result);
	::toString(back, aNumber, aDigits);
	return result;
}

std::string Core::unSuperscript(const std::string& aString)
{
	std::string result;
	static const std::string superScript = "⁻⁰¹²³⁴⁵⁶⁷⁸⁹ᴬᴮᴰᴱᴳᴴᴵᴶᴷᴸᴹᴺᴼᴾᴿᵀᵁⱽᵂᵃᵇᶜᵈᵉᶠᵍʰⁱʲᵏˡᵐⁿᵒᵖʳˢᵗᵘᵛʷˣʸᶻ";
	static const std::string normlScript = "-0123456789ABDEGHIJKLMNOPRTUVWabcdefghijklmnoprstuvwxyz";
	
	utf8::iterator<std::string::const_iterator> iter(cbegin(aString));
	utf8::iterator<std::string::const_iterator> end(cend(aString));
		
	for (; iter != end; ++iter)
	{
		auto pos = positionIn(superScript, *iter);
		if (pos >= 0)
		{
			result.push_back(normlScript[pos]);
		}
	}

	//std::cerr << aString << " -> " << result << std::endl; 
	return result;
}

std::string Core::toString(utf8::iterator<std::string::const_iterator> aIter)
{
	return std::string(aIter.base(), (++aIter).base());
}

std::string Core::toString(const utf8::iterator<std::string::const_iterator>& aIter,
											     const utf8::iterator<std::string::const_iterator>& aEnd)
{
	return std::string(aIter.base(), aEnd.base());
}

std::string Core::toString(uint32_t aChar)
{
	std::string result;
	auto back = std::back_insert_iterator<std::string>(result);
	utf8::append(aChar, back);
	return result;
}

std::ostream& operator<<(std::ostream& aStream, const Core::Format& aFormat)
{
	aStream << "Core::Format{" 
						<< aFormat.width << ", "
						<< aFormat.precision << ", "
						<< aFormat.fill << ", "
						<< aFormat.flags
					  << "} ";

	return aStream;
}

std::string Core::Format::toString() const
{
	std::stringstream ss;
	ss << *this;
	return ss.str();
}

using F = Core::Format::Flags;
using Uci = utf8::iterator<std::string::const_iterator>;

static std::string getFlag(Uci& aIter, const Uci& aEnd)
{
	std::string result;
	while (aIter != aEnd && *aIter != ',')
	{
		result += *aIter++;
	}

	return Core::upperCased(result);
}

static void getFormatWidth(const std::string& aInput, Core::Format& aFormat, Uci& aIter, const Uci& aEnd)
{
	int32_t temp;
	bool ok = Core::numberFrom(aIter, aEnd, temp);
	if (ok)
	{
		aFormat.width = temp;
	}
					
	if (aIter != aEnd)
	{
		if (*aIter == '.')
		{
			++aIter;
			Core::numberFrom(aIter, aEnd, temp);
			aFormat.precision = temp;
		}
		else if (!ok && *aIter != ',')
		{
			aFormat.setError("Missing output format width in " + aInput);
		}
	}
}

static bool tryGetAlign(const std::string& aInput, Core::Format& aFormat, const std::string aFlag)
{
	bool ok = true;
	int flags = 0;

	if (aFlag == "<" || aFlag == "LT" || aFlag == "LEFT")
	{
		flags |= F::AlignLeft;
	}
	else if (aFlag == ">" || aFlag == "RT" || aFlag == "RIGHT")
	{
		flags |= F::AlignRight;
	}
	else if (aFlag == "^" || aFlag == "CR" || aFlag == "CENTER")
	{
		flags |= F::AlignCenter;

		if (aFormat.width < 0)
		{
			aFormat.setError("Center align requires width specified in: " + aInput);
		}
	}
	else
	{
		ok = false;
	}

	if (ok)
	{
		aFormat.flags &= ~F::Align;
		aFormat.flags |= flags;
	}

	//std::cerr << "Align " << flags << std::endl;
	return ok;
}

static bool tryGetSign(const std::string& aInput, Core::Format& aFormat, const std::string aFlag)
{
	bool ok = true;
	int flags = 0;
	
	if (aFlag == "-" || aFlag == "MS" || aFlag == "MINUS")
	{
		flags |= F::SignMinus;
	}
	else if (aFlag == "+" || aFlag == "+-" || aFlag == "PM" || aFlag == "PLUSMINUS")
	{
		flags |= F::SignPlusMinus;
	}
	else if (aFlag == " " || aFlag == " -" || aFlag == "SM" || aFlag == "SPACEMINUS")
	{
		flags |= F::SignSpaceMinus;
	}
	else
	{
		ok = false;
	}

	if (ok)
	{
		aFormat.flags &= ~F::Sign;
		aFormat.flags |= flags;
	}
	
	//std::cerr << "Sign " << flags << std::endl;
	return ok;
}

static bool tryGetBase(const std::string& aInput, Core::Format& aFormat, const std::string aFlag)
{
	bool ok = true;
	int flags = 0;

	if (aFlag == "10" || aFlag == "B10" || aFlag == "DL"|| aFlag == "DECIMAL")
	{
		flags |= F::Base10;
	}
	else if (aFlag == "8" || aFlag == "B8" || aFlag == "OL"|| aFlag == "OCTAL")
	{
		flags |= F::Base08;
	}
	else if (aFlag == "16" || aFlag == "B16" || aFlag == "HL"|| aFlag == "HEXADECIMAL")
	{
		flags |= F::Base16;
	}
	else if (aFlag == "2" || aFlag == "B2" || aFlag == "BY" || aFlag == "BINARY")
	{
		aFormat.setError("Binary output format base is not supported in: " + aInput);
	}
	else
	{
		ok = false;
	}

	if (ok)
	{
		aFormat.flags &= ~F::Base;
		aFormat.flags |= flags;
	}

	if (aFlag == "UC" || aFlag == "UPPERCASE")
	{
		aFormat.flags |= F::Uppercase;
		ok = true;
	}
	else if (aFlag == "0X" || aFlag == "SB" || aFlag == "SHOWBASE")
	{
		aFormat.flags |= F::ShowBase;
		ok = true;
	}
	
	//std::cerr << "Base " << flags << std::endl;
	return ok;
}

static Core::Format getFormatWithHeaders(const std::string& aInput)
{
	Uci iter(aInput.cbegin(), aInput.cbegin(), aInput.cend());
	Uci end(aInput.cend(), aInput.cbegin(), aInput.cend());
	
	Core::Format result;

	while (iter != end)
	{
		std::string header;
		while (iter != end && *iter != ',' && *iter != ':')
		{
			header += *iter++;
		}

		header = Core::upperCased(header);
		if (iter != end && *iter == ':' && !header.empty())
		{
			++iter;
			if (iter != end)
			{
				if (header == "W" || header == "WIDTH")
				{
					getFormatWidth(aInput, result, iter, end);
				}
				else if (header == "A" || header == "ALIGN")
				{
					std::string align = getFlag(iter, end);

					if (!tryGetAlign(aInput, result, align))
					{
						result.setError("Unknown output format align in: "  + aInput);
					}
				}
				else if (header == "F" || header == "FILL")
				{
					result.fill = *iter++;
				}
				else if (header == "S" || header == "SIGN")
				{
					std::string sign = getFlag(iter, end);

					if (!tryGetSign(aInput, result, sign))
					{
						result.setError("Unknown output format sign in: "  + aInput);
					}
				}
				else if (header == "B" || header == "BASE")
				{
					std::string base = getFlag(iter, end);

					if (!tryGetBase(aInput, result, base))
					{
						result.setError("Unknown output format base in: "  + aInput);
					}
				}
				else
				{
					result.setError("Unknown output format header in: "  + aInput);
				}
			}
			else
			{
				result.setError("Missing format value in: "  + aInput);
			}
		}
		else if (!header.empty())
		{
			result.setError("Missing format colon in: "  + aInput);
		}

		if (iter != end)
		{
			if (header.empty() && *iter == ':')
			{
				result.setError("Missing output format header in: " + aInput);
				++iter;
			}
			else if (*iter != ',')
			{
				result.setError("Missing output format seperator in: "  + aInput);
			}
			else
			{
				++iter;
			}
		}
	}

	return result;
}

static Core::Format getFormatHeaderless(const std::string& aInput)
{
	Uci iter(aInput.cbegin(), aInput.cbegin(), aInput.cend());
	Uci end(aInput.cend(), aInput.cbegin(), aInput.cend());
	
	Core::Format result;
	int32_t commaCount = 0;

	if (iter != end && *iter == '[')
	{
		++iter;

		while (iter != end && *iter != ']')
		{
			if (commaCount == 0)
			{
				getFormatWidth(aInput, result, iter, end);
			}
			else if (commaCount == 1)
			{
				if (*iter == '\\')
				{
					++iter;
					if (iter != end)
					{
						result.fill = *iter++;
					}
					else
					{
						result.setError("Missing fill character in [] part in "  + aInput);
					}
				}
				else if (*iter != ']' && *iter != ',')
				{
					result.fill = *iter++;
				}
			}

			if (iter != end && *iter == ',')
			{
			  ++iter;
				++commaCount;
				if (commaCount > 1)
				{
					result.setError("Too many commas in [] part in "  + aInput);
				}
			}
			else if (iter != end && *iter != ']')
			{
				result.setError("Unexpected character in [] part in "  + aInput);
			  ++iter;
			}
		}

		if (iter != end && *iter == ']')
		{
			++iter;
		}
		else
		{
			result.setError("Missing ] in [] part in "  + aInput);
		}
	}

	while (iter != end)
	{
		if (*iter != ',' )
		{
			std::string flag = getFlag(iter, end);

			if (!tryGetAlign(aInput, result, flag) &&
					!tryGetSign(aInput, result, flag) &&
					!tryGetBase(aInput, result, flag))
			{
				result.setError("Unknown output format flag in: "  + aInput);
			}
		}

		if (iter != end && *iter != ',')
		{
			result.setError("Expected comma in: "  + aInput);
		}

		if (iter != end)
		{
			++iter;
		}
	}

	return result;
}

Core::Format Core::getFormat(const std::string& aInput)
{
	Format result;
			
	size_t colon = aInput.find(':');
	if (colon == std::string::npos ||
			(colon == aInput.size() - 1 && aInput[colon+1] == ',') ||
			(colon != 0 && aInput[colon-1] == '\\'))
	{
		result = getFormatHeaderless(aInput);
	}
	else
	{
		result = getFormatWithHeaders(aInput);
	}

	return result;
}

Core::SaveStreamState::SaveStreamState(std::ostream& aStream)
: mStream{aStream},
	mFlags{aStream.flags()},
	mWidth{aStream.width()},
	mPrecision{aStream.precision()},
	mFill{aStream.fill()}
{}

Core::SaveStreamState::~SaveStreamState()
{
	mStream.flags(mFlags);
	mStream.width(mWidth);
	mStream.precision(mPrecision);
	mStream.fill(mFill);
}

void Core::setWidth(std::ostream& aStream, int32_t aWidth)
{
	if (aWidth >= 0)
	{
		aStream.width(aWidth);
	}
}

void Core::setPrecision(std::ostream& aStream, int32_t aPrecision)
{
	if (aPrecision >= 0)
	{
		aStream.precision(aPrecision);
	}
}

void Core::setAlign(std::ostream& aStream, int32_t aAlign)
{
	if (aAlign & F::AlignLeft)
	{
		aStream.setf(std::ios::left);
	}
	else if (aAlign & F::AlignRight)
	{
		aStream.setf(std::ios::right);
	}
}
	
void Core::setSign(std::ostream& aStream, int32_t aSign)
{
	if (aSign & F::SignMinus)
	{
		aStream.unsetf(std::ios_base::showpos);
	}
	else if (aSign & F::SignPlusMinus)
	{
		aStream.setf(std::ios_base::showpos);
	}
}
	
void Core::setFill(std::ostream& aStream, uint32_t aFill)
{
	aStream.fill(aFill);
}

void Core::setBase(std::ostream& aStream, int32_t aBase)
{
	if (aBase & F::Base10)
	{
		aStream << std::setbase(10);
	}
	else if (aBase & F::Base16)
	{
		aStream << std::setbase(16);
	}
	else if (aBase & F::Base08)
	{
		aStream << std::setbase(8);
	}

	if (aBase & F::ShowBase)
	{
		aStream << std::showbase;
	}

	if (aBase & F::Uppercase)
	{
		aStream << std::uppercase;
	}
}
