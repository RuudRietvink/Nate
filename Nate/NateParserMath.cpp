#include "NateParserMath.h"
#include "Identifier.h"

NateParserMath::NateParserMath(NateParser& aNateParser)
  : mNateParser(aNateParser)
{
}

void NateParserMath::error(const Position& aPosition, const std::string& aError) const
{
  yy::position position(nullptr, (unsigned int)aPosition.y, (unsigned int)aPosition.x);
  mNateParser.error(position, aError);
}
	
bool NateParserMath::isSymbol(const std::string& aInput) const
{
  return !!mNateParser.getIdentifier(aInput);
}