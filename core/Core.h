#pragma once

#include "../utf8.h"
#include "Fraction.h"
#include "Complex.h"
#include <string>
#include <codecvt>
#include <ios>
#include <iomanip>
#include <ostream>
#include <sstream>

class Core
{
public:
	static std::wstring u16(const std::string& str);
	static utf8::iterator<std::string::const_iterator> cbegin(const std::string& aString);
	static utf8::iterator<std::string::const_iterator> cend(const std::string& aString);
	static utf8::iterator<std::string::const_iterator> find(const std::string& aString,
																											    uint32_t aChar);
	
	static void parseBaseNumber(std::string& aString);
	static std::string parseNumber(const std::string& aString);
	static std::string upperCased(const std::string& aString);
	static std::string firstOf(const std::string& aString);
	static std::string restOf(const std::string& aString);
	static std::string replaceAll(const std::string& aString, const std::string& aFrom, const std::string& aTo);
	static std::string replaceOne(const std::string& aString, const std::string& aFrom, const std::string& aTo);

	static bool strtoi32(const char* aString, int32_t& aResult, int aBase = 10);
	static bool strtoi64(const char* aString, int64_t& aResult, int aBase = 10);
	static bool strtodbl(const char* aString, double& aResult);
	static int positionIn(const std::string& aString, uint32_t aChar);
	static bool numberFrom(utf8::iterator<std::string::const_iterator>& aIter,
											 	 const utf8::iterator<std::string::const_iterator>& aEnd,
												 const std::string& aDigits,
												 int32_t& aNumber);
	static bool numberFrom(utf8::iterator<std::string::const_iterator>& aIter,
											 	 const utf8::iterator<std::string::const_iterator>& aEnd,
												 int32_t& aNumber);
	static bool numberFrom(const std::string& aString,
												 const std::string& aDigits,
												 int32_t& aNumber);
	static std::string toString(int32_t aNumber,
												      const std::string& aDigits);

	static std::string unSuperscript(const std::string& aString);
	static std::string toString(utf8::iterator<std::string::const_iterator> aIter);
	static std::string toString(const utf8::iterator<std::string::const_iterator>& aIter,
											     	  const utf8::iterator<std::string::const_iterator>& aEnd);
	static std::string toString(uint32_t aChar);
	static std::string::const_iterator lastOf(const std::string& aString);

	struct Format
	{
		int32_t width = -1;
		int32_t precision = -1;
		uint32_t fill = ' ';
		char align = '<';
		char sign = '-';
		int32_t base = 10;

		std::string toString() const;
	};
	
	static Format getFormat(const std::string& aFormat);
	
	static void setWidth(std::ostream& aStream, int32_t aWidth);
	static void setPrecision(std::ostream& aStream, int32_t aPrecision);
	static void setAlign(std::ostream& aStream, char aAlign);
	static void setSign(std::ostream& aStream, char aSign);
	static void setFill(std::ostream& aStream, uint32_t aFill);
	static void setBase(std::ostream& aStream, int32_t aBase);

	template <typename T>
	static std::string formatted(const T& aValue, const Core::Format& aFormat)
	{
		std::string result;
		//std::cerr << aValue << " " << aFormat << std::endl;

		if (((aFormat.align != '=' && aFormat.fill <= 255) || aFormat.width < 0) && aFormat.sign != ' ')
		{
			std::stringstream ss;
			setWidth(ss, aFormat.width);
			setPrecision(ss, aFormat.precision);
			setAlign(ss, aFormat.align);
			setFill(ss, aFormat.fill);
			setSign(ss, aFormat.sign);
			setBase(ss, aFormat.base);
			ss.setf(std::ios_base::fixed, std::ios_base::floatfield);
			ss.setf(std::ios_base::boolalpha);

			ss << aValue;
			result = ss.str();
			//std::cerr << ss.width() << " " << ss.precision() << " " << ss.flags() << " {" << result << "}" << std::endl;
		}
		else
		{
			Format subFormat = aFormat;
		  subFormat.width = -1;
		  subFormat.fill = ' ';
			subFormat.align = '<';
			subFormat.sign = (aFormat.sign == ' ' ? '+' : aFormat.sign);

			result = formatted(aValue, subFormat);
			//std::cerr << "|" << result << "|";

			if (aFormat.sign == ' ')
			{
				result = replaceOne(result, "+", " ");
			}
			
			std::string temp;
			auto back = std::back_inserter(temp);
			int32_t half = 0;
			int32_t otherHalf = 0;
			int32_t size = static_cast<int32_t>(result.size());
			if (aFormat.align == '=')
			{
				half = (aFormat.width - size) / 2;
				otherHalf = aFormat.width - size - half;
			}
			else if (aFormat.align == '>')
			{
				half = aFormat.width - size;
			}
			else
			{
				otherHalf = aFormat.width - size;
			}

			while (half-- > 0)
			{
				utf8::append(aFormat.fill, back);
			}
			temp += result;
				
			temp += std::string(otherHalf, ' ');
			result = std::move(temp);
			//std::cerr << result << "|" << std::endl;
		}

		return result;
	}
	
	template<typename T>
	static std::string join(const T& aBegin,
													const T& aEnd,
													const std::string& seperator = ",")
	{
		std::stringstream ss;
		ss << std::distance(aBegin, aEnd) << ':';
		bool first = true;

		for (auto iter = aBegin; iter != aEnd; ++iter)
		{
			if (!first)
			{
				ss << seperator;
			}

			ss << "(" << *iter << ")";
			first = false;
		}

		return ss.str();
	}

	template<typename CONTAINER>
	static std::string join(const CONTAINER& container, const std::string& seperator = ",")
	{
		return join(container.cbegin(), container.cend(), seperator);
	}

	// Members are all public and mutable, so if we really don't want
	// to restore any particular part of the state, we can override.
	struct SaveStreamState
	{
		SaveStreamState(std::ostream& aStream);
		// deleting copy construction also prevents move
		SaveStreamState(const SaveStreamState&) = delete;
		void operator=(const SaveStreamState&) = delete;

		~SaveStreamState();

		std::ostream&           mStream;
		std::ios_base::fmtflags mFlags;
		std::streamsize         mWidth;
		std::streamsize         mPrecision;
		char                    mFill;
	};

};

std::ostream& operator<<(std::ostream& aStream, const Core::Format& aFormat);

