#pragma once

#include "../utf8.h"
#include "Fraction.h"
#include <string>
#include <codecvt>
#include <ios>
#include <iomanip>
#include <ostream>

class Core
{
public:
	static std::wstring u16(const std::string& str);
	static utf8::iterator<std::string::const_iterator> cbegin(const std::string& aString);
	static utf8::iterator<std::string::const_iterator> cend(const std::string& aString);
	static utf8::iterator<std::string::const_iterator> find(const std::string& aString,
																											    uint32_t aChar);

	static std::string upperCased(const std::string& aString);
	static std::string firstOf(const std::string& aString);
	static std::string restOf(const std::string& aString);

	static bool strtoi32(const char* aString, int32_t& aResult);
	static bool strtoi64(const char* aString, int64_t& aResult);
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
	static std::string unSuperscript(const std::string& aString);
	static std::string toString(utf8::iterator<std::string::const_iterator> aIter);
	static std::string toString(const utf8::iterator<std::string::const_iterator>& aIter,
											     	  const utf8::iterator<std::string::const_iterator>& aEnd);

	struct Format
	{
		int32_t width = -1;
		int32_t precision = -1;
		uint32_t fill = ' ';
		char justify = '<';
	};


	static Format getFormat(const std::string& aFormat);
	
	static void setWidth(std::ostream& aStream, int32_t aWidth);
	static void setPrecision(std::ostream& aStream, int32_t aPrecision);
	static void setJustify(std::ostream& aStream, char aJustify);
	static void setFill(std::ostream& aStream, uint32_t aFill);

	template <typename T>
	static void outputFormatted(std::ostream& aStream, const T& aValue, const Core::Format& aFormat)
	{
		setWidth(aStream, aFormat.width);
		setPrecision(aStream, aFormat.precision);

		if (aFormat.justify != '=' && aFormat.fill <= 255)
		{
			setJustify(aStream, aFormat.justify);
			setFill(aStream, aFormat.fill);

			aStream << aValue;
		}
		else
		{
		}
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

