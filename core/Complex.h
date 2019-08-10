#pragma once

#include <string>
#include <iostream>
#include <complex>
#include "core.h"

class Complex
{
public:
	static bool isImaginaryLetter(uint32_t aLetter);
	static bool checkAndRemoveImaginaryLetter(std::string& aString);
};

