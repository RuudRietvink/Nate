#pragma once

#include "MathParser.h"
#include "NateParser.h"

namespace nate
{
class NateParserMath : public MathParser
{
public:
	NateParserMath(NateParser& aNateParser);

private:
	void error(const InputPosition& aPosition, const std::string& aError) const override;
	std::string codeOperator(const MathValue& aMathValue) const override;

	NateParser& mNateParser;
};

}

