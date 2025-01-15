#include "TestParser.h"

namespace nate
{
static std::filesystem::path in("Expr_tests.txt");

TEST_PIECE(Expr, NormalOperators)
TEST_PIECE(Expr, Aliases)
TEST_PIECE(Expr, Parentheses)
TEST_PIECE(Expr, StringOperators)
TEST_PIECE(Expr, BoolOperators)
TEST_PIECE(Expr, Increments)
TEST_PIECE(Expr, Decrements)
TEST_PIECE(Expr, IncreaseDecrease)
TEST_PIECE(Expr, IfElse)
TEST_PIECE(Expr, Math)
}
