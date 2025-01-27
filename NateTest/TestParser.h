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
    size_t startPos = 0;
    while ((startPos = str.find(from, startPos)) != std::string::npos)
    {
        str.replace(startPos, from.length(), to);
        startPos += to.length();
    }
    return str;
}

inline std::string deleteLines(std::string str)
{
    size_t startPos = 0;
    while ((startPos = str.find("#line", startPos)) != std::string::npos)
    {
        size_t endLine = str.find('\n', startPos);
        if (endLine != std::string::npos)
        {
            str.replace(startPos, endLine + 1 - startPos, "");
        }
    }
    return str;
}

inline std::string textAfter(std::string str, const std::string& after)
{
    auto startEnd = str.find(after);
    if (startEnd != std::string::npos)
    {
        str = str.substr(startEnd + after.size());
        auto afterLine = str.find("\n");
        if (afterLine != std::string::npos)
        {
            str = str.substr(afterLine + 1);
        }
    }

    return str;
}


class TestParser : public testing::Test
{
public:
    TestParser()
    {
    }
      
    int parseProgram(const std::string& in, NateParser::FileType fileType = NateParser::FileType::Normal)
    {
        std::istringstream ins(in);
        mCoder = std::make_unique<NateCode>();
        mParser = std::make_unique<NateParser>(*mCoder, "test.nd", ins, mOutStream, fileType);
        return mParser->parse();
    }

    int parse(const std::string& in, NateParser::FileType fileType = NateParser::FileType::Normal)
    {
        std::string progIn = 
R"__(
import Nate
program:
)__" + in;
        return parseProgram(progIn, fileType);
    }


    std::string code()
    {
        mParser->code();
        return mOutStream.str();
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

    void compareWhole(std::string exp, std::string out)
    {
        EXPECT_STREQ(deleteLines(exp).c_str(), replaceAll(deleteLines(out), "\t", "  ").c_str());
    }

    void compare(std::string exp, std::string out)
    {
        std::string toTest = replaceAll(deleteLines(textAfter(out, "#define NATE_PROGRAM_START\n")), "\t", "  ");

        toTest = toTest.substr(0, toTest.size() - 3);

        EXPECT_STREQ(trimEnd(deleteLines(exp)).c_str(), toTest.c_str());
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

        while ((std::getline(in, line).good() || !line.empty()) &&
               line.substr(0,3) != "###")
        {
            outString += line + "\n";
            line.clear();
        }
    
        ASSERT_FALSE(inString.empty());

        EXPECT_EQ(0, parse(inString));
        compare(outString, code());
    }

    std::stringstream mOutStream;
    std::unique_ptr<NateParser> mParser;
    std::unique_ptr<NateCode> mCoder;
};

#define TEST_PIECE(SUBJECT, NAME) \
TEST_F(TestParser, SUBJECT##NAME) \
{ \
    testCodePiece(#NAME, in); \
}

}