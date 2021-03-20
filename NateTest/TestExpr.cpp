#include "TestParser.h"

static std::filesystem::path in("Expr_tests.txt");

TEST_PIECE(Expr, NormalOperators)
TEST_PIECE(Expr, Aliases)
TEST_PIECE(Expr, Parentheses)
TEST_PIECE(Expr, StringOperators)