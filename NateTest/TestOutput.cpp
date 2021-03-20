#include "TestParser.h"

static std::filesystem::path in("Output_tests.txt");


TEST_PIECE(Output, Empty)
TEST_PIECE(Output, String)
TEST_PIECE(Output, Multiple)
TEST_PIECE(Output, CommaAtEnd)
TEST_PIECE(Output, ConcatAtEnd)