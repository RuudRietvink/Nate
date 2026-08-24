#pragma once

#include <vector>
#include <algorithm>
#include <iterator>
#include <string>
#include <memory>
#include "Core.h"

inline std::vector<std::string> programArguments__;
inline std::string programName__;

void initMain__(int argc, char** argv)
{
    output_ = std::shared_ptr<std::ostream>(&std::cout, [](void*) {});
    error_ = std::shared_ptr<std::ostream>(&std::cerr, [](void*) {});
    input_ = std::shared_ptr<std::istream>(&std::cin, [](void*) {});
    Core::enableUtf8Console();
    //std::locale::global(std::locale(\"en_US.UTF8\"));

    programName__ = argv[0];
    for (int ind = 1; ind < argc; ++ind)
    {
        programArguments__.push_back(argv[ind]);
    }
}

inline void initMain(int argc, char** argv)
{
    initMain__(argc, argv);
}