#include "CppMathParser.h"

#include <sstream>

namespace nate 
{
void CppMathParser::init()
{
	MathParser::init();

	addConstant("𝑒", "e",     "n::e", NumberType::Real);
	addConstant("π", "pi",    "n::pi", NumberType::Real);
	addConstant("𝑖", "i",     "i", NumberType::Real);
	addConstant("α", "alpha", "n::alpha", NumberType::Real);
	addConstant("δ", "delta", "n::delta", NumberType::Real);
	addConstant("φ", "phi",   "n::phi", NumberType::Real);

	addFunction("sin", "std::sin");
	addFunction("cos", "std::cos");
	addFunction("tan", "std::tan");
}

std::string CppMathParser::codeOperator(const MathValue& aMathValue) const
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
	case Oper::Brackets:
	{
		ss << mathString(aMathValue.embedded1) << "[" << mathString(aMathValue.embedded2) << "]";
		break;
	}
	case Oper::FunctionCall:
	{
		if (needsParens(aMathValue.embedded1))
		{
			 ss << aMathValue.symbol.codeName << "(" << mathString(aMathValue.embedded2) << ")";
		}
		else
		{
			 ss << aMathValue.symbol.codeName << mathString(aMathValue.embedded2);
		}
		break;
	}
	case Oper::FunctionName:
	{
		ss << aMathValue.symbol.codeName;
		break;
	}
	case Oper::Ceiling:
	{
		ss << "std::ceil(" << mathString(aMathValue.embedded1) << ")";
		break;
	}
	case Oper::Floor:
	{
		ss << "std::floor(" << mathString(aMathValue.embedded1) << ")";
		break;
	}
	case Oper::Absolute:
	{
		ss << "std::abs(" << mathString(aMathValue.embedded1) << ")";
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
		ss << "std::sqrt(" << mathString(aMathValue.embedded1) << ")";
		break;
	}
	case Oper::Power:
	{
		ss << "std::pow(" << mathString(aMathValue.embedded1) << ", " 
								      << mathString(aMathValue.embedded2) << ")";
		break;
	}
	case Oper::Exponential:
	{
		ss << "std::exp(" << mathString(aMathValue.embedded1) << ")";
		break;
	}
	case Oper::Symbol:
	{
		ss << aMathValue.symbol.codeName;
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
	case Oper::Assignment:
	{
		ss << mathString(aMathValue.embedded1) << " = " << mathString(aMathValue.embedded2);
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

} // namespace nate