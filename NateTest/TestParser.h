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
  var text is text
  var int32 is int-32
  var float is float
)__" + in;
    return parseProgram(progIn, fileType);
  }


  std::string code()
  {
    std::ostringstream out;
	  NateCode coder(out, mParser.get());
    std::ostringstream out2;
	  coder.codeTreeDesc(mParser->data.stats, out2);
    return out2.str();
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
    static const std::string start =  
R"__(Code
  LocalVar output std::shared_ptr<std::ostream>={}
  LocalVar error std::shared_ptr<std::ostream>={}
  LocalVar input std::shared_ptr<std::istream>={}
  Program
    LocalVar text std::string={}
    LocalVar int32 int32_t={}
    LocalVar float float={}
)__";
    static const size_t startLen = start.size();

    EXPECT_STREQ(trimEnd(exp).c_str(), trimEnd(replaceAll(out, "\t", "  ").substr(startLen)).c_str());
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
    
    EXPECT_EQ(0, parse(inString));
    compare(outString, code());
  }

  std::stringstream mOutStream;
  std::unique_ptr<NateParser> mParser;
};