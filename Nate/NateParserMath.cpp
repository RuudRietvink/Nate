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

MathParser::Symbol NateParserMath::getSymbol(const std::string& aInput) const
{
  MathParser::Symbol result { Symbol::Type::Variable, aInput, MathValue::Type::Real };

	return result;
}

std::string NateParserMath::code(Oper aOper, const Math& aMathLeft, const Math& aMathRight) const
{
	std::stringstream ss;

	switch (aOper)
	{
	case Oper::Parentheses:
	{
		if (needsParens(aMathLeft))
		{
			ss << "(" << mathString(aMathLeft) << ")";
		}
		else
		{
			ss << mathString(aMathLeft);
		}
		break;
	}
	case Oper::Multiplication:
	{
		ss << mathString(aMathLeft) << " * " << mathString(aMathRight);
		break;
	}
	case Oper::Division:
	{
		ss << "(" << code(Oper::Parentheses, aMathLeft) << " / "
							<< code(Oper::Parentheses, aMathRight) << ")";
		break;
	}
	case Oper::Addition:
	{
		ss << mathString(aMathLeft) << " + " << mathString(aMathRight);
		break;
	}
	case Oper::Subtraction:
	{
		ss << mathString(aMathLeft) << " - " << mathString(aMathRight);
		break;
	}
	case Oper::UnaryMinus:
	{
		ss << "-" << mathString(aMathLeft);
		break;
	}
	case Oper::UnaryPlus:
	{
		ss << "+" << mathString(aMathLeft);
		break;
	}
	case Oper::SquareRoot:
	{
		ss << "sqrt(" << mathString(aMathLeft) << ")";
		break;
	}
	case Oper::Power:
	{
		ss << "(" << mathString(aMathLeft) << ")^(" 
							<< mathString(aMathRight) << ")";
		break;
	}
	case Oper::Exponential:
	{
		ss << "exp(" << mathString(aMathLeft) << ")";
		break;
	}
	case Oper::Symbol:
	{
		ss << mathString(aMathLeft);
		break;
	}
	case Oper::Number:
	{
		ss << mathString(aMathLeft);
		break;
	}
	case Oper::Monomial:
	{
		ss << "(" << mathString(aMathLeft) << "*" << mathString(aMathRight) << ")";
		break;
	}
	case Oper::Nested:
	{
		ss << mathString(aMathLeft);
		break;
	}
	}

	return ss.str();
}