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
    parser.addVariable("x");
    parser.addVariable("var");
    parser.addVariable("z");
    ss.clear();
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

TEST_F(TestMathParser, TestPower1)
{  
  ss << R"zzz(
  2
 x
)zzz";

  EXPECT_STREQ("pow(x, 2)", parser.doMath(ss).c_str());
  
}

TEST_F(TestMathParser, TestPower2)
{  
  ss << R"zzz(
   2
 -x
)zzz";

  EXPECT_STREQ("-pow(x, 2)", parser.doMath(ss).c_str());
  
}

TEST_F(TestMathParser, TestPower3)
{  
  ss << R"zzz(
  2.5
 x
)zzz";

  EXPECT_STREQ("pow(x, 2.5)", parser.doMath(ss).c_str());

}

TEST_F(TestMathParser, TestPower4)
{  
  ss << R"zzz(
  x - 3
 x
)zzz";

  EXPECT_STREQ("pow(x, x-3)", parser.doMath(ss).c_str());
  
}

TEST_F(TestMathParser, TestPower5)
{  
  ss << R"zzz(
  -(x - 3) * 4
 x
)zzz";

  EXPECT_STREQ("pow(x, -(x-3)*4)", parser.doMath(ss).c_str());

}

TEST_F(TestMathParser, TestPower6)
{  
  ss << R"zzz(
        -(x - 3) * 4
 (x + 3)
)zzz";

  EXPECT_STREQ("pow((x+3), -(x-3)*4)", parser.doMath(ss).c_str());
  
}

TEST_F(TestMathParser, TestPower7)
{  
  ss << R"zzz(
   3
  2
 x
)zzz";

  EXPECT_STREQ("pow(x, pow(2, 3))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestPower8)
{  
  ss << R"zzz(
  2.5
 x   + 3
)zzz";

  EXPECT_STREQ("pow(x, 2.5)+3", parser.doMath(ss).c_str());

}
TEST_F(TestMathParser, TestPowerSuper1)
{
  ss << R"zzz(
x²
)zzz";

  EXPECT_STREQ("pow(x, 2)", parser.doMath(ss).c_str());
  
}

TEST_F(TestMathParser, TestPowerSuper2)
{
  ss << R"zzz(
xᶻ⁻⁵
)zzz";

  EXPECT_STREQ("pow(x, z-5)", parser.doMath(ss).c_str());
  
}

TEST_F(TestMathParser, TestPowerSuper3)
{
  ss << R"zzz(
x⁽ᶻ ⁻ ⁵⁾
)zzz";

  EXPECT_STREQ("pow(x, (z-5))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSquareRoot1)
{  
  ss.clear();
  ss << R"zzz(
√4
)zzz";

  EXPECT_STREQ("sqrt(4)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSquareRoot2)
{  
  ss << R"zzz(
√43.3E-5
)zzz";

  EXPECT_STREQ("sqrt(43.3E-5)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSquareRoot3)
{  
  ss << R"zzz(
√43.3E-5+4
)zzz";

  EXPECT_STREQ("sqrt(43.3E-5)+4", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSquareRoot4)
{  
  ss << R"zzz(
 _________
√43.3E-5+4
)zzz";

  EXPECT_STREQ("sqrt(43.3E-5+4)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSquareRoot5)
{  
  ss << R"zzz(
 _________
√43 + 42.1
)zzz";

  EXPECT_STREQ("sqrt(43+42.1)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSquareRoot6)
{  
  ss << R"zzz(
  ___
 ╱x-z  
√4   
)zzz";

  EXPECT_STREQ("sqrt(pow(4, x-z))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSquareRoot7)
{  
  ss << R"zzz(
  ___
 ╱x-z  
√4    + 3
)zzz";

  EXPECT_STREQ("sqrt(pow(4, x-z))+3", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSquareRoot8)
{  
  ss << R"zzz(
  ___________
 ╱  x-z  
√ (4    ) + 3
)zzz";

  EXPECT_STREQ("sqrt((pow(4, x-z))+3)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSquareRoot9)
{  
  ss << R"zzz(
  ___________
 ╱x-z  
√4   + 3
)zzz";

  EXPECT_STREQ("sqrt(pow(4, x-z)+3)", parser.doMath(ss).c_str());
}


TEST_F(TestMathParser, TestSquareRoot10)
{  
  ss << R"zzz(
  __________
 ╱⎛  1  ⎞
√ ⎜ ――― ⎟*2
  ⎝ x+z ⎠
)zzz";

  EXPECT_STREQ("sqrt((((1) / (x+z)))*2)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSquareRoot11)
{  
  ss << R"zzz(
√(43.3E-5+4)
)zzz";

  EXPECT_STREQ("sqrt((43.3E-5+4))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSquareRoot12)
{  
  ss << R"zzz(
 ⎛  1  ⎞
√⎜ ――― ⎟
 ⎝ x+z ⎠
)zzz";

  EXPECT_STREQ("sqrt((((1) / (x+z))))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestParens1)
{  
  ss << R"zzz(
⎛  1  ⎞
⎜ ――― ⎟
⎝ x+z ⎠
)zzz";

  EXPECT_STREQ("(((1) / (x+z)))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestParens2)
{  
  ss << R"zzz(
1 + ⎛  1  ⎞
    ⎜ ――― ⎟
    ⎝ x+z ⎠
)zzz";

  EXPECT_STREQ("1+(((1) / (x+z)))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestParens3)
{  
  ss << R"zzz(
    ⎛  1  ⎞
    ⎜ ――― ⎟
1 + ⎝ x+z ⎠
)zzz";

  EXPECT_STREQ("1+(((1) / (x+z)))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestParens4)
{  
  ss << R"zzz(
    ⎛  1  ⎞  + 1
    ⎜ ――― ⎟
    ⎝ x+z ⎠
)zzz";

  EXPECT_STREQ("(((1) / (x+z)))+1", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestParens5)
{  
  ss << R"zzz(
    ⎛  1  ⎞
    ⎜ ――― ⎟
    ⎝ x+z ⎠+1
)zzz";

  EXPECT_STREQ("(((1) / (x+z)))+1", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestParens6)
{  
  ss << R"zzz(
1+  ⎛  1  ⎞
    ⎜ ――― ⎟
    ⎝ x+z ⎠+1
)zzz";

  EXPECT_STREQ("1+(((1) / (x+z)))+1", parser.doMath(ss).c_str());
}