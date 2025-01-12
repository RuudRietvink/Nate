#pragma once

#include <vector>
#include <algorithm>
#include <iterator>
#include <string>
#include <memory>

#undef NOMINMAX
#define NOMINMAX
#include <windows.h>

extern std::vector<std::string> programArguments__;
extern std::string programName__;

void initMain__(int argc, char** argv)
{
    output_ = std::shared_ptr<std::ostream>(&std::cout, [](void*) {});
    error_ = std::shared_ptr<std::ostream>(&std::cerr, [](void*) {});
    input_ = std::shared_ptr<std::istream>(&std::cin, [](void*) {});
    SetConsoleOutputCP(65001);
    //std::locale::global(std::locale(\"en_US.UTF8\"));

    programName__ = argv[0];
    for (int ind = 1; ind < argc; ++ind)
    {
        programArguments__.push_back(argv[ind]);
    }
}