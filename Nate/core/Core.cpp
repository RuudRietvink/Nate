#include "Core.h"
#include "utf8.h"
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
#include <sys/types.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#define MakeDir _mkdir
#define RemoveFile _unlink
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#define MakeDir mkdir
#define RemoveFile unlink
#endif

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

namespace Core
{
	Utf8::Utf8(const std::string::const_iterator& aBegin,
				     const std::string::const_iterator& aEnd)
		: mIter(aBegin),
		  mBegin(mIter),
		  mEnd(aEnd)
	{}
		
	Utf8::Utf8(const std::string& aString)
		: mIter(aString.cbegin()),
		  mBegin(mIter),
		  mEnd(aString.cend())
	{}

	uint32_t Utf8::operator*() const
	{
		return mIter != mEnd
			   ? utf8::peek_next(mIter, mEnd)
			   : 0;
	}

	Utf8::operator bool() const
	{
		return mIter != mEnd;
	}

	Utf8& Utf8::operator++()
	{
		utf8::next(mIter, mEnd);
		return *this;
	}
	
	Utf8 Utf8::operator++(int)
	{
		Utf8 copy = *this;
		utf8::next(mIter, mEnd);
		return copy;
	}

	std::string::const_iterator& Utf8::iter()
	{
		return mIter;
	}
	
	const std::string::const_iterator& Utf8::begin()
	{
		return mBegin;
	}
	
	const std::string::const_iterator& Utf8::end()
	{
		return mEnd;
	}

	std::string directorySeperator()
	{
		#ifdef _WIN32
			return "\\";
		#else
			return "/";
		#endif
	}
	
	std::string currentDirectory()
	{  
		char buffer[FILENAME_MAX];
		(void) GetCurrentDir(buffer, FILENAME_MAX);
		return buffer;
	}
	
	void makeDirectory(const std::string& aDirectoryName)
	{
		(void) MakeDir(aDirectoryName.c_str());
	}

	bool exists(const std::string& aFileName)
	{
		struct stat info;

		return (stat(aFileName.c_str(), &info) == 0);
	}
	
	bool isOrdinaryFile(const std::string& aFileName)
	{
		struct stat info;

		return (stat(aFileName.c_str(), &info) == 0 && (info.st_mode & S_IFREG));
	}

	bool isDirectory(const std::string& aDirectoryName)
	{
		struct stat info;

		return (stat(aDirectoryName.c_str(), &info) == 0 && (info.st_mode & S_IFDIR));
	}
	
	bool isReadable(const std::string& aFileName)
	{
		FILE *fp;
		bool result = fopen_s(&fp, aFileName.c_str(), "r") == 0;
		if (result && fp)
		{
			fclose(fp);
		}
		return result;
	}
	
	bool isWritable(const std::string& aFileName)
	{
		bool result = false;

		if (isDirectory(aFileName))
		{
			std::string tempName = aFileName + directorySeperator() + ".___temp";
			FILE *fp;
			result = fopen_s(&fp, tempName.c_str(), "w") == 0;
			if (result)
			{
				fclose(fp);
				RemoveFile(tempName.c_str());
			}
		}
		else
		{
			FILE *fp = nullptr;
			result = fopen_s(&fp, aFileName.c_str(), "r+") == 0;
			if (result)
			{
				fclose(fp);
			}
		}

		return result;
	}

    std::tuple<bool, int>  parseBaseNumber(std::string& aString)
	{
        bool ok = true;
        int length = 0;
		int base = 10;
		if (aString.size() > 2)
		{
			switch (aString[0])
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
                int ind = 1;
                for (; aString[ind] != '_'; ++ind)
                {
                    length = length * 10 + (aString[ind] - '0');
                }

				auto number = aString.substr(ind + 1); 
				int64_t value;
				ok = strtoi64(number.c_str(), value, base);
				if (ok)
				{
                    int64_t max = static_cast<int64_t>(std::pow(2, length == 0 ? 32 : length));
                    if (value > max)
                    {
                        ok = false;
                    }

                    aString = std::to_string(value);

				}
			}
		}

        return std::tie(ok, length);
	}

	std::string normalizeNumber(const std::string& aString)
	{
		std::string result = replaceAll(aString, "ю", "E");
		result = unSubscriptNumbers(result);
		return result;
	}

	std::wstring u16(const std::string& str)
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

	std::string upperCased(const std::string& aString)
	{
		std::string result;
		auto inserter = std::back_inserter(result);
		for (auto iter = cbegin(aString); iter != cend(aString); ++iter)
		{
			inserter = utf8::append(upcase(*iter), inserter);
		}
		return result;
	}

	std::string firstOf(const std::string& aString)
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

	std::string restOf(const std::string& aString)
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

	std::string::const_iterator lastOf(const std::string& aString)
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

	std::string replaceAll(const std::string& aString, const std::string& aFrom, const std::string& aTo)
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

	std::string replaceOne(const std::string& aString, const std::string& aFrom, const std::string& aTo)
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


	int find(Utf8& aString, uint32_t aChar)
	{
		int pos = 0;
		for (; aString; ++aString, ++pos)
		{
			if (*aString == aChar)
			{
				return pos;
			}
		}

		return -1;
	}

	bool strtoi32(const char* aString, int32_t& aResult, int aBase)
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

	bool strtoi64(const char* aString, int64_t& aResult, int aBase)
	{
		char *endptr = NULL;
		errno = 0;

		aResult = strtoll(aString, &endptr, aBase);
		//std::cerr << errno << " " << aString << " " << aResult << " " << (endptr - aString) << " " << (int) *endptr << std::endl;
		return !(endptr == aString || *endptr != 0 ||
				 errno != 0 || aResult == LLONG_MAX);
	}

	bool strtodbl(const char* aString, double& aResult)
	{
		char c;
		int scanned = sscanf_s(aString, "%lG%c", &aResult, &c, 1);
		return (scanned == 1);
	}

	int positionIn(const std::string& aString, uint32_t aChar)
	{
		Utf8 string(aString);
		return find(string, aChar);
	}

	bool numberFrom(Utf8& aString,
					int32_t& aNumber)
	{
		return numberFrom(aString, "0123456789", aNumber);
	}
	
	bool numberFrom(const Utf8& aString,
					const std::string& aDigits,
					int32_t& aNumber)
	{
		Utf8 string = aString;
		return numberFrom(string, aDigits, aNumber);
	}

	bool numberFrom(Utf8& aString,
					const std::string& aDigits,
					int32_t& aNumber)
	{
		bool ok = false;
		int pos = 0;
		aNumber = 0;
		std::string buf;
		
		while (aString && pos >= 0)
		{
			pos = positionIn(aDigits, *aString);
			if (pos >= 0) 
			{
				buf.push_back('0' + pos);
				++aString;
			}
		}

		if (!buf.empty())
		{
			ok = strtoi32(buf.c_str(), aNumber);
		}

		return ok;
	}

	bool numberFrom(std::string& aString,
					const std::string& aDigits,
					int32_t& aNumber)
	{
		auto iter = cbegin(aString);
		return numberFrom(Utf8(aString), aDigits, aNumber);
	}

	namespace detail
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

	std::string toString(int32_t aNumber,
						 const std::string& aDigits)
	{
		std::string result;
		auto back = std::back_inserter(result);
		detail::toString(back, aNumber, aDigits);
		return result;
	}

	std::string unSuperscript(const std::string& aString)
	{
		std::string result;
		static const std::string superScript = "⁻⁰¹²³⁴⁵⁶⁷⁸⁹ᴬᴮᴰᴱᴳᴴᴵᴶᴷᴸᴹᴺᴼᴾᴿᵀᵁⱽᵂᵃᵇᶜᵈᵉᶠᵍʰⁱʲᵏˡᵐⁿᵒᵖʳˢᵗᵘᵛʷˣʸᶻ";
		static const std::string normlScript = "-0123456789ABDEGHIJKLMNOPRTUVWabcdefghijklmnoprstuvwxyz";
	
		Core::Utf8 string(aString);
		
		for (; string; ++string)
		{
			auto pos = positionIn(superScript, *string);
			if (pos >= 0)
			{
				result.push_back(normlScript[pos]);
			}
		}

		return result;
	}
	
	std::string unSubscriptNumbers(const std::string& aString)
	{
		std::string result;
		static const std::string normlScript = "0123456789";
	
		Core::Utf8 string(aString);
		
		for (; string; ++string)
		{
			auto pos = positionIn(subDigits, *string);
			if (pos >= 0)
			{
				result.push_back(normlScript[pos]);
			}
			else
			{
				result.push_back(*string);
			}
		}

		return result;
	}

	std::string toString(utf8::iterator<std::string::const_iterator> aIter)
	{
		return std::string(aIter.base(), (++aIter).base());
	}

	std::string toString(const utf8::iterator<std::string::const_iterator>& aIter,
						 const utf8::iterator<std::string::const_iterator>& aEnd)
	{
		return std::string(aIter.base(), aEnd.base());
	}

	std::string toString(uint32_t aChar)
	{
		std::string result;
		auto back = std::back_insert_iterator<std::string>(result);
		utf8::append(aChar, back);
		return result;
	}

	std::ostream& operator<<(std::ostream& aStream, const Format& aFormat)
	{
		aStream << "Core::Format{" 
				<< aFormat.width << ", "
				<< aFormat.precision << ", "
				<< aFormat.fill << ", "
				<< aFormat.flags
				<< "} ";

		return aStream;
	}

	std::string Format::toString() const
	{
		std::stringstream ss;
		ss << *this;
		return ss.str();
	}

	using F = Format::Flags;
	using Uci = utf8::iterator<std::string::const_iterator>;

	static std::string getFlag(Utf8& aString)
	{
		std::string result;
		while (aString && *aString != ',')
		{
			result += *aString++;
		}

		return upperCased(result);
	}

	static void getFormatWidth(const std::string& aInput, Format& aFormat, 
							   Utf8& aString)
	{
		int32_t temp;
		bool ok = numberFrom(aString, temp);
		if (ok)
		{
			aFormat.width = temp;
		}
					
		if (aString)
		{
			if (*aString == '.')
			{
				++aString;
				numberFrom(aString, temp);
				aFormat.precision = temp;
			}
			else if (!ok && *aString != ',')
			{
				aFormat.setError("Missing output format width in " + aInput);
			}
		}
	}

	static bool tryGetAlign(const std::string& aInput, Format& aFormat, const std::string aFlag)
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

	static bool tryGetSign(const std::string& aInput, Format& aFormat, const std::string aFlag)
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

	static bool tryGetBase(const std::string& aInput, Format& aFormat, const std::string aFlag)
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

	static Format getFormatWithHeaders(const std::string& aInput)
	{
		Core::Utf8 input(aInput);
	
		Format result;

		while (input)
		{
			std::string header;
			while (input && *input != ',' && *input != ':')
			{
				header += *input++;
			}

			header = upperCased(header);
			if (input && *input == ':' && !header.empty())
			{
				++input;
				if (input)
				{
					if (header == "W" || header == "WIDTH")
					{
						getFormatWidth(aInput, result, input);
					}
					else if (header == "A" || header == "ALIGN")
					{
						std::string align = getFlag(input);

						if (!tryGetAlign(aInput, result, align))
						{
							result.setError("Unknown output format align in: "  + aInput);
						}
					}
					else if (header == "F" || header == "FILL")
					{
						result.fill = *input++;
					}
					else if (header == "S" || header == "SIGN")
					{
						std::string sign = getFlag(input);

						if (!tryGetSign(aInput, result, sign))
						{
							result.setError("Unknown output format sign in: "  + aInput);
						}
					}
					else if (header == "B" || header == "BASE")
					{
						std::string base = getFlag(input);

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

			if (input)
			{
				if (header.empty() && *input == ':')
				{
					result.setError("Missing output format header in: " + aInput);
					++input;
				}
				else if (*input != ',')
				{
					result.setError("Missing output format seperator in: "  + aInput);
				}
				else
				{
					++input;
				}
			}
		}

		return result;
	}

	static Format getFormatHeaderless(const std::string& aInput)
	{
		Utf8 input(aInput);
	
		Format result;
		int32_t commaCount = 0;

		if (input && *input == '[')
		{
			++input;

			while (input && *input != ']')
			{
				if (commaCount == 0)
				{
					getFormatWidth(aInput, result, input);
				}
				else if (commaCount == 1)
				{
					if (*input == '\\')
					{
						++input;
						if (input)
						{
							result.fill = *input++;
						}
						else
						{
							result.setError("Missing fill character in [] part in "  + aInput);
						}
					}
					else if (*input != ']' && *input != ',')
					{
						result.fill = *input++;
					}
				}

				if (input && *input == ',')
				{
					++input;
					++commaCount;
					if (commaCount > 1)
					{
						result.setError("Too many commas in [] part in "  + aInput);
					}
				}
				else if (input && *input != ']')
				{
					result.setError("Unexpected character in [] part in "  + aInput);
					++input;
				}
			}

			if (input && *input == ']')
			{
				++input;
			}
			else
			{
				result.setError("Missing ] in [] part in "  + aInput);
			}
		}

		while (input)
		{
			if (*input != ',' )
			{
				std::string flag = getFlag(input);

				if (!tryGetAlign(aInput, result, flag) &&
						!tryGetSign(aInput, result, flag) &&
						!tryGetBase(aInput, result, flag))
				{
					result.setError("Unknown output format flag in: "  + aInput);
				}
			}

			if (input && *input != ',')
			{
				result.setError("Expected comma in: "  + aInput);
			}

			if (input)
			{
				++input;
			}
		}

		return result;
	}

	Format getFormat(const std::string& aInput)
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

	SaveStreamState::SaveStreamState(std::ostream& aStream)
		: mStream{aStream},
		  mFlags{aStream.flags()},
		  mWidth{aStream.width()},
		  mPrecision{aStream.precision()},
		  mFill{aStream.fill()}
	{}

	SaveStreamState::~SaveStreamState()
	{
		mStream.flags(mFlags);
		mStream.width(mWidth);
		mStream.precision(mPrecision);
		mStream.fill(mFill);
	}

	void setWidth(std::ostream& aStream, int32_t aWidth)
	{
		if (aWidth >= 0)
		{
			aStream.width(aWidth);
		}
	}

	void setPrecision(std::ostream& aStream, int32_t aPrecision)
	{
		if (aPrecision >= 0)
		{
			aStream.precision(aPrecision);
		}
	}

	void setAlign(std::ostream& aStream, int32_t aAlign)
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
	
	void setSign(std::ostream& aStream, int32_t aSign)
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
	
	void setFill(std::ostream& aStream, uint32_t aFill)
	{
		aStream.fill(aFill);
	}

	void setBase(std::ostream& aStream, int32_t aBase)
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

	std::string alignValue(const std::string& aValue, const Format& aFormat)
	{
		using F = Format::Flags;

		std::string temp;
		auto back = std::back_inserter(temp);
		int32_t half = 0;
		int32_t otherHalf = 0;
		int32_t size = static_cast<int32_t>(aValue.size());
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

		temp += aValue;

		while (otherHalf-- > 0)
		{
			utf8::append(aFormat.fill, back);
		}
				
		return temp;
	}

	void setTempFormat(std::ostream& aStream, const Format& aFormat)
	{
		using F = Format::Flags;

		setPrecision(aStream, aFormat.precision);
		setAlign(aStream, F::AlignLeft);
		setSign(aStream, (aFormat.flags & F::SignMinus) ? F::SignMinus : F::SignPlusMinus);
		setBase(aStream, aFormat.flags);
		aStream.setf(std::ios_base::fixed, std::ios_base::floatfield);
		aStream.setf(std::ios_base::boolalpha);
	}
}