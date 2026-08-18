#include "gtest/gtest.h"

#ifdef _WIN32
#include <Windows.h>
#endif

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);

#ifdef _WIN32
  // set code page to utf8
  SetConsoleOutputCP(CP_UTF8);                        
#endif

  // Enable buffering to prevent VS from chopping up UTF-8 byte sequences
  setvbuf(stdout, nullptr, _IOFBF, 1000);
  setvbuf(stderr, nullptr, _IOFBF, 1000);

  return RUN_ALL_TESTS();
}