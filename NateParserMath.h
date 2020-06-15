#pragma once

#include "ParserMath.h"
#include "NateParser.h"

class NateParserMath : public ParserMath
{
public:
	NateParserMath(NateParser& aNateParser);

private:
	void error(const Position& aPosition, const std::string& aError) const override;

	NateParser& mNateParser;
};
