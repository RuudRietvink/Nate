#include "MathParser.h"
#include "utf8.h"

#include <iostream>
#include <sstream>
#define NOMINMAX  
#include <windows.h>
#include <algorithm>
#include <cctype>
#include <clocale>

namespace {
//   _
// √╱  ― ⌊⌋⌈⌉|⏐⎛⎞⎝⎠⎜⎟
// ⏨ю
// ⁰¹²³⁴⁵⁶⁷⁸⁹ᴬᴮᴰᴱᴳᴴᴵᴶᴷᴸᴹᴺᴼᴾᴿᵀᵁⱽᵂᵃᵇᶜᵈᵉᶠᵍʰⁱʲᵏˡᵐⁿᵒᵖʳˢᵗᵘᵛʷˣʸᶻ⁻⁺⁽⁾
// ₀₁₂₃₄₅₆₇₈₉ ₊₋₍₎ₐₑₒₓ
// ⅒⅑⅛⅐⅙⅕¼⅓½⅖⅔⅜⅗¾⅘⅝⅚⅞
// πτ𝑖⋅÷⁄×⇑⇓←

	const uint32_t LEFT_PARENTHESIS_UPPER_HOOK				= 0x239B; // ⎛
	const uint32_t LEFT_PARENTHESIS_EXTENSION					= 0x239C; // ⎜
	const uint32_t LEFT_PARENTHESIS_LOWER_HOOK				= 0x239D; // ⎝
	const uint32_t RIGHT_PARENTHESIS_UPPER_HOOK				= 0x239E; // ⎞
	const uint32_t RIGHT_PARENTHESIS_EXTENSION				= 0x239F; // ⎟
	const uint32_t RIGHT_PARENTHESIS_LOWER_HOOK				= 0x23A0; // ⎠
	const uint32_t LEFT_CEILING												= 0x2308; // ⌈
	const uint32_t RIGHT_CEILING											= 0x2309; // ⌉
	const uint32_t LEFT_FLOOR													= 0x230A; // ⌊
	const uint32_t RIGHT_FLOOR												= 0x230B; // ⌋
	const uint32_t LEFT_SQUARE_BRACKET_UPPER_CORNER		= 0x23A1; // ⎡
	const uint32_t LEFT_SQUARE_BRACKET_EXTENSION  		= 0x23A2; // ⎢
	const uint32_t LEFT_SQUARE_BRACKET_LOWER_CORNER		= 0x23A3; // ⎣
	const uint32_t RIGHT_SQUARE_BRACKET_UPPER_CORNER	= 0x23A4; // ⎤
	const uint32_t RIGHT_SQUARE_BRACKET_EXTENSION   	= 0x23A5; // ⎥
	const uint32_t RIGHT_SQUARE_BRACKET_LOWER_CORNER	= 0x23A6; // ⎦
	const uint32_t HORIZONTAL_BAR											= 0x2015; // ―
	const uint32_t ROOT_BAR														= 0x005F; //   _
	const uint32_t ROOT_DIAGONAL											= 0x2571; //  ╱
	const uint32_t SQUARE_ROOT												= 0x221A; // √
  const uint32_t E																	= 0x1D452;// 𝑒
  const uint32_t MULTIPLY_X													= 0x00D7; // ×
  const uint32_t MULTIPLY_STAR											= 0x2217; // ∗
  const uint32_t MULTIPLY_DOT												= 0x22C5; // ⋅
	const uint32_t DIVIDE_SLASH												= 0x002F; // /
	const uint32_t DIVIDE_SIGN												= 0x00F7; // ÷
	const uint32_t SUPER_OPEN													= 0x207D; // ⁽
	const uint32_t SUPER_CLOSE												= 0x207E; // ⁾
	const uint32_t ASSIGNMENT 												= 0x2190; // ←


	template< class charT >
	bool isalpha( charT ch, const std::locale& loc ) {
			return std::use_facet<std::ctype<charT>>(loc).is(std::ctype_base::alpha, ch);
	}
	template< class charT >
	bool isdigit( charT ch, const std::locale& loc ) {
			return std::use_facet<std::ctype<charT>>(loc).is(std::ctype_base::digit, ch);
	}
}

////////////////////////// public  /////////////////////////////////

void MathParser::setTabSize(uint32_t aTabSize)
{
	mTabSize = aTabSize;
}

void MathParser::addVariable(const std::string& aName)
{
	addSymbol({ Symbol::Type::Variable, aName, MathValue::Type::Real });
}

void MathParser::addConstant(const std::string& aName, MathValue::Type aType)
{
	addSymbol({ Symbol::Type::Constant, aName, aType });
}

std::string MathParser::doMath(std::istream& aStream, int line)
{
	Math math;
	math.x = 0;
	math.y = line;
	int pos = 0;
	math.matrix.push_back(MathVector());
	aStream.unsetf(std::ios_base::skipws);

	for (auto iter = std::istream_iterator<char>(aStream); iter != std::istream_iterator<char>();)
	{
		uint32_t kar = utf8::next(iter, std::istream_iterator<char>());
		if (kar == '\n')
		{
			pos = 0;
			math.matrix.push_back(MathVector()); 
		}
		else if (kar == '\t')
		{
	    for (int newPos = ((pos / mTabSize) + 1) * mTabSize; pos < newPos; ++pos)
			{
				math.matrix.back().push_back(SPACE);
			}
		}
		else if (kar != '\r' && kar != 0)
		{
			math.matrix.back().push_back(kar);
		}
	}

	return doMath(math);
}

std::string MathParser::doMath(Math& aMath)
{
	mSymbols["e"] = mSymbols["𝑒"] = Symbol{ Symbol::Type::Constant, "e", MathValue::Type::Real };
	mSymbols["π"] = Symbol{ Symbol::Type::Constant, "π", MathValue::Type::Real };
	mSymbols["i"] = mSymbols["j"] = mSymbols["𝑖"] = Symbol{ Symbol::Type::Constant, "i", MathValue::Type::Imaginary };

	fillUpMath(aMath);
	printDebugMath(false, aMath);
	doStartMathParsing(aMath);
	printDebugMath(false, aMath);
	return mathString(aMath);
}

////////////////////////// protected /////////////////////////////////

void MathParser::printDebugMath(bool print, const Math& aMath, const std::string& aText) const
{
	if (print)
	{
		printMath(aMath, aText);
	}
}

void MathParser::error(const Position& aPosition, const std::string& aError) const
{
	std::cerr << "(" << aPosition.y << "," << aPosition.x << "): " << aError << std::endl;
}

std::string MathParser::code(Oper aOper, const Math& aMathLeft, const Math& aMathRight) const
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
	case Oper::Brackets:
	{
		ss << "[" << mathString(aMathLeft) << "]";
		break;
	}
	case Oper::Ceiling:
	{
		ss << "ceil(" << mathString(aMathLeft) << ")";
		break;
	}
	case Oper::Floor:
	{
		ss << "floor(" << mathString(aMathLeft) << ")";
		break;
	}
	case Oper::Absolute:
	{
		ss << "abs(" << mathString(aMathLeft) << ")";
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
		ss << "pow(" << mathString(aMathLeft) << ", " 
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
		if (aMathRight.matrix[1][1].oper == Oper::Brackets)
		{
			ss << mathString(aMathLeft) << mathString(aMathRight);
		}
		else
		{
			ss << "(" << mathString(aMathLeft) << "*" << mathString(aMathRight) << ")";
		}
		break;
	}
	case Oper::Assignment:
	{
		ss << mathString(aMathLeft) << " = " << mathString(aMathRight);
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

uint32_t MathParser::operatorMultiply() const
{
	return '*';
}

uint32_t MathParser::operatorDivide() const
{
	return '/';
}

bool MathParser::isSymbol(const std::string& aInput) const
{
	return mSymbols.find(aInput) != mSymbols.end();
}

MathParser::Symbol MathParser::getSymbol(const std::string& aInput) const
{
	return mSymbols.at(aInput);
}

bool MathParser::isNumber(const std::string& aInput) const
{
	char* endPtr;
	strtod(aInput.c_str(), &endPtr);
	return *endPtr == '\0';
}

////////////////////////// private /////////////////////////////////

std::locale MathParser::m_localeUtf8("en_US.UTF8");

std::string MathParser::popBack(const std::string& aInput) const
{
	auto iter = aInput.end();
	if (!aInput.empty())
	{
		while (utf8::internal::is_trail(*(--iter)))
		{
			 if (iter == aInput.begin())
			 {
				 break;
			 }
		}
	}

	return std::string(aInput.begin(), iter);
}

std::string MathParser::popFront(const std::string& aInput) const
{
	auto iter = aInput.begin();
	if (!aInput.empty())
	{
		utf8::next(iter, aInput.end());
	}

	return std::string(iter, aInput.end());
}

void MathParser::fillUpMath(Math& aMath) const
{
	aMath.x--;
	aMath.y--;
	
	aMath.matrix.insert(aMath.matrix.begin(), MathVector());
	aMath.matrix.push_back(MathVector());

	size_t maxLength = 0;
	for (auto& row : aMath.matrix)
	{
		maxLength = std::max(row.size(), maxLength);
	}

	++maxLength;
	for (auto& row : aMath.matrix)
	{
		row.insert(row.begin(), SPACE);
		while (row.size() < maxLength)
		{
			row.push_back(SPACE);
		}
		
		row.push_back(SPACE);
	}
}
	
void MathParser::printMath(const Math& aMath, const std::string& aText) const
{
	if (!aText.empty())
	{
		std::cerr <<"--------------------------" << std::endl;
		std::cerr << aText << std::endl;
	}


	bool first = true;
	size_t y = 0;

	for (auto& row : aMath.matrix)
	{
		if (first)
		{
			first = false;
			std::cerr << ' ';
			for (size_t x = 0; x < row.size(); ++x)
			{
				std::cerr << x % 10;
			}
	
			std::cerr << '\n';
		}
		
		std::cerr << y++ % 10;

		for (auto& col : row)
		{
			if (!col.hasSubMatrix())
			{
				utf8::append(col.value, std::ostream_iterator<char>(std::cerr));
			}
			else if (col.isSubMatrix())
			{
				std::cerr << (col.isSuperscript() ?  "$" : "@");
			}
			else if (col.hasSubMatrix())
			{
				std::cerr << (col.isSuperscript() ?  "#" : "_");
			}
		}

		std::cerr << '\n';
	}
		
	std::cerr << std::endl;
}

bool MathParser::needsParens(const Math& aMath) const
{
	bool result = false;
	bool first = true;

	for (auto& row : aMath.matrix)
	{
		for (auto& col : row)
		{
			if (col.hasSubMatrix())
			{
				if (col.oper == Oper::Parentheses && first)
				{
					// single (...)?
				}
				else if (col.oper != Oper::Number && col.oper != Oper::Symbol)
				{
					result = true;
					break;
				}

				first = false;
			}
			else if (!isEmpty(col.value))
			{
				result = true;
				break;
			}
		}
	}

	return result;
}

std::string MathParser::mathString(const Math& aMath) const
{
	std::stringstream ss;
	
	for (auto& row : aMath.matrix)
	{
		for (auto& col : row)
		{
			if (col.isSubMatrix())
			{
				ss << code(col.oper, col.embedded1, col.embedded2);
			}
			else if (col.hasSubMatrix())
			{
				// nothing
			}
			else if (!isEmpty(col.value))
			{
				utf8::append(col.value, std::ostream_iterator<char>(ss));
			}
		}
	}

	return ss.str();
}

MathParser::Math MathParser::createSubMath(
				const Math& aMath, 
			  const Area& aArea, 
				const std::string& aText) const
{
	Math result;
	result.x = aMath.x + aArea.upperLeft.x;
	result.y = aMath.y + aArea.upperLeft.y;
	result.matrix.push_back(MathVector());
	
	auto iter = aText.begin();
	uint32_t kar;
	while (iter != aText.end() && (kar = utf8::next(iter, aText.end())) != 0)
	{
		result.matrix.back().push_back(kar);
	}

	fillUpMath(result);
	return result;
}

MathParser::Math MathParser::getSubMath(
				const Math& aMath, 
			  const Area& aArea) const
{
	Math result;
	result.x = aMath.x + aArea.upperLeft.x;
	result.y = aMath.y + aArea.upperLeft.y;

	for (int y = aArea.upperLeft.y; y <= aArea.lowerRight.y; ++y)
	{
		result.matrix.push_back(MathVector());

		for (int x = aArea.upperLeft.x; x <= aArea.lowerRight.x; ++x)
		{
			result.matrix.back().push_back(aMath.matrix[y][x]);
		}
	}

	fillUpMath(result);
	return result;
}

MathParser::MathValue* MathParser::embedSubMath(
				Math& aMath, 
				const Math& aSubMath, 
				Oper aOper,
				const Area& aArea)
{
	return embedSubMath(aMath, aSubMath, Math(), aOper, aArea);
}

MathParser::MathValue* MathParser::embedSubMath(
				Math& aMath, 
				const Math& aSubMath1, 
				const Math& aSubMath2, 
				Oper aOper,
				const Area& aArea)
{
	Size size{ aArea.lowerRight.x + 1 - aArea.upperLeft.x, aArea.lowerRight.y + 1 - aArea.upperLeft.y };
	auto mathValue = MathValue(aSubMath1, aSubMath2, aOper, size);
	fillerMath(aMath, aArea, mathValue.mathValue);
	aMath.matrix[aArea.upperLeft.y][aArea.upperLeft.x] = mathValue;
	return &aMath.matrix[aArea.upperLeft.y][aArea.upperLeft.x];
}

void MathParser::fillerMath(
				Math& aMath, 
				const Area& aArea,
				const MathValueSPtr& aClearValue)
{
	for (int y = aArea.upperLeft.y; y <= aArea.lowerRight.y; ++y)
	{
		for (int x = aArea.upperLeft.x; x <= aArea.lowerRight.x; ++x)
		{
			aMath.matrix[y][x] = MathValue(aClearValue, Size{ aArea.upperLeft.x - x,  aArea.upperLeft.y - y });
		}
	}
}

void MathParser::spaceMath(
				Math& aMath, 
				const Area& aArea)
{
	for (int y = aArea.upperLeft.y; y <= aArea.lowerRight.y; ++y)
	{
		for (int x = aArea.upperLeft.x; x <= aArea.lowerRight.x; ++x)
		{
			aMath(y, x) = SPACE;
		}
	}
}

void MathParser::doStartMathParsing(Math& aMath)
{
	doMathSuperscript(aMath);
	doMathSimpleOperators(aMath);
	doPrepareMathParsing(aMath);
}

void MathParser::doPrepareMathParsing(Math& aMath)
{
	doMathVariablesNumbers(aMath);
	doMathParentheses(aMath);
	doMathSimpleMatching(aMath, '(',          ')',           Oper::Parentheses, "parentheses");
	doMathSimpleMatching(aMath, '[',          ']',           Oper::Brackets,    "brackets");
	doMathSimpleMatching(aMath, '|',          '|',           Oper::Absolute,    "vertical bars");
	doMathSimpleMatching(aMath, LEFT_CEILING, RIGHT_CEILING, Oper::Floor,       "floor delimiters");
	doMathSimpleMatching(aMath, LEFT_FLOOR,   RIGHT_FLOOR,   Oper::Ceiling,     "ceiling delimiters");
	doMathParsing(aMath);
}

void MathParser::doMathParsing(Math& aMath)
{
	doMathParentheses(aMath);
	doMathBrackets(aMath);
	doMathSquareRoot(aMath);
	doMathFractionBar(aMath);
	doMathPower(aMath);
	doMathMonomial(aMath);
	doMathUnaryLeadingOperator(aMath, '-', Oper::UnaryMinus);
	doMathUnaryLeadingOperator(aMath, '+', Oper::UnaryPlus);
	doMathDownRightOperator(aMath, operatorMultiply(), Oper::Multiplication);
	doMathDownRightOperator(aMath, operatorDivide(), Oper::Division);
	doMathDownRightOperator(aMath, '+', Oper::Addition);
	doMathDownRightOperator(aMath, '-', Oper::Subtraction);
	doMathUpLeftOperator(aMath, ASSIGNMENT, Oper::Assignment);
}

void MathParser::doMathParentheses(Math& aMath)
{
	OptPosition leftUpperParenthesis = findAny(aMath, LEFT_PARENTHESIS_UPPER_HOOK);
	if (leftUpperParenthesis)
	{
		OptPosition rightUpperParenthesis = findMatchingBig(aMath, *leftUpperParenthesis, LEFT_PARENTHESIS_UPPER_HOOK, RIGHT_PARENTHESIS_UPPER_HOOK, "parentheses");
		if (rightUpperParenthesis)
		{
			OptPosition leftLowerParenthesis = findMatchingDown(aMath, *leftUpperParenthesis, 
																													LEFT_PARENTHESIS_EXTENSION,
																													LEFT_PARENTHESIS_LOWER_HOOK);
			if (leftLowerParenthesis)
			{
				OptPosition rightLowerParenthesis = findMatchingDown(aMath, *rightUpperParenthesis, 
																													 	 RIGHT_PARENTHESIS_EXTENSION,
																														 RIGHT_PARENTHESIS_LOWER_HOOK);
				if (rightLowerParenthesis)
				{
					Position leftPos = Position{ leftUpperParenthesis->x+1, leftUpperParenthesis->y };
					Position rightPos = Position{ rightLowerParenthesis->x - 1, rightLowerParenthesis->y };
					Math sub = getSubMath(aMath, Area{ leftPos, rightPos });
					
					printDebugMath(false, sub, __FUNCTION__);
					doMathParsing(sub);
					embedSubMath(aMath, sub, Oper::Parentheses, Area{ *leftUpperParenthesis, *rightLowerParenthesis });

					printDebugMath(false, aMath, __FUNCTION__);
					doMathParsing(aMath);
				}
				else
				{
					error(mathPos(aMath, *rightUpperParenthesis), "missing matching ⎠ or ⎟ below ⎞");
				}
			}
			else
			{
				error(mathPos(aMath, *leftUpperParenthesis), "missing matching ⎝ or ⎜ below ⎞");
			}
		}
		else
		{
			error(mathPos(aMath, *leftUpperParenthesis), "missing matching ⎞ at the right of a ⎛");
		}
	}
	printDebugMath(false, aMath, __FUNCTION__);
}

void MathParser::doMathBrackets(Math& aMath)
{
	OptPosition leftUpperBracket = findAny(aMath, LEFT_SQUARE_BRACKET_UPPER_CORNER);
	if (leftUpperBracket)
	{
		OptPosition rightUpperBracket = findMatchingBig(aMath, *leftUpperBracket, LEFT_SQUARE_BRACKET_UPPER_CORNER, RIGHT_SQUARE_BRACKET_UPPER_CORNER, "brackets");
		if (rightUpperBracket)
		{
			OptPosition leftLowerBracket = findMatchingDown(aMath, *leftUpperBracket, 
																											LEFT_SQUARE_BRACKET_EXTENSION,
																											LEFT_SQUARE_BRACKET_LOWER_CORNER);
			if (leftLowerBracket)
			{
				OptPosition rightLowerBracket = findMatchingDown(aMath, *rightUpperBracket, 
																											   RIGHT_SQUARE_BRACKET_EXTENSION,
																											   RIGHT_SQUARE_BRACKET_LOWER_CORNER);
				if (rightLowerBracket)
				{
					Position leftPos = Position{ leftUpperBracket->x+1, leftUpperBracket->y };
					Position rightPos = Position{ rightLowerBracket->x - 1, rightLowerBracket->y };
					Math sub = getSubMath(aMath, Area{ leftPos, rightPos });
					
					printDebugMath(false, sub, __FUNCTION__);
					doMathParsing(sub);
					embedSubMath(aMath, sub, Oper::Brackets, Area{ *leftUpperBracket, *rightLowerBracket });

					printDebugMath(false, aMath, __FUNCTION__);
					doMathParsing(aMath);
				}
				else
				{
					error(mathPos(aMath, *rightUpperBracket), "missing matching ⎦ or ⎢ below ⎤");
				}
			}
			else
			{
				error(mathPos(aMath, *leftUpperBracket), "missing matching ⎦ or ⎢ below ⎤");
			}
		}
		else
		{
			error(mathPos(aMath, *leftUpperBracket), "missing matching ⎤ at the right of a ⎡");
		}
	}
	printDebugMath(false, aMath, __FUNCTION__);
}

void MathParser::doMathFractionBar(Math& aMath)
{
	OptPosition leftHorizontalBar = findAny(aMath, HORIZONTAL_BAR);
	if (leftHorizontalBar)
	{
		printDebugMath(false, aMath, __FUNCTION__);
		OptPosition rightHorizontalBar = findRepeatingRight(aMath, *leftHorizontalBar, HORIZONTAL_BAR);

		Math subNumerator;
		Math subDenomenator;
		OptPosition leftUpperPosition = findTopOfFraction(aMath, *leftHorizontalBar, rightHorizontalBar->x);
		if (leftUpperPosition)
		{
			leftUpperPosition->x = leftHorizontalBar->x;
			Position rightPos = Position{ rightHorizontalBar->x, rightHorizontalBar->y - 1};
			Area areaNumerator = Area{ *leftUpperPosition, rightPos };
			subNumerator = getSubMath(aMath, areaNumerator);
			printDebugMath(false, subNumerator);

			OptPosition leftLowerPosition = findBottomOfFraction(aMath, *leftHorizontalBar, rightHorizontalBar->x);
			if (leftLowerPosition)
			{
			  leftLowerPosition->x = leftHorizontalBar->x;
				Position leftPos = Position{ leftHorizontalBar->x, leftHorizontalBar->y + 1};
				rightPos = Position{ rightHorizontalBar->x, leftLowerPosition->y };
				Area areaDenomenator = Area{ leftPos, rightPos };
				subDenomenator = getSubMath(aMath, areaDenomenator);
				printDebugMath(false, subDenomenator);
		
				doMathParsing(subNumerator);
				doMathParsing(subDenomenator);

				Area area = join(areaNumerator, areaDenomenator);
				embedSubMath(aMath, subNumerator, subDenomenator, Oper::Division, area);
				
				printDebugMath(false, aMath);
				doMathParsing(aMath);
			}
			else
			{
					error(mathPos(aMath, *leftHorizontalBar), "expected some expression below division bar");
			}
		}
		else
		{
				error(mathPos(aMath, *leftHorizontalBar), "expected some expression above division bar");
		}
		printDebugMath(false, aMath);
	}
}

void MathParser::doMathSquareRoot(Math& aMath)
{
	OptPosition squareRoot = findAny(aMath, SQUARE_ROOT);
	if (squareRoot)
	{
		OptPosition rootBar = findDiagonalRightUp(aMath, *squareRoot, ROOT_DIAGONAL, ROOT_BAR);
		if (rootBar)
		{
			OptPosition lastRootBar = findRepeatingRight(aMath, *rootBar, ROOT_BAR);
			Position leftPos = Position{ squareRoot->x + 1, rootBar->y+1 };
			Position diagonal = *squareRoot;

			while (aMath(diagonal.y - 1, diagonal.x + 1) == ROOT_DIAGONAL)
			{
				aMath(diagonal.y - 1, diagonal.x + 1) = SPACE;
				diagonal = diagonal.up().right();
			}
			
			Position rightPos = Position{ lastRootBar->x, squareRoot->y };
			Math sub = getSubMath(aMath, Area{ leftPos, rightPos });
			printDebugMath(false, sub, __FUNCTION__);
			doMathParsing(sub);
			leftPos = Position{ squareRoot->x, rootBar->y };
			embedSubMath(aMath, sub, Oper::SquareRoot, Area{ leftPos, rightPos });
			
			printDebugMath(false, aMath, __FUNCTION__);
			doMathParsing(aMath);
		}
		else
		{
			Position startSymbol = (*squareRoot).right();
			OptArea symbol = getSymbol(aMath, startSymbol);
			if (symbol)
			{
				Math sub = getSubMath(aMath, *symbol);
				printDebugMath(false, sub, __FUNCTION__);
			  doMathParsing(sub);
				embedSubMath(aMath, sub, Oper::SquareRoot, *symbol);
				spaceMath(aMath, Area{ *squareRoot, *squareRoot });
				
				printDebugMath(false, aMath, __FUNCTION__);
			  doMathParsing(aMath);
			}
			else
			{
				error(*squareRoot, "need symbol following square root (perhaps use root bar(s)");
			}
		}
	}
	printDebugMath(false, aMath, __FUNCTION__);
}

void MathParser::doMathPower(Math& aMath)
{
  auto [powerFound, isSuperscriptBase, isSuperscriptExp, expArea, baseArea] = findPower(aMath);

	if (powerFound)
	{
		printDebugMath(false, aMath, __FUNCTION__);
		Math exp = getSubMath(aMath, expArea);
		printDebugMath(false, exp);
		Math base = getSubMath(aMath, baseArea);
		printDebugMath(false, base);
		doMathParsing(exp);
		doMathParsing(base);
		
		if (isSuperscriptBase)
		{
			spaceMath(aMath, baseArea);
			baseArea.lowerRight = expArea.lowerRight;
			baseArea.upperLeft.y = expArea.upperLeft.y;
		}
		else if (isSuperscriptExp && baseArea.lowerRight.y == expArea.lowerRight.y)
		{
			baseArea.lowerRight = expArea.lowerRight;
		}
		else
		{
			spaceMath(aMath, expArea);
		}

		bool area1 = baseArea.upperLeft.x == baseArea.lowerRight.x && baseArea.upperLeft.y == baseArea.lowerRight.y;
		bool isSub = aMath.isSubMatrix(baseArea.upperLeft);
		bool isSym = aMath.mathValue(baseArea.upperLeft).oper == Oper::Symbol;
		if (area1 && isSub && isSym)
		{
			std::string text = mathString(aMath.mathValue(baseArea.upperLeft).embedded1);
			if (text == "e") 
		  {
				embedSubMath(aMath, exp, Oper::Exponential, baseArea);
			}
			else
			{
				embedSubMath(aMath, base, exp, Oper::Power, baseArea);
			}
		}
		else
		{
			embedSubMath(aMath, base, exp, Oper::Power, baseArea);
		}
		
		printDebugMath(false, aMath);
		doMathParsing(aMath);
	}
	printDebugMath(false, aMath, __FUNCTION__);
}

void MathParser::doMathVariablesNumbers(Math& aMath)
{
	for (int y = 0; y < aMath.height(); ++y)
	{
		for (int x = 0; x < aMath.width(); ++x)
		{
			uint32_t kar = aMath(y, x);
			int endX = -1;
			if (isPartOfNumber(kar) && kar != '+' && kar != '-')
			{
				endX = parseNumber(aMath, x, y) - 1;
				if (endX >= x)
				{
					Position startPos{ x, y };
					Position endPos{ endX, y };
					Math nbr = getSubMath(aMath, Area{ startPos, endPos });
					embedSubMath(aMath, nbr, Oper::Number, Area{ startPos, endPos });
				}
			}
			
			if (endX < x && isVarStart(kar))
			{
				auto [endX, symbol] = parseVariable(aMath, x, y);
				if (endX >= x)
				{
					Position startPos{ x, y };
					Position endPos{ endX, y };
					Math var = createSubMath(aMath, Area{ startPos, endPos}, symbol.name);
					embedSubMath(aMath, var, Oper::Symbol, Area{ startPos, endPos });
				}
			}
		}
	}
		
	printDebugMath(false, aMath, __FUNCTION__);
}

void MathParser::doMathMonomial(Math& aMath)
{
	for (int y = 0; y < aMath.height(); ++y)
	{
	int lastX = -1;
	MathValue* lastMathValue = nullptr;

		for (int x = 0; x < aMath.width() - 1; ++x)
		{
			if (aMath.hasSubMatrix(y, x))
			{
				if (lastMathValue == nullptr || aMath.matrix[y][x].mathValue != lastMathValue->mathValue)
				{
					if (lastX != -1)
					{
						MathValueSPtr& rightMathValue = aMath.matrix[y][x].mathValue;
						Area leftArea = lastMathValue->getArea(lastX, y);
						Area rightArea = aMath.matrix[y][x].getArea(x, y);
						Math left = getSubMath(aMath, leftArea);
						Math right = getSubMath(aMath, rightArea);
						Area area = join(leftArea, rightArea);
						if (lastMathValue->type == MathValue::Type::Imaginary || rightMathValue->type == MathValue::Type::Imaginary)
						{
						  if (lastMathValue->type == MathValue::Type::Imaginary && rightMathValue->type == MathValue::Type::Imaginary)
							{
								Math number = createSubMath(aMath, area, "-1");
								embedSubMath(aMath, number, Oper::Number, area);
							}
							else if (lastMathValue->type == MathValue::Type::Imaginary)
							{
								embedSubMath(aMath, right, Oper::Number, area)->type = MathValue::Type::Imaginary;
							}
							else
							{
								embedSubMath(aMath, left, Oper::Number, area)->type = MathValue::Type::Imaginary;
							}
						}
						else
						{
						  embedSubMath(aMath, left, right, Oper::Monomial, area);
						}

						printDebugMath(false, aMath, __FUNCTION__);
					}
					else
					{
						lastMathValue = &aMath.matrix[y][x];
						lastX = x;
					}
				}
			}
			else if (lastX != -1)
			{
				lastX = -1;
				lastMathValue = nullptr;
			}
		}
	}
	printDebugMath(false, aMath, __FUNCTION__);
}

void MathParser::doMathUnaryLeadingOperator(Math& aMath, int aOperChar, Oper aOper)
{
	for (int y = 0; y < aMath.height(); ++y)
	{
		for (int x = 0; x < aMath.width(); ++x)
		{
			uint32_t kar = aMath(y, x);
			if (kar == aOperChar)
			{
				auto [isSuperscript, leftArea] = getRightToLeftSymbol(aMath, Position{ x - 1, y }, true);
				// no operand left?
				if (!leftArea)
				{
					auto rightArea = getSymbol(aMath, Position{ x + 1, y }, true);
					if (rightArea)
					{
						Math right = getSubMath(aMath, *rightArea);
						rightArea->upperLeft.x = x;
						printDebugMath(false, right, __FUNCTION__);
						doMathParsing(right);
						embedSubMath(aMath, right, aOper, Area{ rightArea->upperLeft, rightArea->lowerRight });
						printDebugMath(false, aMath, __FUNCTION__);
					}
				}
			}
		}
	}
}

void MathParser::doMathDownRightOperator(Math& aMath, int aOperChar, Oper aOper)
{
	for (int y = 0; y < aMath.height(); ++y)
	{
		for (int x = 0; x < aMath.width(); ++x)
		{
			uint32_t kar = aMath(y, x);
			if (kar == aOperChar)
			{
				doMathOperator(aMath, aOper, x, y);
			}
		}
	}
}

void MathParser::doMathUpLeftOperator(Math& aMath, uint32_t aOperChar, Oper aOper)
{
	for (int y = aMath.height() - 1; y >= 0; --y)
	{
		for (int x = aMath.width() - 1; x >= 0; --x)
		{
			uint32_t kar = aMath(y, x);
			if (kar == aOperChar)
			{
				doMathOperator(aMath, aOper, x, y);
			}
		}
	}
}

void MathParser::doMathOperator(Math& aMath, Oper aOper, int x, int y)
{
	auto [isSuperscript, leftArea] = getRightToLeftSymbol(aMath, Position{ x - 1, y }, true);
	if (leftArea)
	{
		auto rightArea = getSymbol(aMath, Position{ x + 1, y }, true);
		if (rightArea)
		{
			Math left = getSubMath(aMath, *leftArea);
			Math right = getSubMath(aMath, *rightArea);
			printDebugMath(false, left, __FUNCTION__);
			doMathParsing(left);
			printDebugMath(false, right, __FUNCTION__);
			doMathParsing(right);
			Area area = join(*leftArea, *rightArea);
			embedSubMath(aMath, left, right, aOper, area);
			printDebugMath(false, aMath, __FUNCTION__);
		}
	}
	else
	{
		error(Position{ x, y }, "Unmatched operator");
	}
}

void MathParser::doMathSimpleOperators(Math& aMath)
{
	for (int y = 0; y < aMath.height(); ++y)
	{
		for (int x = 0; x < aMath.width(); ++x)
		{
			uint32_t kar = aMath(y, x);
			if (kar == MULTIPLY_DOT ||
					kar == MULTIPLY_STAR ||
					kar == MULTIPLY_X ||
					kar == '*')
			{
				kar = operatorMultiply();
			}
			else if (kar == DIVIDE_SIGN ||
					     kar == DIVIDE_SLASH ||
							 kar == '/')
			{
				kar = operatorDivide();
			}

			if (kar != aMath(y, x))
			{
				aMath(y, x) = kar;
			}
		}
	}
}

void MathParser::doMathSuperscript(Math& aMath)
{
	for (int y = 0; y < aMath.height(); ++y)
	{
		for (int x = 0; x < aMath.width(); ++x)
		{
			bool isSuperScript = (getSuperscript(aMath(y, x)) != BADCHAR);
			if (isSuperScript)
			{
				std::vector<uint32_t> prevParens;
				int blankCount = 0;
				uint32_t kar;
				int startX = x;

				for (; x < aMath.width() && (kar = optSuperscript(isSuperScript, aMath(y, x))) != BADCHAR &&
						   (!prevParens.empty() || blankCount < 4); ++x)
				{
					aMath(y, x) = kar;

					if (kar == '(' || kar == ')')
					{
						blankCount = 0;
						if (kar == '(')
						{
							prevParens.push_back(')');
						}
						else if (kar == ')')
						{
							if (prevParens.empty() || prevParens.back() != kar)
							{
								error(Position{ x, y }, "unbalanced parentheses/brackets");
							}
							else
							{
								prevParens.pop_back();
							}
						}
					}
					else
					{
						if (isBlank(kar))
						{
							++blankCount;
						}
						else
						{
							blankCount = 0;
						}
					}
				}

				if (!prevParens.empty())
				{
					error(Position{ x, y }, "unbalanced parentheses");
				}

				x = x - 1 - blankCount;
				Area area{ Position{ startX, y }, Position{ x, y } };
				Math superMath = getSubMath(aMath, Area{ Position{ startX, y }, Position{ x, y } });
				doPrepareMathParsing(superMath);
				MathValue* mathValue = embedSubMath(aMath, superMath, Oper::Nested, area);
				mathValue->superscript = true;
			}
		}
	}
		
	printDebugMath(false, aMath, __FUNCTION__);
}

void MathParser::doMathSimpleMatching(Math& aMath, uint32_t left, uint32_t right, Oper oper, const char* desc)
{
	for (int y = 0; y < aMath.height(); ++y)
	{
		int upY = y;
		int downY = y;
		for (int x = 0; x < aMath.width(); ++x)
		{
			uint32_t kar = aMath(y, x);
			if (kar == left)
			{
				std::vector<uint32_t> prevParens;
				int startX = x;
				bool lastOperator = false;
				bool lastOpen = true;
				prevParens.push_back(right);
				++x;

				for (;x < aMath.width(); ++x)
				{
					kar = aMath(y, x);
					if (kar == left)
					{
						if (left == right)
						{
							if (lastOperator || lastOpen)
							{
								prevParens.push_back(right);
							}
							else
							{
								if (prevParens.empty())
								{
									error(Position{ x, y }, std::string("unbalanced ") + desc);
								}
								else
								{
									prevParens.pop_back();
								}
							}
						}
						else
						{
							prevParens.push_back(right);
							lastOpen = true;
						}
					}
					else if (kar == right)
					{
						if (prevParens.empty() || prevParens.back() != kar)
						{
							error(Position{ x, y }, std::string("unbalanced ") + desc);
						}
						else
						{
							prevParens.pop_back();
						}
					}
					else 
					{
						if (isOperator(kar))
						{
							lastOperator = true;
							lastOpen = false;
						}
						else if (!isBlank(kar))
						{
							lastOperator = false;
							lastOpen = false;
						}

						for (int up = 1; y - up > 0; ++up)
						{
							uint32_t upKar = aMath(y - up, x);
							if (!isBlank(upKar) && !badSomethingVertical(upKar))
							{
								upY = y - up;
							}
							else
							{
								break;
							}
						}
						
						for (int down = 1; y + down < aMath.height() - 1; ++down)
						{
							uint32_t downKar = aMath(y + down, x);
							if (!isBlank(downKar) && !badSomethingVertical(downKar))
							{
								downY = y + down;
							}
						}
					}

					if (prevParens.empty())
					{
						break;
					}
				}
		
				if (!prevParens.empty())
				{
					error(Position{ x, y }, std::string("unbalanced ") + desc);
				}
				else
				{				
					Area area{ Position{ startX, upY }, Position{ x, downY } };
					Math parens = getSubMath(aMath, Area{ Position{ startX + 1, upY }, Position{ x - 1, downY } });
					doPrepareMathParsing(parens);
					embedSubMath(aMath, parens, oper, area);
				}
			}
			else if (kar == right)
			{
				error(Position{ x, y }, std::string("unbalanced ") + desc);
			}
		}
	}

	printDebugMath(false, aMath, __FUNCTION__);
}

bool MathParser::isPartOfNumber(uint32_t kar) const
{
	return kar < 255 &&
		     (isdigit(kar) || 
				  kar == '.' || 
				  kar == '-' || 
			    kar == '+' || 
				  kar == 'E' || 
				  kar == 'e');
}

int MathParser::parseNumber(
				const Math& aMath,
				int x,
				int y) const
{
	int start = x;
	std::string number;
	auto backIns = std::back_inserter(number);

	int size = aMath.width();
	while (x < size && isPartOfNumber(aMath(y, x)))
	{
		utf8::append(aMath(y, x), backIns);
		++x;
	}

	for (;x > start && !isNumber(number); --x)
	{
		number = popBack(number);
	}

	if (number.empty())
	{
		x = -1;
	}

	return x;
}

bool MathParser::isVarStart(uint32_t kar) const
{
	return isalpha(kar, m_localeUtf8);
}

bool MathParser::isVarNext(uint32_t kar) const
{
	return isVarStart(kar) || isdigit(kar, m_localeUtf8) || isSymbolSuffix(kar) || kar == '_';
}

std::tuple<int, MathParser::Symbol> MathParser::parseVariable(
				const Math& aMath,
				int x,
				int y) const
{
	Symbol symbol;
	if (isVarStart(aMath(y, x)))
	{
		int start = x;
		std::string variable;
		auto backIns = std::back_inserter(variable);

		int size = aMath.width();
		for (; x < size && isVarNext(aMath(y, x)); ++x)
		{
			utf8::append(aMath(y, x), backIns);
		}

		--x;
		for (;x >= start && !isSymbol(variable); --x)
		{
			variable = popBack(variable);
		}

		if (variable.empty())
		{
			x = -1;
		}
		else
		{
			symbol = getSymbol(variable);
		}
	}
	else
	{
		x = -1;
	}

	return std::make_tuple(x, symbol);
}

MathParser::Area MathParser::join(const Area& aLeftArea, const Area& aRightArea) const
{
  return Area{ Position{ std::min(aLeftArea.upperLeft.x, aRightArea.upperLeft.x), std::min(aLeftArea.upperLeft.y, aRightArea.upperLeft.y) },
						   Position{ std::max(aLeftArea.lowerRight.x, aRightArea.lowerRight.x), std::max(aLeftArea.lowerRight.y, aRightArea.lowerRight.y) } };
}

MathParser::Area MathParser::totalArea(const Math& aMath, const Area& aArea) const
{
	Area result = aArea;

	for (int y = aArea.upperLeft.y; y <= aArea.lowerRight.y; ++y)
	{
		for (int x = aArea.upperLeft.x; x <= aArea.lowerRight.x; ++x)
		{
			if (aMath.hasSubMatrix(y, x))
			{
				result = join(result, aMath.matrix[y][x].getArea(x, y));
			}
		}
	}
	return result;
}

bool MathParser::isOperator(uint32_t kar) const
{
	return kar == operatorDivide() || 
		     kar == operatorMultiply() ||
		     kar == '+' || 
		     kar == '-';
}

bool MathParser::isBlank(uint32_t kar) const
{
	return kar == SPACE;
}

bool MathParser::isEmpty(uint32_t kar) const
{
	return isBlank(kar) || kar == BADCHAR;
}

MathParser::OptArea MathParser::getSymbol(
				const Math& aMath, 
				const Position& aLeftPosition,
				bool aAllowSpaces) const
{
	OptArea result;
	std::vector<uint32_t> prevParens;
	int x = aLeftPosition.x;
	int y = aLeftPosition.y;
	int size = (int)aMath.width();

	if (aAllowSpaces)
	{
		for (; x < size && isBlank(aMath(y, x)); ++x)
			;
	}

	Position start{ x, y };

	for (; x < size && !result; ++x)
	{
		uint32_t kar = aMath(y, x);
		if (kar == '(' || kar == ')' || kar == '[' || kar == ']')
		{
			if (kar == '(')
			{
				prevParens.push_back(')');
			}
			else if (kar == '[')
			{
				prevParens.push_back(']');
			}
			else if (kar == ')' || kar == ']')
			{
				if (prevParens.empty() || prevParens.back() != kar)
				{
					error(Position{ x, y }, "unbalanced parentheses/brackets");
				}
				else
				{
					prevParens.pop_back();
				}
			}

			if (prevParens.empty())
			{
				result = Area{ start, Position{ x, y } };
			}
		}
		else if (prevParens.empty())
		{
			if (isBlank(kar))
			{
				result = Area{ start, Position{ x-1, y } };
			}
			else if (aMath.hasSubMatrix(y, x))
			{
				result = aMath.matrix[y][x].getArea(x, y);
			}
			else 
			{
				break;
			}
		}
	}

	if (!prevParens.empty())
	{
		error(Position{ x, y }, "unbalanced parentheses/brackets");
	}

	return result;
}

	std::tuple<bool, MathParser::OptArea> MathParser::getRightToLeftSymbol(
				const Math& aMath, 
				const Position& aRightPosition,
				bool aAllowSpaces) const
{
	OptArea result;
	int x = aRightPosition.x;
	int y = aRightPosition.y;
	
	if (aAllowSpaces)
	{
		for (; x >= 0 && isBlank(aMath(y, x)); --x)
			;
	}
	
	bool isSuperScript = false;

	if (x >= 0)
	{
		if (aMath.hasSubMatrix(y, x))
		{
			result = aMath.matrix[y][x].getArea(x, y);
		  isSuperScript = aMath.isSuperscript(y, x) || (result && aMath.isSuperscript(y, result->upperLeft.x));
		}
	}
	
	return std::make_tuple(isSuperScript, result);
}

bool MathParser::isSymbolSuffix(uint32_t aKar) const
{
	return ((aKar >= 0x2080U && aKar <= 0x2089U) || // subscripts
					(aKar >= 0x2090U && aKar <= 0x209CU));
}

MathParser::Area MathParser::getEndExponent(
				const Math& aMath,
				const Position& aStartExponent) const
{
	Area result;

	int x = aStartExponent.x;
	while (x < aMath.width() - 1 && 
					isBlank(aMath(aStartExponent.y+1, x)))
	{
		++x;
	}

	while (isBlank(aMath(aStartExponent.y, x)) && x >= aStartExponent.x)
	{
		--x;
	}

	result = totalArea(aMath, Area{ aStartExponent, Position{ x, aStartExponent.y } });

	return result;
}

uint32_t MathParser::optSuperscript(bool aCheckSuperScript, uint32_t aKar) const
{
	return aCheckSuperScript && aKar != SPACE
		     ? getSuperscript(aKar) : aKar;
}

uint32_t MathParser::getSuperscript(uint32_t aKar) const
{
	static const std::string superScript = "⁽⁾⁺⁻⁰¹²³⁴⁵⁶⁷⁸⁹ᴬᴮᴰᴱᴳᴴᴵᴶᴷᴸᴹᴺᴼᴾᴿᵀᵁⱽᵂᵃᵇᶜᵈᵉᶠᵍʰⁱʲᵏˡᵐⁿᵒᵖʳˢᵗᵘᵛʷˣʸᶻ";
	static const std::string normlScript = "()+-0123456789ABDEGHIJKLMNOPRTUVWabcdefghijklmnoprstuvwxyz";

	auto iter = superScript.begin();
	uint32_t cp;
	for (size_t index = 0; iter != superScript.end() && (cp = utf8::next(iter, superScript.end())) != 0; ++index)
	{
		if (aKar == cp)
		{
			return normlScript[index];
		}
	}

	return 0;
}

std::tuple<bool, bool, bool, MathParser::Area, MathParser::Area> MathParser::findPower(
				Math& aMath) const
{
	for (int y = 0; y < aMath.height() - 1; ++y)
	{
		for (int x = 1; x < aMath.width(); ++x)
		{
			if (!isBlank(aMath(y, x)) && isBlank(aMath(y, x-1)) &&
					isBlank(aMath(y+1, x)) && !isBlank(aMath(y+1, x-1)))
			{
				Area expArea = getEndExponent(aMath, Position{ x, y });	
				auto [isSuperscript, baseArea] = getRightToLeftSymbol(aMath, Position{ expArea.upperLeft.x - 1, expArea.lowerRight.y + 1 });
				if (baseArea)
				{
					return std::make_tuple(true, isSuperscript, false, expArea, *baseArea);
				}
			}
		}
	}
	
	for (int y = 0; y < aMath.height(); ++y)
	{
		for (int x = 1; x < aMath.width(); ++x)
		{
			if (aMath.isSubMatrix(y, x) && aMath.isSuperscript(y, x))
			{
				auto [isSuperscriptBase, baseArea] = getRightToLeftSymbol(aMath, Position{ x - 1, y });
				if (baseArea)
				{
					return std::make_tuple(true, isSuperscriptBase, true, aMath.matrix[y][x].getArea(x, y), *baseArea);
				}
			}
		}
	}
	
	return std::make_tuple(false, false, false, Area{}, Area{});
}

MathParser::OptPosition MathParser::findAnyOf(
				const Math& aMath, 
				const std::initializer_list<uint32_t>& aSearchChars) const
{
	for (auto const& searchChar : aSearchChars)
	{
		OptPosition position = findAny(aMath, searchChar);
		if (position)
		{
			return position;
		}
	}

	return std::nullopt;
}

MathParser::OptPosition MathParser::findAny(
				const Math& aMath, 
				uint32_t aSearchChar) const
{
	for (int y = 0; y < aMath.height(); ++y)
	{
		for (int x = 0; x < aMath.width(); ++x)
		{
			if (aMath(y, x) == aSearchChar)
			{
				return Position{ x, y };
			}
		}
	}

	return std::nullopt;
}

MathParser::OptPosition MathParser::findMatchingBig(
				const Math& aMath, 
				const Position& aLeftPosition,
				uint32_t aLeftChar,
				uint32_t aRightChar,
				const char* aDesc) const
{
	OptPosition result;
	int nested = 1;

	int x = aLeftPosition.x + 1;
	int y = aLeftPosition.y;

	for (; nested > 0 && x < aMath.width(); ++x)
	{
		if (aMath(y, x) == aLeftChar)
		{
			++nested;
		}
		else if (aMath(y, x) == aRightChar)
		{
			--nested;
		}
	}
	
	if (nested != 0)
	{
		error(Position{ aLeftPosition.x, y }, std::string("unbalanced ") + aDesc);
	}

	return Position{ x - 1, y };
}

MathParser::OptPosition MathParser::findRepeatingRight(
				const Math& aMath, 
				const Position& aLeftLowerPosition, 
				uint32_t aSearchChar) const
{
	int y = aLeftLowerPosition.y;

	for (int x = aLeftLowerPosition.x; x < aMath.width(); ++x)
	{
		if (aMath(y, x) != aSearchChar)
		{
			return Position{ x - 1, y };
		}
	}

	return std::nullopt;
}

MathParser::OptPosition MathParser::findMatchingDown(
				const Math& aMath, 
				const Position& aUpperPosition,
				uint32_t aInbetweenChar,
				uint32_t aSearchChar) const
{
	int x = aUpperPosition.x;

	for (int y = aUpperPosition.y + 1; y < aMath.height(); ++y)
	{
		uint32_t kar = aMath(y, x);
		if (aInbetweenChar != 0 && kar == aInbetweenChar)
		{
			// skip
		}
		else if (kar == aSearchChar)
		{
			return Position{ x, y };
		}
		else
		{
			mathError(aMath, Position{ x, y }, kar, aInbetweenChar, aSearchChar);
			break;
		}
	}

	return std::nullopt;
}
	
bool MathParser::badSomethingVertical(uint32_t kar) const
{
	return (kar == HORIZONTAL_BAR || kar == ROOT_BAR);
}

MathParser::OptPosition MathParser::findDiagonalRightUp(
				const Math& aMath, 
				const Position& aLeftLowerPosition,
				uint32_t aInbetweenChar,
				uint32_t aSearchChar) const
{
	int x = aLeftLowerPosition.x + 1;

	for (int y = aLeftLowerPosition.y - 1; y > 0 && x < aMath.width(); --y, ++x)
	{
		uint32_t kar = aMath(y, x);
		if (aInbetweenChar != 0 && kar == aInbetweenChar)
		{
			// skip
		}
		else if (kar == aSearchChar)
		{
			return Position{ x, y };
		}
		else
		{
			break;
		}
	}

	return std::nullopt;
}

std::tuple<bool, bool> MathParser::isFractionBar(
				const Math& aMath, 
				const Position& aPosition,
				int aRightX) const
{
	bool isBlank = true;
	bool isBar = true;
	for (int x = aPosition.x; (isBlank || isBar) && x <= aRightX; ++x)
	{
		uint32_t kar = aMath(aPosition.y, x);
		isBar = isBar && (kar == HORIZONTAL_BAR);
		isBlank = isBlank && (kar == ' ');
	}

	return std::make_tuple(isBar, isBlank);
}

MathParser::OptPosition MathParser::findTopOfFraction(
				const Math& aMath, 
				const Position& aLowerPosition,
				int aRightX) const
{
	OptPosition result;
	
	for (int y = aLowerPosition.y - 1; !result && y >= 0; --y)
	{
		auto position = Position{ aLowerPosition.x, y };
		auto [isBar, isBlank] = isFractionBar(aMath, position, aRightX);
		if (isBar || isBlank)
		{
			result = Position{ aLowerPosition.x, y + 1 };
		}
	}

	if (!result)
	{
		if (aLowerPosition.y > 0)
		{
			result = Position{ aLowerPosition.x, 1 };
		}
	}

	if (result && result->y == aLowerPosition.y)
	{
	  result = std::nullopt;
	}

	return result;
}

MathParser::OptPosition MathParser::findBottomOfFraction(
				const Math& aMath, 
				const Position& aUpperPosition,
				int aRightX) const
{
	OptPosition result;

	for (int y = aUpperPosition.y + 1; !result && y < aMath.height(); ++y)
	{
		auto position = Position{ aUpperPosition.x, y };
		auto [isBar, isBlank] = isFractionBar(aMath, position, aRightX);
		if (isBar || isBlank)
		{
			result = Position{ aUpperPosition.x, y - 1 };
		}
	}

	
	if (!result)
	{
		if (aUpperPosition.y < aMath.height() - 1)
		{
			result = Position{ aUpperPosition.x, aMath.height() - 2 };
		}
	}
	
	if (result && result->y == aUpperPosition.y)
	{
	  result = std::nullopt;
	}

	return result;
}

MathParser::Position MathParser::mathPos(const Math& aMath, const Position& aPosition) const
{
	return Position{ (int)aMath.y + aPosition.y, (int)aMath.x + aPosition.x };
}

void MathParser::mathError(
				const Math& aMath, 
				const Position& aPosition,
				uint32_t aBadChar,
				uint32_t aInbetweenChar,
				uint32_t aSearchChar) const
{
	std::stringstream ss;

	ss << "bad character found '";
	utf8::append(aBadChar, std::ostream_iterator<char>(ss));
	ss << "', expected '";

	if (aInbetweenChar != 0)
	{
		utf8::append(aInbetweenChar, std::ostream_iterator<char>(ss));
		ss << "', or '";
	}

	utf8::append(aSearchChar, std::ostream_iterator<char>(ss));
	ss << "'";

	error(mathPos(aMath, aPosition), ss.str());
}


	void MathParser::addSymbol(const Symbol& aSymbol)
	{
		if (aSymbol.name == "i" || aSymbol.name == "j" || aSymbol.name == "e" ||
				aSymbol.name == "π" || aSymbol.name == "𝑖")
		{
			error(Position(), "Reserved name: " + aSymbol.name);
		}
		else
		{
			if (mSymbols.find(aSymbol.name) != mSymbols.end())
			{
				error(Position(), "Duplicate name: " + aSymbol.name);
			}
			else
			{
				mSymbols[aSymbol.name] = aSymbol;
			}
		}
	}