#include "TestParser.h"

static std::filesystem::path in("Expr_tests.txt");

TEST_F(TestParser, NormalOperators)
{
  testCodePiece("NormalOperators", in);
}
