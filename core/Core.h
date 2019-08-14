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

		struct Flags
		{
			static const int AlignLeft			= 0x0001;
			static const int AlignRight			= 0x0002;
			static const int AlignCenter		= 0x0004;
			static const int Align					= AlignLeft | AlignRight | AlignCenter;
			static const int SignMinus			= 0x0008;
			static const int SignPlusMinus	= 0x0010;
			static const int SignSpaceMinus	= 0x0020;
			static const int Sign  					= SignMinus | SignPlusMinus | SignSpaceMinus;
			static const int ShowBase				= 0x0040;
			static const int Uppercase			= 0x0080;
			static const int Base10   			= 0x0100;
			static const int Base16   			= 0x0200;
			static const int Base08   			= 0x0400;
			static const int Base     			= Base10 | Base16 | Base08;
		};

		int flags = Flags::AlignLeft | Flags::SignMinus | Flags::Base10;
		std::string error;

		void setError(const std::string& aError)
		{
			if (error.empty())
			{
				error = aError;
			}
		}
		std::string toString() const;
	};
	
	static Format getFormat(const std::string& aFormat);
	
	static void setWidth(std::ostream& aStream, int32_t aWidth);
	static void setPrecision(std::ostream& aStream, int32_t aPrecision);
	static void setAlign(std::ostream& aStream, int32_t aAlign);
	static void setSign(std::ostream& aStream, int32_t aSign);
	static void setFill(std::ostream& aStream, uint32_t aFill);
	static void setBase(std::ostream& aStream, int32_t aBase);

	template <typename T>
	static std::string formatted(const T& aValue, const Core::Format& aFormat)
	{
		using F = Format::Flags;

		std::string result;

		if (!aFormat.error.empty())
		{
			std::cerr << aFormat.error << std::endl;
		}

		//std::cerr << aValue << " " << aFormat << std::endl;

		std::stringstream ss;
		setPrecision(ss, aFormat.precision);
		setAlign(ss, F::AlignLeft);
		setSign(ss, F::SignPlusMinus);
		setBase(ss, aFormat.flags);
		ss.setf(std::ios_base::fixed, std::ios_base::floatfield);
		ss.setf(std::ios_base::boolalpha);

		ss << aValue;
		result = ss.str();

		//std::cerr << "|" << result << "|";
			
		//if (aFormat.fill != ' ' || 
		//		(aFormat.flags & F::SignSpaceMinus))
		{
			if (aFormat.flags & F::SignSpaceMinus)
			{
				result = replaceOne(result, "+", " ");
			}
			
			std::string temp;
			auto back = std::back_inserter(temp);
			int32_t half = 0;
			int32_t otherHalf = 0;
			int32_t size = static_cast<int32_t>(result.size());
			if (aFormat.flags & F::AlignCenter)
			{
				half = (aFormat.width - size) / 2;
				otherHalf = aFormat.width - size - half;
			}
			else if (aFormat.flags & F::AlignRight)
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

			while (otherHalf-- > 0)
			{
				utf8::append(aFormat.fill, back);
			}
				
			result = std::move(temp);
		}

		//std::cerr << result << "|" << std::endl;

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

