#include "TestParser.h"

namespace nate
{
extern const char* programExp =
R"__(#undef NOMINMAX
#define NOMINMAX
#include <windows.h>
#include <cmath>
#include <stdfloat>
#line 1 "test.nd"
std::shared_ptr<std::ostream> output_ = {};
#line 1
std::shared_ptr<std::ostream> error_ = {};
#line 1
std::shared_ptr<std::istream> input_ = {};
#line 27 ""
const std::float64_t pi_ = 3.14159265358979323;
const std::float64_t tau_ = 6.283185307179586;
const std::float64_t e_ = 2.718281828459045;
#line 6
#include <vector>
#line 5
#include "C:\Users\ruud\source\repos\Nate\Nate\core\main.h"
int main(int argc, char** argv)
{
  initMain(argc, argv);
#define NATE_PROGRAM_START
#line 6
  string_t text_ = {};
  int32_t int32_ = {};
  std::float32_t float_ = {};
  *output_ << "Hello\n";
}
)__";

TEST_F(TestParser, Program)
{
  std::string in =
R"__(
import Math
import List
import Text
program:
  var text is text
  var int32 is int-32
  var float is float
  output "Hello"
)__";

  EXPECT_EQ(0, parseProgram(in));
  compareWhole(programExp, code());
}

static std::filesystem::path in("Program_tests.txt");

TEST_PIECE(Program, Arguments)
TEST_PIECE(Program, Name)
}
