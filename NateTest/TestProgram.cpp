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
#undef NOMINMAX
#define NOMINMAX
#include <windows.h>
#line 1 ""
int main(int argc, char** argv)
{
  output = std::shared_ptr<std::ostream>(&std::cout, [](void*) {});
  error = std::shared_ptr<std::ostream>(&std::cerr, [](void*) {});
  input = std::shared_ptr<std::istream>(&std::cin, [](void*) {});
  SetConsoleOutputCP(65001);
#line 2
  string_t text = {};
  int32_t int32 = {};
  float float = {};
  *output << "Hello" << std::endl;
}
)__";

TEST_F(TestParser, Program)
{
  std::string in =
R"__(program:
  var text is text
  var int32 is int-32
  var float is float
  output "Hello"
)__";

  EXPECT_EQ(0, parseProgram(in));
  compareWhole(programExp, code());
}

}
