#include "MathParser.h"
#include <sstream>
#include <string>
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using ::testing::_;

class MockMathParser : public MathParser
{
public:
      
  MOCK_CONST_METHOD2(error, void(const Position&, const std::string&));
};

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
    parser.addVariable("a");
    parser.addVariable("b");
    parser.addVariable("c");
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
  MockMathParser parser;
};

TEST_F(TestMathParser, TestAddVariable)
{
  EXPECT_CALL(parser, error(_, "Reserved name: i"));
  EXPECT_CALL(parser, error(_, "Reserved name: j"));
  EXPECT_CALL(parser, error(_, "Reserved name: e"));

  parser.addVariable("i");
  parser.addVariable("j");
  parser.addVariable("e");
  parser.addVariable("E");
  parser.addVariable("ij");
  parser.addVariable("I");
}

TEST_F(TestMathParser, TestSimple1)
{  
  ss << R"zzz(
 - x+2*-var/+2.3
)zzz";

  EXPECT_STREQ("-x + ((2 * -var) / (+2.3))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSimple2)
{  
  ss << R"zzz(
 - (x- z)
)zzz";

  EXPECT_STREQ("-(x - z)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSimple3)
{  
  ss << R"zzz(
(43.3E-5+4)
)zzz";

  EXPECT_STREQ("(43.3E-5 + 4)", parser.doMath(ss).c_str());
}

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
 -var
)zzz";

  EXPECT_STREQ("-pow(var, 2)", parser.doMath(ss).c_str());
  
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
  var - 3
 x
)zzz";

  EXPECT_STREQ("pow(x, var - 3)", parser.doMath(ss).c_str());
  
}

TEST_F(TestMathParser, TestPower5)
{  
  ss << R"zzz(
  -(x - 3) * 4
 x
)zzz";

  EXPECT_STREQ("pow(x, -(x - 3) * 4)", parser.doMath(ss).c_str());

}

TEST_F(TestMathParser, TestPower6)
{  
  ss << R"zzz(
        -(x - 3) * 4
 (x + 3)
)zzz";

  EXPECT_STREQ("pow((x + 3), -(x - 3) * 4)", parser.doMath(ss).c_str());
  
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

  EXPECT_STREQ("pow(x, 2.5) + 3", parser.doMath(ss).c_str());
  }

TEST_F(TestMathParser, TestExponential1)
{  
  ss << R"zzz(
  x
 e
)zzz";

  EXPECT_STREQ("exp(x)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestExponential2)
{  
  ss << R"zzz(
  x
 e
𝑒
)zzz";

  EXPECT_STREQ("exp(exp(x))", parser.doMath(ss).c_str());
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

  EXPECT_STREQ("pow(x, z - 5)", parser.doMath(ss).c_str());
  
}

TEST_F(TestMathParser, TestPowerSuper3)
{
  ss << R"zzz(
x⁽ᶻ ⁻ ⁵⁾
)zzz";

  EXPECT_STREQ("pow(x, (z - 5))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestPowerSuper4)
{
  ss << R"zzz(
  x
⁵⁵
)zzz";

  EXPECT_STREQ("pow(55, x)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestPowerSuper5)
{
  ss << R"zzz(
           e⁵
          e
     x - 2
  z⁵⁵
xz
)zzz";

  EXPECT_STREQ("(x*pow(z, pow(z, pow(55, x - pow(2, exp(pow(e, 5)))))))", parser.doMath(ss).c_str());
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

  EXPECT_STREQ("sqrt(43.3E-5) + 4", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSquareRoot4)
{  
  ss << R"zzz(
 _________
√43.3E-5+4
)zzz";

  EXPECT_STREQ("sqrt(43.3E-5 + 4)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSquareRoot5)
{  
  ss << R"zzz(
 _________
√43 + 42.1
)zzz";

  EXPECT_STREQ("sqrt(43 + 42.1)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSquareRoot6)
{  
  ss << R"zzz(
  ___
 ╱x-z  
√4   
)zzz";

  EXPECT_STREQ("sqrt(pow(4, x - z))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSquareRoot7)
{  
  ss << R"zzz(
  ___
 ╱x-z  
√4    + 3
)zzz";

  EXPECT_STREQ("sqrt(pow(4, x - z)) + 3", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSquareRoot8)
{  
  ss << R"zzz(
  ___________
 ╱  x-z  
√ (4    ) + 3
)zzz";

  EXPECT_STREQ("sqrt((pow(4, x - z)) + 3)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSquareRoot9)
{  
  ss << R"zzz(
  ___________
 ╱x-z  
√4   + 3
)zzz";

  EXPECT_STREQ("sqrt(pow(4, x - z) + 3)", parser.doMath(ss).c_str());
}


TEST_F(TestMathParser, TestSquareRoot10)
{  
  ss << R"zzz(
   __________
  ╱⎛  1  ⎞
 ╱ ⎜ ――― ⎟*2
√  ⎝ x+z ⎠
)zzz";

  EXPECT_STREQ("sqrt(((1 / (x + z))) * 2)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSquareRoot11)
{  
  ss << R"zzz(
√(43.3E-5+4)
)zzz";

  EXPECT_STREQ("sqrt((43.3E-5 + 4))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestSquareRoot12)
{  
  ss << R"zzz(
 ⎛  1  ⎞
√⎜ ――― ⎟
 ⎝ x+z ⎠
)zzz";

  EXPECT_STREQ("sqrt(((1 / (x + z))))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestDivide)
{  
  ss << R"zzz(
 1
 ―
 x
)zzz";

  EXPECT_STREQ("(1 / x)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestParens1)
{  
  ss << R"zzz(
⎛  1  ⎞
⎜ ――― ⎟
⎝ x+z ⎠
)zzz";

  EXPECT_STREQ("((1 / (x + z)))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestParens2)
{  
  ss << R"zzz(
1 + ⎛  1  ⎞
    ⎜ ――― ⎟
    ⎝ x+z ⎠
)zzz";

  EXPECT_STREQ("1 + ((1 / (x + z)))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestParens3)
{  
  ss << R"zzz(
    ⎛  1  ⎞
    ⎜ ――― ⎟
1 + ⎝ x+z ⎠
)zzz";

  EXPECT_STREQ("1 + ((1 / (x + z)))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestParens4)
{  
  ss << R"zzz(
    ⎛  1  ⎞  + 1
    ⎜ ――― ⎟
    ⎝ x+z ⎠
)zzz";

  EXPECT_STREQ("((1 / (x + z))) + 1", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestParens5)
{  
  ss << R"zzz(
    ⎛  1  ⎞
    ⎜ ――― ⎟
    ⎝ x+z ⎠+1
)zzz";

  EXPECT_STREQ("((1 / (x + z))) + 1", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestParens6)
{  
  ss << R"zzz(
1+  ⎛  1  ⎞
    ⎜ ――― ⎟
    ⎝ x+z ⎠+1
)zzz";

  EXPECT_STREQ("1 + ((1 / (x + z))) + 1", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestParens7)
{  
  ss << R"zzz(
         x
  ⎛  1  ⎞
  ⎜ ――― ⎟
  ⎝ x+z ⎠
)zzz";

  EXPECT_STREQ("pow(((1 / (x + z))), x)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestParens8)
{  
  ss << R"zzz(
         x + 1
  ⎛  1  ⎞
  ⎜ ――― ⎟
  ⎝ x+z ⎠
)zzz";

  EXPECT_STREQ("pow(((1 / (x + z))), x + 1)", parser.doMath(ss).c_str());
}


TEST_F(TestMathParser, TestParens9)
{  
  ss << R"zzz(
         
  ⎛  1  ⎞
  ⎜ ――― ⎟
  ⎝ x+z ⎠
 x
)zzz";

  EXPECT_STREQ("pow(x, ((1 / (x + z))))", parser.doMath(ss).c_str());
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

  EXPECT_STREQ("pow(((z / (x + z))), (pow(x, 2)))", parser.doMath(ss).c_str());
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

  EXPECT_STREQ("pow(((z / (x + z))), pow((pow(x, 2)), 3))", parser.doMath(ss).c_str());
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

  EXPECT_STREQ("pow(3, pow(((z / (x + z))), pow((pow(x, 2)), exp(2))))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestMonomial)
{  
  ss << R"zzz(
2z
)zzz";

  EXPECT_STREQ("(2*z)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestMonomial2)
{  
  ss << R"zzz(
xz
)zzz";

  EXPECT_STREQ("(x*z)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestMonomial3)
{  
  ss << R"zzz(
2xz
)zzz";

  EXPECT_STREQ("((2*x)*z)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestMonomial4)
{  
  ss << R"zzz(
-2.3xz
)zzz";

  EXPECT_STREQ("-((2.3*x)*z)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestMonomial5)
{  
  ss << R"zzz(
+2.3varxz
)zzz";

  EXPECT_STREQ("+(((2.3*var)*x)*z)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestMonomial6)
{  
  ss << R"zzz(
x³⁴z
)zzz";

  EXPECT_STREQ("(pow(x, 34)*z)", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestMonomial7)
{  
  ss << R"zzz(
 _
√4x²
)zzz";

  EXPECT_STREQ("(sqrt(4)*pow(x, 2))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestFormula)
{  
  ss << R"zzz(
       _________
      ╱ b² - 4ac 
-b + ╱  ―――――――― 
    √      2a    
)zzz";

  EXPECT_STREQ("-b + sqrt(((pow(b, 2) - ((4*a)*c)) / ((2*a))))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestFormula2)
{  
  ss << R"zzz(
      _________
     √ b² - 4ac 
-b + ――――――――――
         2a    
)zzz";

  EXPECT_STREQ("-b + ((sqrt(pow(b, 2) - ((4*a)*c))) / ((2*a)))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestFormula3)
{  
  ss << R"zzz(
      _________
-b + √ b² - 4ac 
―――――――――――――――
        2a    
)zzz";

  EXPECT_STREQ("((-b + sqrt(pow(b, 2) - ((4*a)*c))) / ((2*a)))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestFormula4)
{  
  ss << R"zzz(
          ⎛  _         ⎞
          ⎜ √4x²-√(3/4)⎟ 
          ⎜ ―――――――――――⎟
          ⎜        ___ ⎟
       x ∗⎜       √x-2 ⎟
          ⎜ 1.3 + ―――― ⎟
          ⎜        x   ⎟ 
          ⎜            ⎟
          ⎜ ―――――――――――⎟
          ⎝    42E5÷a  ⎠
)zzz";

  EXPECT_STREQ("x * ((((((sqrt(4)*pow(x, 2)) - sqrt(((3 / 4)))) / "
               "(1.3 + ((sqrt(x - 2)) / x)))) / "
               "((42E5 / a))))", parser.doMath(ss).c_str());
}

TEST_F(TestMathParser, TestFormula5)
{  
  ss << R"zzz( 
      z:=                                             (x/2.4)
          ⎛              x - 3.3⎞              ⎛  1  ⎞
          ⎜⎛  _         ⎞       ⎟              ⎜ ――― ⎟
          ⎜⎜ √4x²-√(3/4)⎟       ⎟              ⎝ x+z ⎠                      4
          ⎜⎜ ―――――――――――⎟      z⎟    ⎛   ____ ⎞                          3⁵⁵
          ⎜⎜        ___ ⎟    32 ⎟    ⎜  ╱ 3   ⎟         -42.5E+3 + x⁽ᶻ⁻⁵⁾
       x ∗⎜⎜       √x-2 ⎟ + e   ⎟ ⋅ √⎜ √ x -4 ⎟  × -0.E4
          ⎜⎜ 1.3 + ―――― ⎟       ⎟    ⎝        ⎠
          ⎜⎜        x   ⎟       ⎟
          ⎜⎜            ⎟       ⎟
          ⎜⎜ ―――――――――――⎟       ⎟
          ⎝⎝    42E5÷a  ⎠       ⎠
)zzz";

  EXPECT_STREQ("z:=x * (pow(((((((sqrt(4)*pow(x, 2)) - sqrt(((3 / 4)))) / (1.3 + ((sqrt(x - 2)) / x)))) / ((42E5 / a)))), x - 3.3) + exp(pow(32, z))) * "
               "pow(sqrt((sqrt(pow(x, 3) - 4))), pow(((1 / (x + z))), ((x / 2.4)))) * "
               "-pow(0.E4, -42.5E+3 + pow(x, pow((z - 5), pow(3, pow(55, 4)))))", parser.doMath(ss).c_str());
}
