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

bool Core::strtoi32(const char* aString, int32_t& aResult)
{
	char c;
	int64_t temp;
	int scanned = sscanf_s(aString, "%" SCNd64 "%c", &temp, &c, 1);
	bool ok = (scanned == 1);
	if (temp > std::numeric_limits<int32_t>::max() || temp < std::numeric_limits<int32_t>::lowest())
	{
		ok = false;
	}
	else
	{
		aResult = static_cast<int32_t>(temp);
	}
		
	return ok;
}

bool Core::strtoi64(const char* aString, int64_t& aResult)
{
	char c;
	int scanned = sscanf_s(aString, "%" SCNd64 "%c", &aResult, &c, 1);
	return (scanned == 1);
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

std::ostream& operator<<(std::ostream& aStream, const Core::Format& aFormat)
{
	aStream << "Core::Format{" 
						<< aFormat.width << ", "
						<< aFormat.precision << ", "
						<< aFormat.fill << ", "
						<< "'" << aFormat.align << "',"
						<< "'" << aFormat.sign << "'"
					  << "} ";

	return aStream;
}

std::string Core::Format::toString() const
{
	std::stringstream ss;
	ss << *this;
	return ss.str();
}

Core::Format Core::getFormat(const std::string& aFormat)
{
	utf8::iterator<std::string::const_iterator> iter(aFormat.cbegin(), aFormat.cbegin(), aFormat.cend());
	utf8::iterator<std::string::const_iterator> end(aFormat.cend(), aFormat.cbegin(), aFormat.cend());
	
	Format result;
	int32_t temp;
			
	while (iter != end)
	{
		std::string header;
		while (iter != end && *iter != ',' && *iter != ':')
		{
			header += *iter++;
		}

		header = upperCased(header);
		if (*iter == ':' && !header.empty())
		{
			++iter;
			if (iter != end)
			{
				if (header == "W" || header == "WIDTH")
				{
					bool ok = numberFrom(iter, end, temp);
					if (ok)
					{
						result.width = temp;
					}
					
					if (iter != end)
					{
						if (*iter == '.')
						{
							++iter;
							numberFrom(iter, end, temp);
							result.precision = temp;
						}
						else if (!ok && *iter != '|')
						{
							std::cerr << "Missing output format width, got: " << toString(iter) << std::endl;
						}
					}
				}
				else if (header == "A" || header == "ALIGN")
				{
					std::string align;
					while (iter != end && *iter != ',')
					{
						align += *iter++;
					}

					align = upperCased(align);

					if (align == "<" || align == "L" || align == "LEFT")
					{
						result.align = '<';
					}
					else if (align == ">" || align == "R" || align == "RIGHT")
					{
						result.align = '>';
					}
					else if (align == "^" || align == "C" || align == "CENTER")
					{
						result.align = '=';
						if (result.width < 0)
						{
							std::cerr << "Center align requires width specified" << std::endl;
						}
					}
					else
					{
						std::cerr << "Unknown output format align, got: " << align << std::endl;
					}
				}
				else if (header == "F" || header == "FILL")
				{
					result.fill = *iter++;
				}
				else if (header == "S" || header == "SIGN")
				{
					std::string sign;
					while (iter != end && *iter != ',')
					{
						sign += *iter++;
					}

					sign = upperCased(sign);

					if (sign == "-" || sign == "MINUS")
					{
						result.sign = '-';
					}
					else if (sign == "+" || sign == "PLUS")
					{
						result.sign = '+';
					}
					else if (sign == " " || sign == "SPACE")
					{
						result.sign = ' ';
					}
					else
					{
						std::cerr << "Unknown output format sign, got: " << sign << std::endl;
					}
				}
				else
				{
					std::cerr << "Unknown output format header, got: " << header << std::endl;
				}
			}
			else
			{
				std::cerr << "Missing format value" << std::endl;
			}
		}
		else if (!header.empty())
		{
			std::cerr << "Missing format colon" << std::endl;
		}

		if (iter != end)
		{
			if (*iter != ',')
			{
				std::cerr << "Missing output format seperator: got: " << toString(iter) << std::endl;
			}
			else
			{
				++iter;
			}
		}
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

void Core::setAlign(std::ostream& aStream, char aAlign)
{
	if (aAlign == '<')
	{
		aStream.setf(std::ios::left);
	}
	else if (aAlign == '>')
	{
		aStream.setf(std::ios::right);
	}
}
	
void Core::setSign(std::ostream& aStream, char aSign)
{
	if (aSign == '-')
	{
		aStream.unsetf(std::ios_base::showpos);
	}
	else if (aSign == '+')
	{
		aStream.setf(std::ios_base::showpos);
	}
}
	
void Core::setFill(std::ostream& aStream, uint32_t aFill)
{
	aStream.fill(aFill);
}
