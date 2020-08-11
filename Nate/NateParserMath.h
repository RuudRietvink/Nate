#pragma once

#include "MathParser.h"
#include "NateParser.h"

class NateParserMath : public MathParser
{
public:
	NateParserMath(NateParser& aNateParser);

private:
	void error(const Position& aPosition, const std::string& aError) const override;
	bool isSymbol(const std::string& aInput) const override;

	NateParser& mNateParser;
};
