#include "TestParser.h"

namespace nate
{
static std::filesystem::path in("Literals_tests.txt");

TEST_PIECE(Literals, Integers)
TEST_PIECE(Literals, Int8)
TEST_PIECE(Literals, Int16)
TEST_PIECE(Literals, Int32)
TEST_PIECE(Literals, Int64)
TEST_PIECE(Literals, Binary)
TEST_PIECE(Literals, Octal)
TEST_PIECE(Literals, Hex)
}
