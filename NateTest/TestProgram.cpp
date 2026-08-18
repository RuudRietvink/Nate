#include "TestParser.h"

namespace nate
{
extern const char* programExp =
R"__(#include <cmath>
#include "Core.h"
#include "File-Input.h"
std::shared_ptr<std::ostream> output_ = {};
std::shared_ptr<std::ostream> error_ = {};
std::shared_ptr<std::istream> input_ = {};
const float64_t pi_ = 3.14159265358979323;
const float64_t tau_ = 6.283185307179586;
const float64_t e_ = 2.718281828459045;
#include "Complex.h"
#include <complex>
const float64_t i_ = 1;
#include <vector>
#include "Rational.h"
#include <sstream>
#include <fstream>
#include "main.h"
int main(int argc, char** argv)
{
  initMain(argc, argv);
#define NATE_PROGRAM_START
  string_t text_ = {};
  int32_t int32_ = {};
  float32_t float_ = {};
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
