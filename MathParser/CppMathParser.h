#pragma once

#include "MathParser.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace nate
{
class CppMathParser : public MathParser
{
public:
	MATHPARSER_API ~CppMathParser() override = default;
	void init() override;
		
protected:	
	MATHPARSER_API std::string codeOperator(const MathValue& aMathValue) const override;
};

namespace n {
	static const long double e     = M_E;
	static const long double pi    = M_PI;
	static const long double alpha = 2.502907875095892822283902873218L;
	static const long double delta = 4.669201609102990671853203820466L;
	static const long double phi   = 1.618033988749894848204586834365L;
} // namespace n

} // namespace nate
