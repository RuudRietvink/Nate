#include "TestParser.h"

TEST_F(TestParser, Program)
{
  std::string in =
R"__(program:
  var text is text
  var int32 is int-32
  var float is float
  output "Hello"
)__";

  const char* exp =
R"__(Code
LocalVar output std::shared_ptr<std::ostream>={}
LocalVar error std::shared_ptr<std::ostream>={}
LocalVar input std::shared_ptr<std::istream>={}
Program
  LocalVar text string_t={}
  LocalVar int32 int32_t={}
  LocalVar float float={}
  StdOutput
    Expr "Hello"
    End
)__";

  EXPECT_EQ(0, parseProgram(in));
  compareWhole(exp, code());
}
