#include "NateParserMath.h"
#include "Identifier.h"

namespace nate
{
NateParserMath::NateParserMath(NateParser& aNateParser)
  : mNateParser(aNateParser)
{
}

void NateParserMath::error(const InputPosition& aPosition, const std::string& aError) const
{
    nate::position position(nullptr, (unsigned int)aPosition.y, (unsigned int)aPosition.x);
    mNateParser.error(position, aError);
}
	
std::string NateParserMath::codeOperator(const MathValue& aMathValue) const
{
	std::stringstream ss;

	switch (aMathValue.oper)
	{
	case Oper::Parentheses:
	{
		if (needsParens(aMathValue.embedded1))
		{
			ss << "(" << mathString(aMathValue.embedded1) << ")";
		}
		else
		{
			ss << mathString(aMathValue.embedded1);
		}
		break;
	}
	case Oper::Multiplication:
	{
		ss << mathString(aMathValue.embedded1) << " * " << mathString(aMathValue.embedded2);
		break;
	}
	case Oper::Division:
	{
		ss << "(";
		if (needsParens(aMathValue.embedded1))
		{
			ss << "(" << mathString(aMathValue.embedded1) << ")";
		}
		else
		{
			ss << mathString(aMathValue.embedded1);
		} 
		
		ss << " / ";

		if (needsParens(aMathValue.embedded2))
		{
			ss << "(" << mathString(aMathValue.embedded2) << ")";
		}
		else
		{
			ss << mathString(aMathValue.embedded2);
		} 
								
		ss << ")";
		break;
	}
	case Oper::Addition:
	{
		ss << mathString(aMathValue.embedded1) << " + " << mathString(aMathValue.embedded2);
		break;
	}
	case Oper::Subtraction:
	{
		ss << mathString(aMathValue.embedded1) << " - " << mathString(aMathValue.embedded2);
		break;
	}
	case Oper::UnaryMinus:
	{
		ss << "-" << mathString(aMathValue.embedded1);
		break;
	}
	case Oper::UnaryPlus:
	{
		ss << "+" << mathString(aMathValue.embedded1);
		break;
	}
	case Oper::SquareRoot:
	{
		ss << "sqrt(" << mathString(aMathValue.embedded1) << ")";
		break;
	}
	case Oper::Power:
	{
		ss << "(" << mathString(aMathValue.embedded1) << ")^(" 
							<< mathString(aMathValue.embedded2) << ")";
		break;
	}
	case Oper::Exponential:
	{
		ss << "exp(" << mathString(aMathValue.embedded1) << ")";
		break;
	}
	case Oper::Symbol:
	{
		ss << mathString(aMathValue.embedded1);
		break;
	}
	case Oper::Number:
	{
		ss << mathString(aMathValue.embedded1);
		break;
	}
	case Oper::Monomial:
	{
		ss << "(" << mathString(aMathValue.embedded1) << "*" << mathString(aMathValue.embedded2) << ")";
		break;
	}
	case Oper::Nested:
	{
		ss << mathString(aMathValue.embedded1);
		break;
	}
	}

	return ss.str();
}
}
