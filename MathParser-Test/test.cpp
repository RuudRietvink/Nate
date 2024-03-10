#include "CppMathParser.h"
#include <sstream>
#include <string>
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using ::testing::_;

class MockMathParser : public nate::CppMathParser
{
public:      
  MOCK_CONST_METHOD2(error, void(const nate::InputPosition&, const std::string&));
};

class ErrorMathParser : public nate::CppMathParser
{
public:
      
  void error(const nate::InputPosition& aPosition, const std::string& aError) const override
  {
	  std::cerr << "(" << aPosition.y << "," << aPosition.x << "): " << aError << std::endl;
  }
};

class TestMathParser : public ::testing::Test
{

 protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  TestMathParser()
  {
    addVariables(parser);
    addVariables(errorParser);
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
  
  void addVariables(nate::MathParser& aParser)
  {
    aParser.addVariable("x", "x");
    aParser.addVariable("x₂", "x_2");
    aParser.addVariable("H₂O", "H2O");
    aParser.addVariable("var", "var");
    aParser.addVariable("z", "z");
    aParser.addVariable("z_π", "z_pi");
    aParser.addVariable("a", "a");
    aParser.addVariable("b", "b");
    aParser.addVariable("c", "c");
  }

  std::stringstream ss;
  MockMathParser parser;
  ErrorMathParser errorParser;
};

TEST_F(TestMathParser, TestAddVariable)
{
  parser.init();
  EXPECT_CALL(parser, error(_, "Duplicate name: i"));
  EXPECT_CALL(parser, error(_, "Duplicate name: 𝑖"));
  EXPECT_CALL(parser, error(_, "Duplicate name: e"));
  EXPECT_CALL(parser, error(_, "Duplicate name: π"));
  EXPECT_CALL(parser, error(_, "Duplicate name: pi"));

  parser.addVariable("i", "i");
  parser.addVariable("𝑖", "i");
  parser.addVariable("e", "e");
  parser.addVariable("π", "pi");
  parser.addVariable("pi", "pi");
  parser.addVariable("E", "E");
  parser.addVariable("ij", "ij");
  parser.addVariable("I", "I");
}

TEST_F(TestMathParser, TestSimple01)
{  
  ss << R"zzz(
 - x+2*-var/+2.3
)zzz";

  EXPECT_STREQ("-x + ((2 * -var) / (+2.3))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSimple02)
{  
  ss << R"zzz(
 - (x₂- z)
)zzz";

  EXPECT_STREQ("-(x_2 - z)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSimple03)
{  
  ss << R"zzz(
(43.3E-5+4)
)zzz";

  EXPECT_STREQ("(43.3E-5 + 4)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSimple04)
{  
  ss << R"zzz(
  2
(x )
)zzz";

  EXPECT_STREQ("(std::pow(x, 2))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSimple05)
{  
  ss << R"zzz(
 1
(― +2)*3
 x
)zzz";

  EXPECT_STREQ("((1 / x) + 2) * 3", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSimple06)
{  
  ss << R"zzz(
 1              2
(― +(2/H₂O) + (x -z_π))*3
 x
)zzz";

  EXPECT_STREQ("((1 / x) + ((2 / H2O)) + (std::pow(x, 2) - z_pi)) * 3", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSimple07)
{  
  ss << R"zzz(
 - (x₂[z-1]- z)
)zzz";

  EXPECT_STREQ("-(x_2[z - 1] - z)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSimple08)
{  
  ss << R"zzz(
 - (⌊z-1⌋- z)
)zzz";

  EXPECT_STREQ("-(std::ceil(z - 1) - z)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSimple09)
{  
  ss << R"zzz(
 - (⌈⌊z⌋-1⌉- z)
)zzz";

  EXPECT_STREQ("-(std::floor(std::ceil(z) - 1) - z)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSimple10)
{  
  ss << R"zzz(
   |x|
)zzz";

  EXPECT_STREQ("std::abs(x)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSimple11)
{  
  ss << R"zzz(
   ||x||
)zzz";

  EXPECT_STREQ("std::abs(std::abs(x))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSimple12)
{  
  ss << R"zzz(
   | |x| |
)zzz";

  EXPECT_STREQ("std::abs(std::abs(x))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSimple13)
{  
  ss << R"zzz(
   |x + 1 | * | z-2|
)zzz";

  EXPECT_STREQ("std::abs(x + 1) * std::abs(z - 2)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSimple14)
{  
  ss << R"zzz(
   |-|x-1| * |z||
)zzz";

  EXPECT_STREQ("std::abs(-std::abs(x - 1) * std::abs(z))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSimple15)
{  
  ss << R"zzz(
   x|z|x
)zzz";

  EXPECT_STREQ("((x*std::abs(z))*x)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSimple16)
{  
  ss << R"zzz(
   |-|x+ |-z *|-2 /|(x+1)|||||
)zzz";

  EXPECT_STREQ("std::abs(-std::abs(x + std::abs(-z * std::abs(((-2) / (std::abs((x + 1))))))))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSimple17)
{  
  ss << R"zzz(
   x|z
)zzz";
  
  EXPECT_CALL(parser, error(_, "unbalanced vertical bars"));
  parser.doMath(ss);
}
TEST_F(TestMathParser, TestPower01)
{  
  ss << R"zzz(
  2
 x
)zzz";

  EXPECT_STREQ("std::pow(x, 2)", parser.doMath(ss).c_str());
  
}

TEST_F(TestMathParser, TestPower02)
{  
  ss << R"zzz(
     2
 -var
)zzz";

  EXPECT_STREQ("-std::pow(var, 2)", parser.doMath(ss).c_str());
  
}

TEST_F(TestMathParser, TestPower03)
{  
  ss << R"zzz(
  2.5
 x
)zzz";

  EXPECT_STREQ("std::pow(x, 2.5)", parser.doMath(ss).c_str());

}

TEST_F(TestMathParser, TestPower04)
{  
  ss << R"zzz(
  var - 3
 x
)zzz";

  EXPECT_STREQ("std::pow(x, var - 3)", parser.doMath(ss).c_str());
  
}

TEST_F(TestMathParser, TestPower05)
{  
  ss << R"zzz(
  -(x - 3) * 4
 x
)zzz";

  EXPECT_STREQ("std::pow(x, -(x - 3) * 4)", parser.doMath(ss).c_str());

}

TEST_F(TestMathParser, TestPower06)
{  
  ss << R"zzz(
        -(x - 3) * 4
 (x + 3)
)zzz";

  EXPECT_STREQ("std::pow((x + 3), -(x - 3) * 4)", parser.doMath(ss).c_str());
  
}

TEST_F(TestMathParser, TestPower07)
{  
  ss << R"zzz(
   3
  2
 x
)zzz";

  EXPECT_STREQ("std::pow(x, std::pow(2, 3))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestPower08)
{  
  ss << R"zzz(
  2.5
 x   + 3
)zzz";

  EXPECT_STREQ("std::pow(x, 2.5) + 3", parser.doMath(ss).c_str());
  }

TEST_F(TestMathParser, TestExponential01)
{  
  ss << R"zzz(
  x
 e
)zzz";

  EXPECT_STREQ("std::exp(x)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestExponential02)
{  
  ss << R"zzz(
  x
 e
𝑒
)zzz";

  EXPECT_STREQ("std::exp(std::exp(x))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestPowerSuper01)
{
  ss << R"zzz(
x²
)zzz";

  EXPECT_STREQ("std::pow(x, 2)", parser.doMath(ss).c_str());
  
}

TEST_F(TestMathParser, TestPowerSuper02)
{
  ss << R"zzz(
xᶻ⁻⁵
)zzz";

  EXPECT_STREQ("std::pow(x, z - 5)", parser.doMath(ss).c_str());
  
}

TEST_F(TestMathParser, TestPowerSuper03)
{
  ss << R"zzz(
x⁽ᶻ ⁻ ⁵⁾
)zzz";

  EXPECT_STREQ("std::pow(x, (z - 5))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestPowerSuper04)
{
  ss << R"zzz(
  x
⁵⁵
)zzz";

  EXPECT_STREQ("std::pow(55, x)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestPowerSuper05)
{
  ss << R"zzz(
           e⁵
          e
     x - 2
  z⁵⁵
xz
)zzz";

  EXPECT_STREQ("(x*std::pow(z, std::pow(z, std::pow(55, x - std::pow(2, std::exp(std::exp(5)))))))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestPowerSuper06)
{
  ss << R"zzz(
  xz⁵⁵
)zzz";

  EXPECT_STREQ("(x*std::pow(z, 55))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestPowerSuper07)
{
  ss << R"zzz(
        z
  x * ⁵⁵
)zzz";

  // ERROR 55 superscript only for exponents!!!!!
  EXPECT_STREQ("std::pow(55, z)x*", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestPowerSuper08)
{
  ss << R"zzz(
         z
  x * z⁵⁵
)zzz";

  EXPECT_STREQ("x * std::pow(z, std::pow(55, z))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSquareRoot01)
{  
  ss.clear();
  ss << R"zzz(
√4
)zzz";

  EXPECT_STREQ("std::sqrt(4)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSquareRoot02)
{  
  ss << R"zzz(
√43.3E-5
)zzz";

  EXPECT_STREQ("std::sqrt(43.3E-5)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSquareRoot03)
{  
  ss << R"zzz(
√43.3E-5+4
)zzz";

  EXPECT_STREQ("std::sqrt(43.3E-5) + 4", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSquareRoot04)
{  
  ss << R"zzz(
 _________
√43.3E-5+4
)zzz";

  EXPECT_STREQ("std::sqrt(43.3E-5 + 4)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSquareRoot05)
{  
  ss << R"zzz(
 _________
√43 + 42.1
)zzz";

  EXPECT_STREQ("std::sqrt(43 + 42.1)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSquareRoot06)
{  
  ss << R"zzz(
  ___
 ╱x-z  
√4   
)zzz";

  EXPECT_STREQ("std::sqrt(std::pow(4, x - z))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSquareRoot07)
{  
  ss << R"zzz(
  ___
 ╱x-z  
√4    + 3
)zzz";

  EXPECT_STREQ("std::sqrt(std::pow(4, x - z)) + 3", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSquareRoot08)
{  
  ss << R"zzz(
  ___________
 ╱  x-z  
√ (4    ) + 3
)zzz";

  EXPECT_STREQ("std::sqrt((std::pow(4, x - z)) + 3)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSquareRoot09)
{  
  ss << R"zzz(
  ___________
 ╱x-z  
√4   + 3
)zzz";

  EXPECT_STREQ("std::sqrt(std::pow(4, x - z) + 3)", parser.doMath(ss).c_str());
}


TEST_F(TestMathParser, TestSquareRoot10)
{  
  ss << R"zzz(
   __________
  ╱⎛  1  ⎞
 ╱ ⎜ ――― ⎟*2
√  ⎝ x+z ⎠
)zzz";

  EXPECT_STREQ("std::sqrt(((1 / (x + z))) * 2)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSquareRoot11)
{  
  ss << R"zzz(
√(43.3E-5+4)
)zzz";

  EXPECT_STREQ("std::sqrt((43.3E-5 + 4))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSquareRoot12)
{  
  ss << R"zzz(
 ⎛  1  ⎞
√⎜ ――― ⎟
 ⎝ x+z ⎠
)zzz";

  EXPECT_STREQ("std::sqrt(((1 / (x + z))))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestDivide01)
{  
  ss << R"zzz(
 1
 ―
 x
)zzz";

  EXPECT_STREQ("(1 / x)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestDivide02)
{  
  ss << R"zzz(
 1+x
 ―――
 x+1
)zzz";

  EXPECT_STREQ("((1 + x) / (x + 1))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestDivide03)
{  
  ss << R"zzz(
  1+x
 ―――――――――
    x-1
)zzz";

  EXPECT_STREQ("((1 + x) / (x - 1))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestDivide04)
{  
  ss << R"zzz(
⎛  1  ⎞
⎜―――――⎟
⎝ x+z ⎠
)zzz";

  EXPECT_STREQ("((1 / (x + z)))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestDivide05)
{  
  ss << R"zzz(
    1+x
    ――――
     z
 ――――――――――――
      x-1
)zzz";

  EXPECT_STREQ("((((1 + x) / z)) / (x - 1))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestDivide06)
{  
  ss << R"zzz(
      x-1
 ――――――――――――
    1+x
    ――――
     z
)zzz";

  EXPECT_STREQ("((x - 1) / (((1 + x) / z)))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestDivide07)
{  
  ss << R"zzz(
      x-1
 ――――――――――――
    1+x
    ――――
     z
 ――――――――――――
   (c * x)
)zzz";

  EXPECT_STREQ("((((x - 1) / (((1 + x) / z)))) / ((c * x)))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestDivide08)
{  
  ss << R"zzz(
      x-1
 ――――――――――――
    1+x
    ――――
     z
  ―――――――― 
   (x * x)
)zzz";

  EXPECT_STREQ("((x - 1) / (((((1 + x) / z)) / ((x * x)))))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestParens01)
{  
  ss << R"zzz(
⎛  1  ⎞
⎜ ――― ⎟
⎝ x+z ⎠
)zzz";

  EXPECT_STREQ("((1 / (x + z)))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestParens02)
{  
  ss << R"zzz(
1 + ⎛  1  ⎞
    ⎜ ――― ⎟
    ⎝ x+z ⎠
)zzz";

  EXPECT_STREQ("1 + ((1 / (x + z)))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestParens03)
{  
  ss << R"zzz(
    ⎛  1  ⎞
    ⎜ ――― ⎟
1 + ⎝ x+z ⎠
)zzz";

  EXPECT_STREQ("1 + ((1 / (x + z)))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestParens04)
{  
  ss << R"zzz(
    ⎛  1  ⎞  + 1
    ⎜ ――― ⎟
    ⎝ x+z ⎠
)zzz";

  EXPECT_STREQ("((1 / (x + z))) + 1", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestParens05)
{  
  ss << R"zzz(
    ⎛  1  ⎞
    ⎜ ――― ⎟
    ⎝ x+z ⎠+1
)zzz";

  EXPECT_STREQ("((1 / (x + z))) + 1", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestParens06)
{  
  ss << R"zzz(
1+  ⎛  1  ⎞
    ⎜ ――― ⎟
    ⎝ x+z ⎠+1
)zzz";

  EXPECT_STREQ("1 + ((1 / (x + z))) + 1", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestParens07)
{  
  ss << R"zzz(
         x
  ⎛  1  ⎞
  ⎜ ――― ⎟
  ⎝ x+z ⎠
)zzz";

  EXPECT_STREQ("std::pow(((1 / (x + z))), x)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestParens08)
{  
  ss << R"zzz(
         x + 1
  ⎛  1  ⎞
  ⎜ ――― ⎟
  ⎝ x+z ⎠
)zzz";

  EXPECT_STREQ("std::pow(((1 / (x + z))), x + 1)", parser.doMath(ss).c_str());
}


TEST_F(TestMathParser, TestParens09)
{  
  ss << R"zzz(
         
  ⎛  1  ⎞
  ⎜ ――― ⎟
  ⎝ x+z ⎠
 x
)zzz";

  EXPECT_STREQ("std::pow(x, ((1 / (x + z))))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestParens10)
{  
  ss << R"zzz(
         
       ⎛    ⎞
       ⎜  2 ⎟
       ⎝ x  ⎠
⎛  z  ⎞
⎜ ――― ⎟
⎝ x+z ⎠
)zzz";

  EXPECT_STREQ("std::pow(((z / (x + z))), (std::pow(x, 2)))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestParens11)
{  
  ss << R"zzz(
             3
       ⎛    ⎞
       ⎜  2 ⎟
       ⎝ x  ⎠
⎛  z  ⎞
⎜ ――― ⎟
⎝ x+z ⎠
)zzz";

  EXPECT_STREQ("std::pow(((z / (x + z))), std::pow((std::pow(x, 2)), 3))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestParens12)
{  
  ss << R"zzz(
                2
               e
         ⎛    ⎞
         ⎜  2 ⎟
         ⎝ x  ⎠
  ⎛  z  ⎞
  ⎜ ――― ⎟
  ⎝ x+z ⎠
 3
)zzz";

  EXPECT_STREQ("std::pow(3, std::pow(((z / (x + z))), std::pow((std::pow(x, 2)), std::exp(2))))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestBrackets01)
{  
  ss << R"zzz(
    ⎛  z  ⎞
    ⎜ x   ⎟
 x[ ⎜ ――― ⎟ ] ← 3
    ⎜  z  ⎟
    ⎜ ――― ⎟
    ⎝ x+z ⎠
)zzz";

  EXPECT_STREQ("x[(((((std::pow(x, z)) / z)) / (x + z)))] = 3", errorParser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestBrackets02)
{  
  ss << R"zzz(
 x[ 3 * c ] ← 3
)zzz";

  EXPECT_STREQ("x[3 * c] = 3", errorParser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestBrackets03)
{  
  ss << R"zzz(
    3    2
 x[ ― * c ] ← 3
    4
)zzz";

  EXPECT_STREQ("x[(3 / 4) * std::pow(c, 2)] = 3", errorParser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestBrackets04)
{  
  ss << R"zzz(
   ⎛  z  ⎞
   ⎜ x   ⎟      2
 x[⎜ ――― ⎟ * var ] ← 3
   ⎜  z  ⎟
   ⎜ ――― ⎟
   ⎝ x+z ⎠
)zzz";

  EXPECT_STREQ("x[(((((std::pow(x, z)) / z)) / (x + z))) * std::pow(var, 2)] = 3", errorParser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestBrackets05)
{  
  ss << R"zzz(
   ⎛  z  ⎞
   ⎜ x   ⎟       2
 x[⎜ ――― ⎟ * x[c] ] ← 3
   ⎜  z  ⎟
   ⎜ ――― ⎟
   ⎝ x+z ⎠
)zzz";

  EXPECT_STREQ("x[(((((std::pow(x, z)) / z)) / (x + z))) * std::pow(x[c], 2)] = 3", errorParser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestFunctionCall01)
{  
  ss << R"zzz(
sin(z)
)zzz";

  EXPECT_STREQ("std::sin(z)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestFunctionCall02)
{  
  ss << R"zzz(
sin z 
)zzz";

  EXPECT_STREQ("std::sin(z)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestFunctionCall03)
{  
  ss << R"zzz(
cos 2z 
)zzz";

  EXPECT_STREQ("std::cos((2*z))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestFunctionCall04)
{  
  ss << R"zzz(
cos -2z 
)zzz";

  EXPECT_STREQ("std::cos(-(2*z))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestFunctionCall05)
{  
  ss << R"zzz(
  4 + -cos -2z * sin sin var
)zzz";

  EXPECT_STREQ("4 + -std::cos(-(2*z)) * std::sin(std::sin(var))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestFunctionCall06)
{  
  ss << R"zzz(
                    2
  -2cos 2πvarxsinvar
)zzz";

  EXPECT_STREQ("(-2*std::cos(((((2*n::pi)*var)*x)*std::pow(var, 2))))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestFunctionCall07)
{  
  ss << R"zzz(
                         2
  -2⋅cos(2πe⋅var⋅x⋅sin(var ))
)zzz";

  EXPECT_STREQ("-2 * std::cos((((2*n::pi)*n::e) * var * x * std::sin(2var)))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestMonomial01)
{  
  ss << R"zzz(
2z
)zzz";

  EXPECT_STREQ("(2*z)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestMonomial02)
{  
  ss << R"zzz(
xz
)zzz";

  EXPECT_STREQ("(x*z)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestMonomial03)
{  
  ss << R"zzz(
2xz
)zzz";

  EXPECT_STREQ("((2*x)*z)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestMonomial04)
{  
  ss << R"zzz(
-2.3xz
)zzz";

  EXPECT_STREQ("-((2.3*x)*z)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestMonomial05)
{  
  ss << R"zzz(
+2.3varxz
)zzz";

  EXPECT_STREQ("+(((2.3*var)*x)*z)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestMonomial06)
{  
  ss << R"zzz(
x³⁴z
)zzz";

  EXPECT_STREQ("(std::pow(x, 34)*z)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestMonomial07)
{  
  ss << R"zzz(
 _
√4x²
)zzz";

  EXPECT_STREQ("(std::sqrt(4)*std::pow(x, 2))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestMonomial08)
{  
  ss << R"zzz(
2πx
)zzz";

  EXPECT_STREQ("((2*n::pi)*x)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestMonomial09)
{  
  ss << R"zzz(
x2π
)zzz";
  
  EXPECT_CALL(parser, error(_, "Number without operator"));
  parser.doMath(ss);
}

TEST_F(TestMathParser, TestMonomial10)
{  
  ss << R"zzz(
2πvarx
)zzz";

  EXPECT_STREQ("(((2*n::pi)*var)*x)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestMonomial11)
{  
  ss << R"zzz(
2x[1]
)zzz";

  EXPECT_STREQ("(2*x[1])", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestMonomial12)
{  
  ss << R"zzz(
2πx[1]varH₂Ox₂
)zzz";

  EXPECT_STREQ("(((((2*n::pi)*x[1])*var)*H2O)*x_2)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestFormula01)
{  
  ss << R"zzz(
       _________
      ╱ b² - 4ac 
-b + ╱  ―――――――― 
    √      2a    
)zzz";

  EXPECT_STREQ("-b + std::sqrt(((std::pow(b, 2) - ((4*a)*c)) / ((2*a))))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestFormula02)
{  
  ss << R"zzz(
      _________
     √ b² - 4ac 
-b + ――――――――――
         2a    
)zzz";

  EXPECT_STREQ("-b + ((std::sqrt(std::pow(b, 2) - ((4*a)*c))) / ((2*a)))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestFormula03)
{  
  ss << R"zzz(
      _________
-b + √ b² - 4ac 
―――――――――――――――
        2a    
)zzz";

  EXPECT_STREQ("((-b + std::sqrt(std::pow(b, 2) - ((4*a)*c))) / ((2*a)))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestFormula04)
{  
  ss << R"zzz(
          ⎛  _         ⎞
          ⎜ √4x²-√(3/4)⎟ 
          ⎜ ―――――――――――⎟
          ⎜        ___ ⎟
       x ∗⎜       √x-2 ⎟
          ⎜ 1.3 + ―――― ⎟
          ⎜        x   ⎟ 
          ⎜ ―――――――――――⎟
          ⎝    42E5÷a  ⎠
)zzz";

  EXPECT_STREQ("x * ((((((std::sqrt(4)*std::pow(x, 2)) - std::sqrt(((3 / 4)))) / "
               "(1.3 + ((std::sqrt(x - 2)) / x)))) / "
               "((42E5 / a))))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestFormula05)
{  
  ss << R"zzz( 
                                                      (x/2.4)
          ⎛              x - 3.3⎞              ⎛  1  ⎞
          ⎜⎛  _         ⎞       ⎟              ⎜ ――― ⎟
          ⎜⎜ √4x²-√(3/4)⎟       ⎟              ⎝ x+z ⎠                      4
          ⎜⎜ ―――――――――――⎟      z⎟    ⎛   ____ ⎞                          3⁵⁵
          ⎜⎜        ___ ⎟    32 ⎟    ⎜  ╱ 3   ⎟         -42.5E+3 + x⁽ᶻ⁻⁵⁾
   z← x * ⎜⎜       √x-2 ⎟ + e   ⎟ ⋅ √⎜ √ x -4 ⎟  × -0.E4
          ⎜⎜ 1.3 + ―――― ⎟       ⎟    ⎝        ⎠
          ⎜⎜        x   ⎟       ⎟
          ⎜⎜ ―――――――――――⎟       ⎟
          ⎝⎝    42E5÷a  ⎠       ⎠
)zzz";

  EXPECT_STREQ("z = x * (std::pow(((((((std::sqrt(4)*std::pow(x, 2)) - std::sqrt(((3 / 4)))) / (1.3 + ((std::sqrt(x - 2)) / x)))) / ((42E5 / a)))), x - 3.3) + std::exp(std::pow(32, z))) * "
               "std::pow(std::sqrt((std::sqrt(std::pow(x, 3) - 4))), std::pow(((1 / (x + z))), ((x / 2.4)))) * "
               "-std::pow(0.E4, -42.5E+3 + std::pow(x, std::pow((z - 5), std::pow(3, std::pow(55, 4)))))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestAssignment01)
{  
  ss << R"zzz( 
                   _________
   var← z ←  -b + √ b² - 4ac ;
             ―――――――――――――――
                 2a    
        
)zzz";

  EXPECT_STREQ("var = z = ((-b + std::sqrt(std::pow(b, 2) - ((4*a)*c))) / ((2*a)));", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestAssignment02)
{  
  ss << R"zzz( 
                _________
      z←  -b + √ b² - 4ac ;
          ―――――――――――――――
                2a    
        
)zzz";

  EXPECT_STREQ("z = ((-b + std::sqrt(std::pow(b, 2) - ((4*a)*c))) / ((2*a)));", parser.doMath(ss).c_str());
}
