#include "ParserMath.h"
#include "utf8.h"

#include <iostream>
#include <sstream>
#define NOMINMAX  
#include <windows.h>
#include <algorithm>
#include <cctype>

namespace {
	const uint32_t SPACE                        = 0x20;  
	const uint32_t SUBMATRIX                    = 0x0; 

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
  const uint32_t MULTIPLY                     = 0x00D7; // ×
  const uint32_t DOT                          = 0x22C5; // ⋅

	const std::string OPER_LEFT_TO_RIGHT        = "left_to_right";
	const std::string OPER_PARENS               = "()";
	const std::string OPER_DIVIDE               = "/";
	const std::string OPER_SQUARE_ROOT          = "sqrt";
	const std::string OPER_POWER                = "pow";
	const std::string OPER_EXP                  = "exp";
	const std::string OPER_MULTIPLY             = "*";
}

void ParserMath::doMath(Math& aMath)
{
	fillUpMath(aMath);
	printMath(aMath);
	doMathParsing(aMath);
	printMath(aMath);
	std::cerr << mathString(aMath) << std::endl;
}

void ParserMath::fillUpMath(Math& aMath)
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
	
void ParserMath::printDebugMath(const Math& aMath) const
{
	//printMath(aMath);
}

void ParserMath::printMath(const Math& aMath) const
{
	std::cerr <<"--------------------------" << std::endl;
	for (auto& row : aMath.matrix)
	{
		for (auto& col : row)
		{
			if (col.value != SUBMATRIX)
			{
				utf8::append(col.value, std::ostream_iterator<char>(std::cerr));
			}
			else
			{
				std::cerr << "@";
			}
		}

		std::cerr << std::endl;
	}
	std::cerr <<"--------------------------" << std::endl;
}

std::string ParserMath::operParens(const Math& aMath) const
{
	std::stringstream ss;
	ss << "(" << mathString(aMath) << ")";
	return ss.str();
}

std::string ParserMath::operDivide(const Math& aMathLeft, const Math& aMathRight) const
{
	std::stringstream ss;
	ss << "((" << mathString(aMathLeft) << ") / (" 
						 << mathString(aMathRight) << "))";
	return ss.str();
}

std::string ParserMath::operSquareRoot(const Math& aMath) const
{
	std::stringstream ss;
	ss << "sqrt(" << mathString(aMath) << ")";
	return ss.str();
}

std::string ParserMath::operPower(const Math& aMathBase, const Math& aMathExp) const
{
	std::stringstream ss;
	ss << "pow(" << mathString(aMathBase) << ", " 
						   << mathString(aMathExp) << ")";
	return ss.str();
}

std::string ParserMath::operMultiply(const Math& aMathLeft, const Math& aMathRight) const
{
	std::stringstream ss;
	ss << "((" << mathString(aMathLeft) << ") * (" 
						 << mathString(aMathRight) << "))";
	return ss.str();
}

std::string ParserMath::operExp(const Math& aMathExp) const
{
	std::stringstream ss;
	ss << "exp(" << mathString(aMathExp) << ")";
	return ss.str();
}

std::string ParserMath::mathString(const Math& aMath) const
{
	std::stringstream ss;
	
	for (auto& row : aMath.matrix)
	{
		for (auto& col : row)
		{
			if (col.value == SUBMATRIX)
			{
				if (col.oper == OPER_LEFT_TO_RIGHT)
				{
					ss << mathString(col.embedded1) << mathString(col.embedded2);
				}
				else if (col.oper == OPER_PARENS)
				{
					ss << operParens(col.embedded1);
				}
				else if (col.oper == OPER_DIVIDE)
				{
					ss << operDivide(col.embedded1, col.embedded2);
				}
				else if (col.oper == OPER_SQUARE_ROOT)
				{
					ss << operSquareRoot(col.embedded1);
				}
				else if (col.oper == OPER_POWER)
				{
					ss << operPower(col.embedded1, col.embedded2);
				}
				else if (col.oper == OPER_EXP)
				{
					ss << operExp(col.embedded1);
				}
				else if (col.oper == OPER_MULTIPLY)
				{
					ss << operMultiply(col.embedded1, col.embedded2);
				}
			}
			else if (col.value != SPACE)
			{
				utf8::append(col.value, std::ostream_iterator<char>(ss));
			}
		}
	}

	return ss.str();
}

ParserMath::Math ParserMath::getSubMath(
				const Math& aMath, 
			  const Position& aLeftUpperPosition, 
			  const Position& aRightLowerPosition)
{
	Math result;
	result.x = aMath.x + aLeftUpperPosition.x;
	result.y = aMath.y + aLeftUpperPosition.y;

	for (int y = aLeftUpperPosition.y; y <= aRightLowerPosition.y; ++y)
	{
		result.matrix.push_back(MathVector());

		for (int x = aLeftUpperPosition.x; x <= aRightLowerPosition.x; ++x)
		{
			result.matrix.back().push_back(aMath.matrix[y][x]);
		}
	}

	fillUpMath(result);
	return result;
}

void ParserMath::embedSubMath(
				Math& aMath, 
				const Math& aSubMath, 
				const std::string& aOper,
				const Position& aLeftUpperPosition,
				const Position& aRightLowerPosition,
				const Position& aPlacePosition)
{
	embedSubMath(aMath, aSubMath, Math(), aOper, aLeftUpperPosition, aRightLowerPosition, aPlacePosition);
}

void ParserMath::embedSubMath(
				Math& aMath, 
				const Math& aSubMath1, 
				const Math& aSubMath2, 
				const std::string& aOper,
				const Position& aLeftUpperPosition,
				const Position& aRightLowerPosition,
			  const Position& aPlacePosition)
{
	clearMath(aMath, aLeftUpperPosition, aRightLowerPosition);
	
	aMath.matrix[aPlacePosition.y][aPlacePosition.x] = MathValue(aSubMath1, aSubMath2, aOper);
}

void ParserMath::clearMath(
				Math& aMath, 
				const Position& aLeftUpperPosition,
				const Position& aRightLowerPosition)
{
	for (int y = aLeftUpperPosition.y; y <= aRightLowerPosition.y; ++y)
	{
		for (int x = aLeftUpperPosition.x; x <= aRightLowerPosition.x; ++x)
		{
			aMath.matrix[y][x].value = SPACE;
		}
	}
}

void ParserMath::doMathParsing(Math& aMath)
{
	doMathParentheses(aMath);
	doMathDivision(aMath);
	doMathSquareRoot(aMath);
	doMathPower(aMath);
	doMathMultiplication(aMath);
}

void ParserMath::doMathParentheses(Math& aMath)
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
					Math sub = getSubMath(aMath, leftPos, rightPos);
					
					OptPosition anchor = findSomethingLeft(aMath, *leftUpperParenthesis, *leftLowerParenthesis);
					if (!anchor)
					{
						anchor = findSomethingRight(aMath, *rightUpperParenthesis, *rightLowerParenthesis);
						if (!anchor)
						{
							anchor = leftLowerParenthesis;
						}
						else
						{
							anchor->x--;
						}
					}
					else
					{
						anchor->x++;
					}

					if (rightUpperParenthesis->y > 0 && rightUpperParenthesis->x < aMath.width() &&
							aMath.matrix[rightUpperParenthesis->y - 1][rightUpperParenthesis->x + 1].value != SPACE)
					{
						//std::cerr << "Parens Power" << aMath.y + anchor->y << "," << aMath.x + anchor->x << std::endl;
						printDebugMath(sub);
						Position startExponent = Position{ rightUpperParenthesis->x + 1, rightUpperParenthesis->y - 1 };
						Position endExponent = getEndExponent(aMath, startExponent);
						Math exp = getSubMath(aMath, startExponent, endExponent);
						printDebugMath(exp);
						doMathParsing(sub);
						doMathParsing(exp);
						clearMath(aMath, startExponent, endExponent);
						embedSubMath(aMath, sub, exp, OPER_POWER, *leftUpperParenthesis, *rightLowerParenthesis, *anchor);
					}
					else
					{
						//std::cerr << "Parentheses" << aMath.y + rightUpperParenthesis->y << "," << aMath.x + rightUpperParenthesis->x << std::endl;
						printDebugMath(sub);
						doMathParsing(sub);
						embedSubMath(aMath, sub, OPER_PARENS, *leftUpperParenthesis, *rightLowerParenthesis,
												 *anchor);
					}
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

void ParserMath::doMathDivision(Math& aMath)
{
	OptPosition leftHorizontalBar = findAny(aMath, HORIZONTAL_BAR);
	if (leftHorizontalBar)
	{
		//std::cerr << "Division" << std::endl;
		OptPosition rightHorizontalBar = findRepeatingRight(aMath, *leftHorizontalBar, HORIZONTAL_BAR);

		Math subNumerator;
		Math subDenomenator;
		OptPosition leftUpperPosition = findUntilUp(aMath, *leftHorizontalBar, HORIZONTAL_BAR);
		if (leftUpperPosition)
		{
			Position rightPos = Position{ rightHorizontalBar->x, rightHorizontalBar->y - 1};
			subNumerator = getSubMath(aMath, *leftUpperPosition, rightPos);
			printDebugMath(subNumerator);
		}

		OptPosition leftLowerPosition = findUntilDown(aMath, *leftHorizontalBar, HORIZONTAL_BAR);
		OptPosition rightLowerPosition = leftLowerPosition;
		if (leftLowerPosition)
		{
			Position leftPos = Position{ leftHorizontalBar->x, leftHorizontalBar->y + 1};
			rightLowerPosition->x = rightHorizontalBar->x;
			subDenomenator = getSubMath(aMath, leftPos, *rightLowerPosition);
			printDebugMath(subDenomenator);
		}
		
		doMathParsing(subNumerator);
		doMathParsing(subDenomenator);

		embedSubMath(aMath, subNumerator, subDenomenator, OPER_DIVIDE, *leftUpperPosition, *rightLowerPosition,
								 *leftHorizontalBar);
		doMathParsing(aMath);
	}
}

void ParserMath::doMathSquareRoot(Math& aMath)
{
	OptPosition squareRoot = findAny(aMath, SQUARE_ROOT);
	if (squareRoot)
	{
		//std::cerr << "Square root" << std::endl;
		OptPosition rootBar = findDiagonalRightUp(aMath, *squareRoot, ROOT_DIAGONAL, ROOT_BAR);
		if (rootBar)
		{
			OptPosition lastRootBar = findRepeatingRight(aMath, *rootBar, ROOT_BAR);
			Position leftPos = Position{ rootBar->x, rootBar->y+1 };
			Position rightPos = Position{ lastRootBar->x, (int)aMath.height() - 1 };
			Math sub = getSubMath(aMath, leftPos, rightPos);
			printDebugMath(sub);
			doMathParsing(sub);
			leftPos = Position{ squareRoot->x, rootBar->y };
			embedSubMath(aMath, sub, OPER_SQUARE_ROOT, leftPos, rightPos, *squareRoot);
			doMathParsing(aMath);
		}
		else
		{
			OptPosition symbol = getSymbol(aMath, *squareRoot);
			if (symbol)
			{
			  Math sub = getSubMath(aMath, squareRoot->right(), *symbol);
				printDebugMath(sub);
				embedSubMath(aMath, sub, OPER_SQUARE_ROOT, *squareRoot, *symbol, *squareRoot);
			  doMathParsing(aMath);
			}
			else
			{
				error(*squareRoot, "need symbol following square root (perhaps use root bar(s)");
			}
		}
	}
}

void ParserMath::doMathPower(Math& aMath)
{
	OptPosition aStartExponent;
	Position aEndExponent;
	Position aStartBase;
	Position aEndBase;
  aStartExponent = findPower(aMath, aEndExponent, aStartBase, aEndBase);

	if (aStartExponent)
	{
		//std::cerr << "Power" << std::endl;
		Math exp = getSubMath(aMath, *aStartExponent, aEndExponent);
		printDebugMath(exp);
		Math base = getSubMath(aMath, aStartBase, aEndBase);
		printDebugMath(base);
		doMathParsing(exp);
		doMathParsing(base);
		Position leftPos{ aStartBase.x, aEndExponent.y };
		Position rightPos{ aEndExponent.x, aStartBase.y };

		if (aStartBase.x == aEndBase.x && aStartBase.y == aEndBase.y &&
				(aMath.matrix[aStartBase.y][aStartBase.x].value == E ||
				 aMath.matrix[aStartBase.y][aStartBase.x].value == 'e'))
		{
			embedSubMath(aMath, exp, OPER_EXP, leftPos, rightPos, aStartBase);
		}
		else
		{
			embedSubMath(aMath, base, exp, OPER_POWER, leftPos, rightPos, aStartBase);
		}
		doMathParsing(aMath);
	}
}

void ParserMath::doMathMultiplication(Math& aMath)
{
	OptPosition multiplication = findAnyOf(aMath, { MULTIPLY, DOT });
	if (multiplication)
	{
		OptPosition symbol = getSymbol(aMath, *multiplication, true);
		if (symbol)
		{
			Math right = getSubMath(aMath, multiplication->right(), *symbol);
			OptPosition leftSymbol = getRightToLeftSymbol(aMath, *multiplication, true);
			if (leftSymbol)
			{
				Math left = getSubMath(aMath, *leftSymbol, multiplication->left());
				printDebugMath(left);
				printDebugMath(right);
				embedSubMath(aMath, left, right, OPER_MULTIPLY, *leftSymbol, *symbol, *leftSymbol);
				doMathParsing(aMath);
			}
			else
			{
				error(*multiplication, "need symbol preceding multiplication");
			}
		}
		else
		{
			error(*multiplication, "need symbol following multiplication");
		}
	}
}

ParserMath::OptPosition ParserMath::getSymbol(
				const Math& aMath, 
				const Position& aLeftPosition,
				bool aAllowSpaces) const
{
	OptPosition result;
	int parens = 0;
	int x = aLeftPosition.x + 1;
	int y = aLeftPosition.y;
	int size = (int)aMath.width();

	if (aAllowSpaces)
	{
		for (; x < size && aMath.matrix[y][x].value == SPACE; ++x)
			;
	}

	for (; x < size && parens >= 0 && !result; ++x)
	{
		if (aMath.matrix[y][x].value == '(')
		{
			++parens;
		}
		else if (aMath.matrix[y][x].value == ')')
		{
			--parens;
		}
		else if (parens == 0)
		{
			if (aMath.matrix[y][x].value == SPACE)
			{
				result = Position{ x-1, y };
			}
			else if (aMath.matrix[y][x].value == SUBMATRIX)
			{
				result = Position{ x, y };
			}
			else
			{
				result = Position{ x, y };
			}
		}
	}

	if (parens != 0)
	{
		error(Position{ x, y }, "unbalanced parentheses");
	}

	return result;
}

ParserMath::OptPosition ParserMath::getRightToLeftSymbol(
				const Math& aMath, 
				const Position& aRightPosition,
				bool aAllowSpaces) const
{
	OptPosition result;
	int parens = 0;
	int x = aRightPosition.x - 1;
	int y = aRightPosition.y;
	
	if (aAllowSpaces)
	{
		for (; x >= 0 && aMath.matrix[y][x].value == SPACE; --x)
			;
	}
	
	for (; x >= 0 && parens >= 0 && !result; --x)
	{
		if (aMath.matrix[y][x].value == ')')
		{
			++parens;
		}
		else if (aMath.matrix[y][x].value == '(')
		{
			--parens;
		}
		else if (parens == 0)
		{
			if (aMath.matrix[y][x].value == SPACE)
			{
				result = Position{ x+1, y };
			}
			else if (aMath.matrix[y][x].value == SUBMATRIX)
			{
				result = Position{ x, y };
			}
			else
			{
				result = Position{ x, y };
			}
		}
	}

	if (parens != 0)
	{
		error(Position{ x, y }, "unbalanced parentheses");
	}
	
	return result;
}

ParserMath::Position ParserMath::getEndExponent(
				const Math& aMath,
				const Position& aStartExponent) const
{
	Position result = aStartExponent;
	if (aMath.matrix[result.y][result.x].value != SUBMATRIX)
	{
		for (;result.x < aMath.width() - 1 && 
					aMath.matrix[result.y+1][result.x+1].value == SPACE; ++result.x)
			;
	}

	return result;
}

ParserMath::OptPosition ParserMath::findPower(
				const Math& aMath,
				Position& aEndExponent,
				Position& aStartBase,
				Position& aEndBase) const
{
	for (int y = 0; y < aMath.height() - 1; ++y)
	{
		for (int x = 1; x < aMath.width(); ++x)
		{
			if (aMath.matrix[y][x].value != SPACE && aMath.matrix[y][x-1].value == SPACE &&
					aMath.matrix[y+1][x].value == SPACE && aMath.matrix[y+1][x-1].value != SPACE)
			{
				Position startExponent = Position{ x, y };
				aEndExponent = getEndExponent(aMath, startExponent);				
				x = aEndExponent.x;
				aEndBase = Position{ startExponent.x, y + 1 };
				OptPosition start = getRightToLeftSymbol(aMath, aEndBase);
				if (start)
				{
					aStartBase = *start;
					return startExponent;
				}
			}
		}
	}

	return std::nullopt;
}

ParserMath::OptPosition ParserMath::findAnyOf(
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

ParserMath::OptPosition ParserMath::findAny(
				const Math& aMath, 
				uint32_t aSearchChar) const
{
	for (int y = 0; y < aMath.height(); ++y)
	{
		for (int x = 0; x < aMath.width(); ++x)
		{
			if (aMath.matrix[y][x].value == aSearchChar)
			{
				return Position{ x, y };
			}
		}
	}

	return std::nullopt;
}

ParserMath::OptPosition ParserMath::findMatchingBigParens(
				const Math& aMath, 
				const Position& aLeftPosition) const
{
	OptPosition result;
	int parens = 1;

	int x = aLeftPosition.x + 1;
	int y = aLeftPosition.y;

	for (; parens > 0 && x < aMath.width(); ++x)
	{
		if (aMath.matrix[y][x].value == LEFT_PARENTHESIS_UPPER_HOOK)
		{
			++parens;
		}
		else if (aMath.matrix[y][x].value == RIGHT_PARENTHESIS_UPPER_HOOK)
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

ParserMath::OptPosition ParserMath::findRepeatingRight(
				const Math& aMath, 
				const Position& aLeftLowerPosition, 
				uint32_t aSearchChar) const
{
	int y = aLeftLowerPosition.y;

	for (int x = aLeftLowerPosition.x; x < aMath.width(); ++x)
	{
		if (aMath.matrix[y][x].value != aSearchChar)
		{
			return Position{ x - 1, y };
		}
	}

	return std::nullopt;
}

ParserMath::OptPosition ParserMath::findMatchingDown(
				const Math& aMath, 
				const Position& aUpperPosition,
				uint32_t aInbetweenChar,
				uint32_t aSearchChar) const
{
	int x = aUpperPosition.x;

	for (int y = aUpperPosition.y + 1; y < aMath.height(); ++y)
	{
		uint32_t kar = aMath.matrix[y][x].value;
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
	
ParserMath::OptPosition ParserMath::findSomethingLeft(
				const Math& aMath, 
				const Position& aLeftUpperPosition,
				const Position& aLeftLowerPosition) const
{
	for (int x = aLeftUpperPosition.x - 1; x > 0; --x)
	{
		for (int y = aLeftUpperPosition.y; y < aLeftLowerPosition.y; ++y)
		{
			if (aMath.matrix[y][x].value != SPACE)
			{
				return Position{ x, y };
			}
		}
	}

	return std::nullopt;
}

ParserMath::OptPosition ParserMath::findSomethingRight(
				const Math& aMath, 
				const Position& aRightUpperPosition,
				const Position& aRightLowerPosition) const
{
	for (int x = aRightUpperPosition.x + 1; x < aMath.width(); ++x)
	{
		for (int y = aRightUpperPosition.y; y < aRightLowerPosition.y; ++y)
		{
			if (aMath.matrix[y][x].value != SPACE)
			{
				return Position{ x, y };
			}
		}
	}

	return std::nullopt;
}

ParserMath::OptPosition ParserMath::findDiagonalRightUp(
				const Math& aMath, 
				const Position& aLeftLowerPosition,
				uint32_t aInbetweenChar,
				uint32_t aSearchChar) const
{
	int x = aLeftLowerPosition.x + 1;

	for (int y = aLeftLowerPosition.y - 1; y > 0 && x < aMath.width(); --y, ++x)
	{
		uint32_t kar = aMath.matrix[y][x].value;
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

ParserMath::OptPosition ParserMath::findUntilUp(
				const Math& aMath, 
				const Position& aLowerPosition,
				uint32_t aSearchChar) const
{
	OptPosition result;

	int x = aLowerPosition.x;

	for (int y = aLowerPosition.y - 1; !result && y >= 0; --y)
	{
		uint32_t kar = aMath.matrix[y][x].value;
		if (kar == aSearchChar)
		{
			if (y < aLowerPosition.y - 1)
			{
				result = Position{ x, y + 1 };
			}
			else
			{
				error(mathPos(aMath, aLowerPosition), "expected some expression above division bar");
				break;
			}
		}
	}

	if (!result)
	{
		if (aLowerPosition.y > 0)
		{
			result = Position{ x, 0 };
		}
		else
		{
			error(mathPos(aMath, aLowerPosition), "expected some expression above division bar");
		}
	}

	return result;
}

ParserMath::OptPosition ParserMath::findUntilDown(
				const Math& aMath, 
				const Position& aUpperPosition,
				uint32_t aSearchChar) const
{
	OptPosition result;

	int x = aUpperPosition.x;

	for (int y = aUpperPosition.y + 1; !result && y < aMath.height(); ++y)
	{
		uint32_t kar = aMath.matrix[y][x].value;
		if (kar == aSearchChar)
		{
			if (y > aUpperPosition.y + 1)
			{
				result = Position{ x, y - 1 };
			}
			else
			{
				error(mathPos(aMath, aUpperPosition), "expected some expression below division bar");
				break;
			}
		}
	}

	
	if (!result)
	{
		if (aUpperPosition.y < aMath.height() - 1)
		{
			result = Position{ x, aMath.height() - 1 };
		}
		else
		{
			error(mathPos(aMath, aUpperPosition), "expected some expression below division bar");
		}
	}

	return result;
}

ParserMath::Position ParserMath::mathPos(const Math& aMath, const Position& aPosition) const
{
	return Position{ (int)aMath.y + aPosition.y, (int)aMath.x + aPosition.x };
}

void ParserMath::mathError(
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

void ParserMath::error(const Position& aPosition, const std::string& aError) const
{
	std::cerr << "(" << aPosition.y << "," << aPosition.x << "): " << aError << std::endl;
}