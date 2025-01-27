#include "TestParser.h"

namespace nate
{
extern const char* programExp =
R"__(#undef NOMINMAX
#define NOMINMAX
#include <windows.h>
#include <cmath>
#include <stdfloat>
#include "C:\Users\ruud\source\repos\Nate\Nate\core\Core.h"
#include "C:\Users\Ruud\source\repos\Nate\NateTest\created\File-Input.h"
std::shared_ptr<std::ostream> output_ = {};
std::shared_ptr<std::ostream> error_ = {};
std::shared_ptr<std::istream> input_ = {};
const std::float64_t pi_ = 3.14159265358979323;
const std::float64_t tau_ = 6.283185307179586;
const std::float64_t e_ = 2.718281828459045;
#include "C:\Users\ruud\source\repos\Nate\Nate\core\Complex.h"
#include <complex>
const std::float64_t i_ = 1;
#include <vector>
#include "C:\Users\ruud\source\repos\Nate\Nate\core\Rational.h"
#include <sstream>
#include <fstream>
#include "C:\Users\ruud\source\repos\Nate\Nate\core\main.h"
int main(int argc, char** argv)
{
  initMain(argc, argv);
#define NATE_PROGRAM_START
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
import Nate
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
