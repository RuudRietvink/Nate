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
	Symbol getSymbol(const std::string& aInput) const override;
	std::string code(Oper aOper, const Math& aMathLeft, const Math& aMathRight = Math()) const override;

	NateParser& mNateParser;
};
