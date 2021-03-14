#include "TestParser.h"

std::filesystem::path in("Output_tests.txt");

TEST_F(TestParser, Output_Empty)
{
  testCodePiece("Empty", in);
}

TEST_F(TestParser, Output_String)
{
  testCodePiece("String", in);
}