#pragma once

#include "MathParser.h"

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

} // namespace
