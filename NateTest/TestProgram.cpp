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
#line 4
int main(int argc, char** argv)
{
  output_ = std::shared_ptr<std::ostream>(&std::cout, [](void*) {});
  error_ = std::shared_ptr<std::ostream>(&std::cerr, [](void*) {});
  input_ = std::shared_ptr<std::istream>(&std::cin, [](void*) {});
  SetConsoleOutputCP(65001);
#define NATE_PROGRAM_START
#line 5
  string_t text_ = {};
  int32_t int32_ = {};
  std::float32_t float_ = {};
  *output_ << "Hello" << std::endl;
}
)__";

TEST_F(TestParser, Program)
{
  std::string in =
R"__(
import Math
import List
program:
  var text is text
  var int32 is int-32
  var float is float
  output "Hello"
)__";

  EXPECT_EQ(0, parseProgram(in));
  compareWhole(programExp, code());
}

}
