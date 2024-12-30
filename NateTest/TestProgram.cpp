#include "TestParser.h"

namespace nate
{
extern const char* programExp =
R"__(#line 1 "test.nd"
std::shared_ptr<std::ostream> output = {};
#line 1
std::shared_ptr<std::ostream> error = {};
#line 1
std::shared_ptr<std::istream> input = {};
#line 27 ""
#define NOMINMAX
#include <windows.h>
#include <cmath>
#line 32
const double pi = 3.14159265358979323;
#line 34
const double tau = 6.283185307179586;
#line 36
const double e = 2.718281828459045;
#undef NOMINMAX
#define NOMINMAX
#include <windows.h>
#line 3
int main(int argc, char** argv)
{
  output = std::shared_ptr<std::ostream>(&std::cout, [](void*) {});
  error = std::shared_ptr<std::ostream>(&std::cerr, [](void*) {});
  input = std::shared_ptr<std::istream>(&std::cin, [](void*) {});
  SetConsoleOutputCP(65001);
#define NATE_PROGRAM_START
#line 4
  string_t text = {};
  int32_t int32 = {};
  float float = {};
  *output << "Hello" << std::endl;
}
)__";

TEST_F(TestParser, Program)
{
  std::string in =
R"__(
import Math
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
