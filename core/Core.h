#pragma once

#include "../utf8.h"
#include <string>
#include <codecvt>
#include <ios>
#include <iomanip>
#include <ostream>
#include <sstream>

namespace Core
{
	std::wstring u16(const std::string& str);
	utf8::iterator<std::string::const_iterator> cbegin(const std::string& aString);
	utf8::iterator<std::string::const_iterator> cend(const std::string& aString);
	utf8::iterator<std::string::const_iterator> find(const std::string& aString,
																							     uint32_t aChar);
	
	void parseBaseNumber(std::string& aString);
	std::string parseNumber(const std::string& aString);
	std::string upperCased(const std::string& aString);
	std::string firstOf(const std::string& aString);
	std::string restOf(const std::string& aString);
	std::string replaceAll(const std::string& aString, const std::string& aFrom, const std::string& aTo);
	std::string replaceOne(const std::string& aString, const std::string& aFrom, const std::string& aTo);

	bool strtoi32(const char* aString, int32_t& aResult, int aBase = 10);
	bool strtoi64(const char* aString, int64_t& aResult, int aBase = 10);
	bool strtodbl(const char* aString, double& aResult);
	int positionIn(const std::string& aString, uint32_t aChar);
	bool numberFrom(utf8::iterator<std::string::const_iterator>& aIter,
									const utf8::iterator<std::string::const_iterator>& aEnd,
									const std::string& aDigits,
									int32_t& aNumber);
	bool numberFrom(utf8::iterator<std::string::const_iterator>& aIter,
									const utf8::iterator<std::string::const_iterator>& aEnd,
									int32_t& aNumber);
	bool numberFrom(const std::string& aString,
									const std::string& aDigits,
									int32_t& aNumber);
	std::string toString(int32_t aNumber,
								       const std::string& aDigits);

	std::string unSuperscript(const std::string& aString);
	std::string toString(utf8::iterator<std::string::const_iterator> aIter);
	std::string toString(const utf8::iterator<std::string::const_iterator>& aIter,
											 const utf8::iterator<std::string::const_iterator>& aEnd);
	std::string toString(uint32_t aChar);
	std::string::const_iterator lastOf(const std::string& aString);

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
	
	Format getFormat(const std::string& aFormat);
	
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

	void setWidth(std::ostream& aStream, int32_t aWidth);
	void setPrecision(std::ostream& aStream, int32_t aPrecision);
	void setAlign(std::ostream& aStream, int32_t aAlign);
	void setSign(std::ostream& aStream, int32_t aSign);
	void setFill(std::ostream& aStream, uint32_t aFill);
	void setBase(std::ostream& aStream, int32_t aBase);

	std::string alignValue(const std::string& aValue, const Core::Format& aFormat);
	void setTempFormat(std::ostream& aStream, const Core::Format& aFormat);
	
	template <typename T>
	std::string outputValue(const T& aValue, const Core::Format& aFormat)
	{
		using F = Format::Flags;

		std::stringstream ss;
		setTempFormat(ss, aFormat);
		ss << aValue;
		std::string result = ss.str();
			
		return result;
	}
		
	template <typename T>
	std::string formatted(const T& aValue, const Core::Format& aFormat)
	{
		using F = Format::Flags;

		if (!aFormat.error.empty())
		{
			std::cerr << aFormat.error << std::endl;
		}
				
		std::string result = outputValue(aValue, aFormat);
		
		if ((aFormat.flags & F::SignSpaceMinus) && result[0] == '+')
		{
			if (aFormat.fill == ' ')
			{
				result[0] = ' ';
			}
			else
			{
				result = result.substr(1);
			}
		}

		result = alignValue(result, aFormat);

		return result;
	}
	
	template<typename T>
	std::string join(const T& aBegin,
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
	std::string join(const CONTAINER& container, const std::string& seperator = ",")
	{
		return join(container.cbegin(), container.cend(), seperator);
	}
	
	std::ostream& operator<<(std::ostream& aStream, const Core::Format& aFormat);
};

