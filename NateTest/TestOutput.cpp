#include "TestParser.h"

static std::filesystem::path in("Output_tests.txt");

TEST_F(TestParser, Output_Empty)
{
  testCodePiece("Empty", in);
}

TEST_F(TestParser, Output_String)
{
  testCodePiece("String", in);
}

TEST_F(TestParser, Output_Multiple)
{
  testCodePiece("Multiple", in);
}

TEST_F(TestParser, Output_CommaAtEnd)
{
  testCodePiece("CommaAtEnd", in);
}

TEST_F(TestParser, Output_ConcatAtEnd)
{
  testCodePiece("ConcatAtEnd", in);
}