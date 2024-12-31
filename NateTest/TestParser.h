#pragma once
#include "NateParser.h"
#include "NateCode.h"
#include "gtest/gtest.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <filesystem>
#include <algorithm>

namespace nate
{
extern const char* programExp;

inline std::string replaceAll(std::string str, const std::string& from, const std::string& to)
{
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

class TestParser : public testing::Test
{
public:
    TestParser()
    {
    }
    
    int parse(std::istream& in, NateParser::FileType fileType = NateParser::FileType::Normal)
    {
        mParser.reset(new NateParser("test.nd", in, mOutStream, fileType));
    }
  
    int parseProgram(const std::string& in, NateParser::FileType fileType = NateParser::FileType::Normal)
    {
        std::istringstream ins(in);
        mParser.reset(new NateParser("test.nd", ins, mOutStream, fileType));
        return mParser->parse();
    }

    int parse(const std::string& in, NateParser::FileType fileType = NateParser::FileType::Normal)
    {
        std::string progIn = 
R"__(program:
)__" + in;
        return parseProgram(progIn, fileType);
    }


    std::string code()
    {
        std::ostringstream out;
	    NateCode coder(out, mParser.get());
        coder.codeStats(mParser->getStats());
        return out.str();
    }

    std::string trimEnd(const std::string& in)
    {
        std::string result = in;
        while (!result.empty() && std::isspace(result.back()))
        {
            result = result.erase(result.size() - 1);
        }

        return result;
    }

    void compareWhole(const std::string& exp, const std::string& out)
    {
        EXPECT_STREQ(exp.c_str(), replaceAll(out, "\t", "  ").c_str());
    }

    void compare(const std::string& exp, const std::string& out)
    {
        std::string toTest = replaceAll(out, "\t", "  ");
        static const std::string programStartLine = "#define NATE_PROGRAM_START\n";
        auto startEnd = toTest.find(programStartLine);
        if (startEnd != std::string::npos)
        {
            toTest = toTest.substr(startEnd + programStartLine.size());
            auto afterLine = toTest.find("\n");
            if (afterLine != std::string::npos)
            {
                toTest = toTest.substr(afterLine + 1);
            }
        }

        toTest = toTest.substr(0, toTest.size() - 3);

        EXPECT_STREQ(trimEnd(exp).c_str(), toTest.c_str());
    }

    void testCodePiece(const std::string& testName, const std::filesystem::path& filename)
    {
        std::filesystem::path cwd = std::filesystem::current_path() / "testfiles" / filename;
        std::ifstream in(cwd.string());
        std::string inString;
        std::string outString;

        std::string line;
        while (std::getline(in, line).good() &&
               line != "###" + testName)
        {
        }

        while (std::getline(in, line).good() &&
               line != "---")
        {
            inString += line + "\n";
        }

        while (std::getline(in, line).good() &&
               line.substr(0,3) != "###")
        {
            outString += line + "\n";
        }
    
        ASSERT_FALSE(inString.empty());

        EXPECT_EQ(0, parse(inString));
        compare(outString, code());
    }

    std::stringstream mOutStream;
    std::unique_ptr<NateParser> mParser;
};

#define TEST_PIECE(SUBJECT, NAME) \
TEST_F(TestParser, SUBJECT##NAME) \
{ \
    testCodePiece(#NAME, in); \
}

}