#pragma once

#include <string>
#include <tuple>

extern std::string toCodeName(const std::string& aName);
extern std::string escapedMatch(const std::string& aString);
extern std::string toCodeWord(const std::string& aWord);
extern std::string unquote(const std::string& aString);
extern std::tuple<std::string, std::string> fromMonomial(const std::string& aString);

