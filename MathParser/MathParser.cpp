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

	const uint32_t LEFT_PARENTHESIS_UPPER_HOOK  = 0x239B; // ⎛
	const uint32_t LEFT_PARENTHESIS_EXTENSION   = 0x239C; // ⎜
	const uint32_t LEFT_PARENTHESIS_LOWER_HOOK  = 0x239D; // ⎝
	const uint32_t RIGHT_PARENTHESIS_UPPER_HOOK = 0x239E; // ⎞
	const uint32_t RIGHT_PARENTHESIS_EXTENSION  = 0x239F; // ⎟
	const uint32_t RIGHT_PARENTHESIS_LOWER_HOOK = 0x23A0; // ⎠
	const uint32_t HORIZONTAL_BAR               = 0x2015; // ―
	const uint32_t ROOT_BAR                     = 0x005F; //   _
	const uint32_t ROOT_DIAGONAL                = 0x2571; //  ╱
	const uint32_t SQUARE_ROOT                  = 0x221A; // √
  const uint32_t E                            = 0x1D452;// 𝑒
  const uint32_t MULTIPLY_X                   = 0x00D7; // ×
  const uint32_t MULTIPLY_STAR                = 0x2217; // ∗
  const uint32_t MULTIPLY_DOT                 = 0x22C5; // ⋅
	const uint32_t DIVIDE_SLASH                 = 0x002F; // /
	const uint32_t DIVIDE_SIGN                  = 0x00F7; // ÷
	const uint32_t SUPER_OPEN                   = 0x207D; // ⁽
	const uint32_t SUPER_CLOSE                  = 0x207E; // ⁾


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
	addSymbol({ Symbol::Type::Variable, aName, "" });
}

void MathParser::addConstant(const std::string& aName, const std::string& aValue)
{
	addSymbol({ Symbol::Type::Constant, aName, aValue });
}

std::string MathParser::doMath(std::istream& aStream, int line)
{
	mSymbols["e"] = Symbol{ Symbol::Type::Constant, "e", "M_E" };
	mSymbols["E"] = Symbol{ Symbol::Type::Constant, "e", "M_E" };
	mSymbols["π"] = Symbol{ Symbol::Type::Constant, "π", "M_PI" };

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
	fillUpMath(aMath);
	printDebugMath(aMath);
	doStartMathParsing(aMath);
	printDebugMath(aMath);
	return mathString(aMath);
}

////////////////////////// protected /////////////////////////////////

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
	case Oper::Division:
	{
		ss << "(" << code(Oper::Parentheses, aMathLeft) << " / "
							<< code(Oper::Parentheses, aMathRight) << ")";
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
	case Oper::Multiplication:
	{
		break;
	}
	case Oper::Variable:
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
		ss << "(" << mathString(aMathLeft) << (char)(operatorMultiply()) << mathString(aMathRight) << ")";
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

std::string MathParser::getRightToLeftVariable(const std::string& aInput) const
{
	std::string result;
	std::string possible = aInput;

	while (possible.size() > 0)
	{
		if (!isSymbol(possible))
		{
			possible = popFront(possible);
		}
		else
		{
			result = possible;
			break;
		}
	}

	return result;
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
		for (auto& col : row)
		{
			if (col.isSubMatrix())
			{
				++col.mathValue->area.upperLeft.x;
				++col.mathValue->area.upperLeft.y;
				++col.mathValue->area.lowerRight.y;
				++col.mathValue->area.lowerRight.y;
			}
		}
	}
}
	
void MathParser::printDebugMath(const Math& aMath) const
{
	printMath(aMath);
}

void MathParser::printMath(const Math& aMath) const
{
	std::cerr <<"--------------------------" << std::endl;
	for (auto& row : aMath.matrix)
	{
		for (auto& col : row)
		{
			if (!col.hasSubMatrix())
			{
				utf8::append(col.value, std::ostream_iterator<char>(std::cerr));
			}
			else if (col.isSubMatrix())
			{
				std::cerr << "@";
			}
			else if (col.hasSubMatrix())
			{
				std::cerr << "_";
			}
		}

		std::cerr << std::endl;
	}
}

void MathParser::printDebugMath(
				const Math& aMath,
				const Area& aArea) const
{
	printMath(getSubMath(aMath, aArea));
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
				else if (col.oper != Oper::Number && col.oper != Oper::Variable)
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

void MathParser::embedSubMath(
				Math& aMath, 
				const Math& aSubMath, 
				Oper aOper,
				const Area& aArea,
				const Position& aPlacePosition)
{
	embedSubMath(aMath, aSubMath, Math(), aOper, aArea, aPlacePosition);
}

void MathParser::embedSubMath(
				Math& aMath, 
				const Math& aSubMath1, 
				const Math& aSubMath2, 
				Oper aOper,
				const Area& aArea,
			  const Position& aPlacePosition)
{
	auto mathValue = MathValue(aSubMath1, aSubMath2, aOper, aArea);
	fillerMath(aMath, aArea, mathValue.mathValue);
	aMath.matrix[aPlacePosition.y][aPlacePosition.x] = mathValue;
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
			aMath.matrix[y][x] = MathValue(aClearValue);
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
	doMathSimpleOperators(aMath);
	doMathParsing(aMath);
}

void MathParser::doMathParsing(Math& aMath)
{
	doMathParentheses(aMath);
	doMathSquareRoot(aMath);
	doMathFractionBar(aMath);
	doMathPower(aMath);
	doMathVariablesNumbers(aMath);
	doMathMonomial(aMath);
}

void MathParser::doMathParentheses(Math& aMath)
{
	OptPosition leftUpperParenthesis = findAny(aMath, LEFT_PARENTHESIS_UPPER_HOOK);
	if (leftUpperParenthesis)
	{
		OptPosition rightUpperParenthesis = findMatchingBigParens(aMath, *leftUpperParenthesis);
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
					
					Position place = *leftLowerParenthesis;
					OptPosition anchor = findSomethingLeft(aMath, *leftUpperParenthesis, *leftLowerParenthesis);
					if (!anchor)
					{
						anchor = findSomethingRight(aMath, *rightUpperParenthesis, *rightLowerParenthesis);
						if (anchor)
						{
							place.y = anchor->y;
						}
					}
					else
					{
						place.y = anchor->y;
						place.x = anchor->x + 1;
					}

					printDebugMath(sub);
					doMathParsing(sub);
					embedSubMath(aMath, sub, Oper::Parentheses, Area{ *leftUpperParenthesis, *rightLowerParenthesis }, place);

					printDebugMath(aMath);
					doMathParsing(aMath);
				}
				else
				{
					error(mathPos(aMath, *rightUpperParenthesis), "missing matching ⎞ at the right of a ⎛");
				}
			}
			else
			{
				error(mathPos(aMath, *leftLowerParenthesis), "missing matching ⎝ or ⎜ below ⎞");
			}
		}
		else
		{
			error(mathPos(aMath, *leftUpperParenthesis), "missing matching ⎠ or ⎟ below ⎞");
		}
	}
}

void MathParser::doMathFractionBar(Math& aMath)
{
	OptPosition leftHorizontalBar = findAny(aMath, HORIZONTAL_BAR);
	if (leftHorizontalBar)
	{
		OptPosition rightHorizontalBar = findRepeatingRight(aMath, *leftHorizontalBar, HORIZONTAL_BAR);

		Math subNumerator;
		Math subDenomenator;
		Position midHorizontalBar{ (rightHorizontalBar->x + leftHorizontalBar->x) / 2, leftHorizontalBar->y };
		OptPosition leftUpperPosition = findUntilUp(aMath, midHorizontalBar, HORIZONTAL_BAR);
		if (leftUpperPosition)
		{
			leftUpperPosition->x = leftHorizontalBar->x;
			Position rightPos = Position{ rightHorizontalBar->x, rightHorizontalBar->y - 1};
			subNumerator = getSubMath(aMath, Area{ *leftUpperPosition, rightPos });
			printDebugMath(subNumerator);

			OptPosition leftLowerPosition = findUntilDown(aMath, midHorizontalBar, HORIZONTAL_BAR);
			if (leftLowerPosition)
			{
			  leftLowerPosition->x = leftHorizontalBar->x;
				Position leftPos = Position{ leftHorizontalBar->x, leftHorizontalBar->y + 1};
				rightPos = Position{ rightHorizontalBar->x, leftLowerPosition->y };
				subDenomenator = getSubMath(aMath, Area{ leftPos, rightPos });
				printDebugMath(subDenomenator);
		
				doMathParsing(subNumerator);
				doMathParsing(subDenomenator);

				embedSubMath(aMath, subNumerator, subDenomenator, Oper::Division, Area{ *leftUpperPosition, rightPos },
										 *leftHorizontalBar);
				
				printDebugMath(aMath);
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
			printDebugMath(sub);
			doMathParsing(sub);
			leftPos = Position{ squareRoot->x, rootBar->y };
			embedSubMath(aMath, sub, Oper::SquareRoot, Area{ leftPos, rightPos }, *squareRoot);
			
			printDebugMath(aMath);
			doMathParsing(aMath);
		}
		else
		{
			Position startSymbol = (*squareRoot).right();
			OptArea symbol = getSymbol(aMath, startSymbol);
			if (symbol)
			{
				Math sub = getSubMath(aMath, *symbol);
				printDebugMath(sub);
				embedSubMath(aMath, sub, Oper::SquareRoot, *symbol, startSymbol);
				spaceMath(aMath, Area{ *squareRoot, *squareRoot });
				
				printDebugMath(aMath);
			  doMathParsing(aMath);
			}
			else
			{
				error(*squareRoot, "need symbol following square root (perhaps use root bar(s)");
			}
		}
	}
}

void MathParser::doMathPower(Math& aMath)
{
  auto [isSuperscript, expArea, baseArea] = findPower(aMath);

	if (expArea)
	{
		Math exp = getSubMath(aMath, *expArea);
		printDebugMath(exp);
		Math base = getSubMath(aMath, baseArea);
		printDebugMath(base);
		doMathParsing(exp);
		doMathParsing(base);
		

		if (baseArea.upperLeft.x == baseArea.lowerRight.x && baseArea.upperLeft.y == baseArea.lowerRight.y &&
				(aMath(baseArea.upperLeft.y, baseArea.upperLeft.x) == E ||
				 aMath(baseArea.upperLeft.y, baseArea.upperLeft.x) == 'e'))
		{
			embedSubMath(aMath, exp, Oper::Exponential, baseArea, baseArea.lowerRight);
			spaceMath(aMath, *expArea);
		}
		else
		{
			if (isSuperscript)
			{
			  spaceMath(aMath, baseArea);
				expArea->upperLeft.x = baseArea.upperLeft.x;
				embedSubMath(aMath, base, exp, Oper::Power, *expArea, Position{ expArea->upperLeft.x, expArea->lowerRight.y });
			}
			else
			{
			 	embedSubMath(aMath, base, exp, Oper::Power, baseArea, baseArea.lowerRight);
			  spaceMath(aMath, *expArea);
			}
		}
		
		printDebugMath(aMath);
		doMathParsing(aMath);
	}
}

void MathParser::doMathVariablesNumbers(Math& aMath)
{
	for (int y = 0; y < aMath.height(); ++y)
	{
		for (int x = 0; x < aMath.width(); ++x)
		{
			uint32_t kar = aMath(y, x);
			if (isPartOfNumber(kar) && kar != '+' && kar != '-')
			{
				int endX = parseNumber(aMath, x, y) - 1;
				if (endX >= x)
				{
					Position startPos{ x, y };
					Position endPos{ endX, y };
					Math nbr = getSubMath(aMath, Area{ startPos, endPos });
					embedSubMath(aMath, nbr, Oper::Number, Area{ startPos, endPos }, startPos);
				}
			}
			else if (isVarStart(kar))
			{
				int endX = parseVariable(aMath, x, y) - 1;
				Position startPos{ x, y };
				Position endPos{ endX, y };
				Math var = getSubMath(aMath, Area{ startPos, endPos });
			  embedSubMath(aMath, var, Oper::Variable, Area{ startPos, endPos }, startPos);
			}
		}
	}
}
void MathParser::doMathMonomial(Math& aMath)
{
	for (int y = 0; y < aMath.height(); ++y)
	{
		int lastX = -1;
		for (int x = 0; x < aMath.width() - 1; ++x)
		{
			if (aMath.isSubMatrix(y, x))
			{
				if (lastX != -1)
				{
					Position startPos{ lastX, y };
					Position endPos{ x, y };
					Math left = getSubMath(aMath, Area{ startPos, startPos });
					Math right = getSubMath(aMath, Area{ endPos, endPos });
					embedSubMath(aMath, left, right, Oper::Monomial, Area{ startPos, endPos }, startPos);
					printDebugMath(aMath);
				}
				else
				{
					lastX = x;
				}
			}
			else if (aMath.hasSubMatrix(y, x))
			{
				// nothing
			}
			else if (lastX != -1)
			{
				lastX = -1;
			}
		}
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
					kar == MULTIPLY_X)
			{
				kar = operatorMultiply();
			}
			else if (kar == DIVIDE_SIGN ||
					     kar == DIVIDE_SLASH )
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

bool MathParser::isPartOfNumber(uint32_t kar) const
{
	return isdigit(kar) || 
				 kar == '.' || 
				 kar == '-' || 
			   kar == '+' || 
				 kar == 'E' || 
				 kar == 'e';
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

	return x;
}

int MathParser::parseRightToLeftNumber(
			  bool isSuperScript,
				const Math& aMath,
				int x,
				int y) const
{
	if (isPartOfNumber(optSuperscript(isSuperScript, aMath(y, x))))
	{
		int start = x;
		std::string number;

		int size = aMath.width();
		while (x >= 0 && isPartOfNumber(optSuperscript(isSuperScript, aMath(y, x))))
		{
			std::string part;
			auto backIns = std::back_inserter(part);
			utf8::append(optSuperscript(isSuperScript, aMath(y, x)), backIns);
			number.insert(number.begin(), part.begin(), part.end());
			--x;
		}

		++x;

		for (;x <= start && !isNumber(number); ++x)
		{
			number = popFront(number);
		}
	}

	return x;
}



bool MathParser::isVarStart(uint32_t kar) const
{
	return isalpha(kar, m_localeUtf8);
}

bool MathParser::isVarNext(uint32_t kar) const
{
	return isVarStart(kar) || isdigit(kar) || isSymbolSuffix(kar) || kar == '_';
}

int MathParser::parseVariable(
				const Math& aMath,
				int x,
				int y) const
{
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

		for (;x > start && !isSymbol(variable); --x)
		{
			variable = popBack(variable);
		}
	}

	return x;
}

int MathParser::parseRightToLeftVariable(
				bool isSuperScript,
				const Math& aMath,
				int x,
				int y) const
{
	if (isVarNext(optSuperscript(isSuperScript, aMath(y, x))))
	{
		int start = x;
		std::string variable;

		int size = aMath.width();
		for (; x >= 0 && isVarNext(optSuperscript(isSuperScript, aMath(y, x))); --x)
		{
			std::string part;
			auto backIns = std::back_inserter(part);
			utf8::append(optSuperscript(isSuperScript, aMath(y, x)), backIns);
			variable.insert(variable.begin(), part.begin(), part.end());
		}

		++x;

		for (;x <= start && !isSymbol(variable); ++x)
		{
			variable = popFront(variable);
		}
	}

	return x;
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
				result = aMath.matrix[y][x].mathValue->area;
			}
			else 
			{
				int nextX;
				if ((kar == '-' || kar == '+') && x < size -1 && aMath.hasSubMatrix(y, x+1))
				{
					auto area = aMath.matrix[y][x+1].mathValue->area;
					result = Area{ area.upperLeft.left(), area.lowerRight };
				}
				else if ((nextX = parseNumber(aMath, x, y)) > x)
				{
					result = Area{ start, Position{ nextX - 1, y } };
				}
				else if ((nextX = parseVariable(aMath, x, y)) > x)
				{
					result = Area{ start, Position{ nextX - 1, y } };
				}
				else
				{
					std::string desc("unknown token: ");
					auto backIns = std::back_inserter(desc);
					utf8::append(kar, backIns);
					error(Position{ x, y }, desc);
				}
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
	
	bool isSuperScript = (x >= 0 && getSuperscript(aMath(y, x)) != BADCHAR);
	std::vector<uint32_t> prevParens;

	for (; x >= 0 && !result; --x)
	{
		uint32_t kar = optSuperscript(isSuperScript, aMath(y, x));
		
		if (kar == '(' || kar == ')' || kar == '[' || kar == ']')
		{
			if (kar == ')')
			{
				prevParens.push_back('(');
			}
			else if (kar == ']')
			{
				prevParens.push_back('[');
			}
			else if (kar == '(' || kar == '[')
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
				result = Area{ Position{ x, y }, aRightPosition };
			}
		}
		else if (prevParens.empty())
		{
			if (isBlank(kar) || kar == BADCHAR)
			{
				result = Area{ Position{ x+1, y }, aRightPosition };
			}
			else if (aMath.hasSubMatrix(y, x))
			{
				result = aMath.matrix[y][x].mathValue->area;
			}
			else 
			{
				int startX;
				if ((startX = parseRightToLeftNumber(isSuperScript, aMath, x, y)) <= x)
				{
					result = Area{ Position{ startX, y }, aRightPosition };
				}
				else if ((startX = parseRightToLeftVariable(isSuperScript, aMath, x, y)) <= x)
				{
					result = Area{ Position{ startX, y }, aRightPosition };
				}
				else
				{
					std::string desc("unknown token: ");
					auto backIns = std::back_inserter(desc);
					utf8::append(kar, backIns);
					error(Position{ x, y }, desc);
				}
			}
		}
	}

	if (!prevParens.empty())
	{
		error(Position{ x, y }, "unbalanced parentheses");
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
	if (aMath.hasSubMatrix(aStartExponent.y, aStartExponent.x))
	{
		result = aMath.matrix[aStartExponent.y][aStartExponent.x].mathValue->area;
	}
	else
	{
		int x = aStartExponent.x;
		for (; x < aMath.width() - 1 && 
					isBlank(aMath(aStartExponent.y+1, x)); ++x)
			;

		result = Area{ aStartExponent, Position{ x - 1, aStartExponent.y } };
	}

	return result;
}

uint32_t MathParser::optSuperscript(bool aCheckSuperScript, uint32_t aKar) const
{
	return aCheckSuperScript ? getSuperscript(aKar) : aKar;
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

void MathParser::unsuperscript(
				Math& aMath,
				const Position& aLeftPosition,
				const Position& aRightPosition) const
{
	for (int x = aLeftPosition.x; x <= aRightPosition.x; ++x)
	{
		aMath(aLeftPosition.y, x) = getSuperscript(aMath(aLeftPosition.y, x));
	}
}

std::tuple<bool, MathParser::OptArea, MathParser::Area> MathParser::findPower(
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
					if (isSuperscript)
					{
						unsuperscript(aMath, baseArea->upperLeft, baseArea->lowerRight);
					}

					return std::make_tuple(isSuperscript, expArea, *baseArea);
				}
			}
		}
	}
	
	for (int y = 0; y < aMath.height(); ++y)
	{
		for (int x = 1; x < aMath.width(); ++x)
		{
			if (getSuperscript(aMath(y, x)) != BADCHAR)
			{
				auto [isSuperscript, baseArea] = getRightToLeftSymbol(aMath, Position{ x - 1, y });
				if (baseArea)
				{
					uint32_t subs;
					Position startExponent = Position{ x, y };
					int startX = x;
					while (x < aMath.width())
					{
						if (x > startX && isBlank(aMath(y, x)))
						{
							++x;
						}
						else if ((subs = getSuperscript(aMath(y, x))) != BADCHAR)
						{
							aMath(y, x) = subs;
							++x;
						}
						else
						{
							break;
						}
					}
					
					return std::make_tuple(false, Area{ startExponent, Position{ x - 1, y } }, *baseArea);
				}
			}
		}
	}
	
	return std::make_tuple(false, std::nullopt, Area{});
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

MathParser::OptPosition MathParser::findMatchingBigParens(
				const Math& aMath, 
				const Position& aLeftPosition) const
{
	OptPosition result;
	int parens = 1;

	int x = aLeftPosition.x + 1;
	int y = aLeftPosition.y;

	for (; parens > 0 && x < aMath.width(); ++x)
	{
		if (aMath(y, x) == LEFT_PARENTHESIS_UPPER_HOOK)
		{
			++parens;
		}
		else if (aMath(y, x) == RIGHT_PARENTHESIS_UPPER_HOOK)
		{
			--parens;
		}
	}
	
	if (parens != 0)
	{
		error(Position{ aLeftPosition.x, y }, "unbalanced parentheses");
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
	
bool MathParser::badSomething(uint32_t kar) const
{
	return (kar == ROOT_DIAGONAL);
}

MathParser::OptPosition MathParser::findSomethingLeft(
				const Math& aMath, 
				const Position& aLeftUpperPosition,
				const Position& aLeftLowerPosition) const
{
	for (int x = aLeftUpperPosition.x - 1; x > 0; --x)
	{
		for (int y = aLeftUpperPosition.y; y < aLeftLowerPosition.y; ++y)
		{
			if (badSomething(aMath(y, x)))
			{
				return std::nullopt;
			}
		}
		for (int y = aLeftUpperPosition.y; y < aLeftLowerPosition.y; ++y)
		{
			if (aMath(y, x) != SPACE)
			{
				return Position{ x, y };
			}
		}
	}

	return std::nullopt;
}

MathParser::OptPosition MathParser::findSomethingRight(
				const Math& aMath, 
				const Position& aRightUpperPosition,
				const Position& aRightLowerPosition) const
{
	for (int x = aRightUpperPosition.x + 1; x < aMath.width(); ++x)
	{
		for (int y = aRightUpperPosition.y; y < aRightLowerPosition.y; ++y)
		{
			if (badSomething(aMath(y, x)))
			{
				return std::nullopt;
			}
		}
		for (int y = aRightUpperPosition.y; y < aRightLowerPosition.y; ++y)
		{
			if (aMath(y, x) != SPACE)
			{
				return Position{ x, y };
			}
		}
	}

	return std::nullopt;
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

MathParser::OptPosition MathParser::findUntilUp(
				const Math& aMath, 
				const Position& aLowerPosition,
				uint32_t aSearchChar) const
{
	OptPosition result;

	int x = aLowerPosition.x;

	for (int y = aLowerPosition.y - 1; !result && y >= 0; --y)
	{
		uint32_t kar = aMath(y, x);
		if (kar == aSearchChar)
		{
			if (y < aLowerPosition.y - 1)
			{
				result = Position{ x, y + 1 };
			}
			else
			{
				break;
			}
		}
	}

	if (!result)
	{
		if (aLowerPosition.y > 0)
		{
			result = Position{ x, 1 };
		}
	}

	return result;
}

MathParser::OptPosition MathParser::findUntilDown(
				const Math& aMath, 
				const Position& aUpperPosition,
				uint32_t aSearchChar) const
{
	OptPosition result;

	int x = aUpperPosition.x;

	for (int y = aUpperPosition.y + 1; !result && y < aMath.height(); ++y)
	{
		uint32_t kar = aMath(y, x);
		if (kar == aSearchChar)
		{
			if (y > aUpperPosition.y + 1)
			{
				result = Position{ x, y - 1 };
			}
			else
			{
				break;
			}
		}
	}

	
	if (!result)
	{
		if (aUpperPosition.y < aMath.height() - 1)
		{
			result = Position{ x, aMath.height() - 2 };
		}
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