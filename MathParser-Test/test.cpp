#include "MathParser.h"
#include <sstream>
#include <string>
#include "gtest/gtest.h"

class TestMathParser : public ::testing::Test
{

 protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  TestMathParser()
  {
  }

  ~TestMathParser() override
  {
  }

  void SetUp() override
  {
  }

  void TearDown() override
  {
  }
  
  std::stringstream ss;
  MathParser parser;
};

TEST_F(TestMathParser, TestPower)
{
  std::stringstream ss;
  MathParser parser;
  
  ss.clear();
  ss << R"zzz(
  2
 x
)zzz";

  EXPECT_STREQ("pow(x, 2)", parser.doMath(ss).c_str());

  ss.clear();
  ss << R"zzz(
   2
 -x
)zzz";

  EXPECT_STREQ("-pow(x, 2)", parser.doMath(ss).c_str());
  
  ss.clear();
  ss << R"zzz(
  2.5
 x
)zzz";

  EXPECT_STREQ("pow(x, 2.5)", parser.doMath(ss).c_str());

  ss.clear();
  ss << R"zzz(
  x - 3
 x
)zzz";

  EXPECT_STREQ("pow(x, x-3)", parser.doMath(ss).c_str());

  ss.clear();
  ss << R"zzz(
  -(x - 3) * 4
 x
)zzz";

  EXPECT_STREQ("pow(x, -(x-3)*4)", parser.doMath(ss).c_str());

  ss.clear();
  ss << R"zzz(
        -(x - 3) * 4
 (x + 3)
)zzz";

  EXPECT_STREQ("pow((x+3), -(x-3)*4)", parser.doMath(ss).c_str());

  ss.clear();
  ss << R"zzz(
   3
  2
 x
)zzz";

  EXPECT_STREQ("pow(x, pow(2, 3))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestPowerSuper)
{
  ss.clear();
  ss << R"zzz(
x²
)zzz";

  EXPECT_STREQ("pow(x, 2)", parser.doMath(ss).c_str());

  ss.clear();
  ss << R"zzz(
xᶻ⁻⁵
)zzz";

  EXPECT_STREQ("pow(x, z-5)", parser.doMath(ss).c_str());

  ss.clear();
  ss << R"zzz(
x⁽ᶻ ⁻ ⁵⁾
)zzz";

  EXPECT_STREQ("pow(x, (z-5))", parser.doMath(ss).c_str());

}

TEST_F(TestMathParser, TestParens)
{  
  ss.clear();
  ss << R"zzz(
⎛  1  ⎞
⎜ ――― ⎟
⎝ x+z ⎠
)zzz";

  EXPECT_STREQ("(((1) / (x+z)))", parser.doMath(ss).c_str());
}