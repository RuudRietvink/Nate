#include "MathParser.h"
#include "utf8.h"

#include <iostream>
#include <sstream>
#define NOMINMAX  
#include <windows.h>
#include <algorithm>
#include <cctype>

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

	const std::string OPER_LEFT_TO_RIGHT        = "left_to_right";
	const std::string OPER_PARENS               = "()";
	const std::string OPER_DIVIDE               = "/";
	const std::string OPER_SQUARE_ROOT          = "sqrt";
	const std::string OPER_POWER                = "pow";
	const std::string OPER_EXP                  = "exp";
	const std::string OPER_MULTIPLY             = "*";
}

void MathParser::setTabSize(uint32_t aTabSize)
{
	mTabSize = aTabSize;
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
	fillUpMath(aMath);
	printDebugMath(aMath);
	doStartMathParsing(aMath);
	printDebugMath(aMath);
	return mathString(aMath);
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
	
void MathParser::printDebugMath(const Math& aMath) const
{
	//printMath(aMath);
}

void MathParser::printMath(const Math& aMath) const
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

void MathParser::printDebugMath(
				const Math& aMath,
				const Position& aLeftPosition,
				const Position& aRightPosition) const
{
	printMath(getSubMath(aMath, aLeftPosition, aRightPosition));
}

std::string MathParser::operParens(const Math& aMath) const
{
	std::stringstream ss;
	ss << "(" << mathString(aMath) << ")";
	return ss.str();
}

std::string MathParser::operDivide(const Math& aMathLeft, const Math& aMathRight) const
{
	std::stringstream ss;
	ss << "((" << mathString(aMathLeft) << ") / (" 
						 << mathString(aMathRight) << "))";
	return ss.str();
}

std::string MathParser::operSquareRoot(const Math& aMath) const
{
	std::stringstream ss;
	ss << "sqrt(" << mathString(aMath) << ")";
	return ss.str();
}

std::string MathParser::operPower(const Math& aMathBase, const Math& aMathExp) const
{
	std::stringstream ss;
	ss << "pow(" << mathString(aMathBase) << ", " 
						   << mathString(aMathExp) << ")";
	return ss.str();
}

std::string MathParser::operExp(const Math& aMathExp) const
{
	std::stringstream ss;
	ss << "exp(" << mathString(aMathExp) << ")";
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

std::string MathParser::mathString(const Math& aMath) const
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
			}
			else if (col.value != SPACE)
			{
				utf8::append(col.value, std::ostream_iterator<char>(ss));
			}
		}
	}

	return ss.str();
}

MathParser::Math MathParser::getSubMath(
				const Math& aMath, 
			  const Position& aLeftUpperPosition, 
			  const Position& aRightLowerPosition) const
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

void MathParser::embedSubMath(
				Math& aMath, 
				const Math& aSubMath, 
				const std::string& aOper,
				const Position& aLeftUpperPosition,
				const Position& aRightLowerPosition,
				const Position& aPlacePosition)
{
	embedSubMath(aMath, aSubMath, Math(), aOper, aLeftUpperPosition, aRightLowerPosition, aPlacePosition);
}

void MathParser::embedSubMath(
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

void MathParser::clearMath(
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

void MathParser::doStartMathParsing(Math& aMath)
{
	doMathSimpleOperators(aMath);
	doMathParsing(aMath);
}

void MathParser::doMathParsing(Math& aMath)
{
	doMathParentheses(aMath);
	doMathFractionBar(aMath);
	doMathSquareRoot(aMath);
	doMathPower(aMath);
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

void MathParser::doMathFractionBar(Math& aMath)
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

void MathParser::doMathSquareRoot(Math& aMath)
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

void MathParser::doMathPower(Math& aMath)
{
	Position aEndExponent;
	Position aStartBase;
	Position aEndBase;
  auto [isSuperscript, aStartExponent] = findPower(aMath, aEndExponent, aStartBase, aEndBase);

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
		Position place = isSuperscript ? aStartBase.up() : aStartBase;

		if (aStartBase.x == aEndBase.x && aStartBase.y == aEndBase.y &&
				(aMath.matrix[aStartBase.y][aStartBase.x].value == E ||
				 aMath.matrix[aStartBase.y][aStartBase.x].value == 'e'))
		{
			embedSubMath(aMath, exp, OPER_EXP, leftPos, rightPos, place);
		}
		else
		{
			embedSubMath(aMath, base, exp, OPER_POWER, leftPos, rightPos, place);
		}

		doMathParsing(aMath);
	}
}

void MathParser::doMathSimpleOperators(Math& aMath)
{
	for (int y = 0; y < aMath.height(); ++y)
	{
		for (int x = 0; x < aMath.width(); ++x)
		{
			uint32_t kar = aMath.matrix[y][x].value;
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

			if (kar != aMath.matrix[y][x].value)
			{
				aMath.matrix[y][x].value = kar;
			}
		}
	}
}

MathParser::OptPosition MathParser::getSymbol(
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
		uint32_t kar = aMath.matrix[y][x].value;
		if (kar == '(')
		{
			++parens;
		}
		else if (kar == ')')
		{
			--parens;
		}
		else if (parens == 0)
		{
			if (kar == SPACE)
			{
				result = Position{ x-1, y };
			}
			else if (kar == SUBMATRIX)
			{
				result = Position{ x, y };
			}
			else
			{
				if ((kar == '-' || kar == '+') && x < size -1 && aMath.matrix[y][x].value == SUBMATRIX)
				{
					result = Position{ x + 1, y };
				}
				else if (kar == '-' || kar == '+' || kar == '.' || isdigit(kar))
				{
					while (x < size && 
								 (isdigit(aMath.matrix[y][x].value) || 
									aMath.matrix[y][x].value == '.' || 
									aMath.matrix[y][x].value == '-' || 
									aMath.matrix[y][x].value == '+' || 
									aMath.matrix[y][x].value == 'E' || 
									aMath.matrix[y][x].value == 'e'))
					{
						++x;
					}

					--x;
					result = Position{ x, y };
				}
				else
				{
					result = Position{ x, y };
				}
			}
		}
	}

	if (parens != 0)
	{
		error(Position{ x, y }, "unbalanced parentheses");
	}

	return result;
}

	std::tuple<bool, MathParser::OptPosition> MathParser::getRightToLeftSymbol(
				const Math& aMath, 
				const Position& aRightPosition,
				bool aAllowSpaces) const
{
	OptPosition result;
	int x = aRightPosition.x - 1;
	int y = aRightPosition.y;
	
	if (aAllowSpaces)
	{
		for (; x >= 0 && aMath.matrix[y][x].value == SPACE; --x)
			;
	}
	
	bool isSuperScript = (x >= 0 && getSuperscript(aMath.matrix[y][x].value) != BADCHAR);
	std::vector<uint32_t> prevParens;

	for (; x >= 0 && !result; --x)
	{
		uint32_t kar = optSuperscript(isSuperScript, aMath.matrix[y][x].value);

		if (kar == ')')
		{
			prevParens.push_back(kar);
		}
		else if (kar == '(')
		{
			if (prevParens.empty())
			{
				error(Position{ x, y }, "unbalanced parentheses");
				break;
			}
			if ((kar == '(') && prevParens.back() != ')')
			{
				error(Position{ x, y }, "unbalanced parentheses");
				break;
			}
			prevParens.pop_back();
		}
		else if (prevParens.empty())
		{
			if (kar == SPACE || kar == BADCHAR)
			{
				result = Position{ x+1, y };
			}
			else if (kar == SUBMATRIX)
			{
				result = Position{ x, y };
			}
			else if (kar == '.' || isdigit(kar))
			{
				--x;
				while (x > 0 && (kar = optSuperscript(isSuperScript, aMath.matrix[y][x].value)) &&
								(isdigit(kar) || 
								 kar == '.' || 
								 kar == '-' || 
								 kar == '+' || 
								 kar == 'E' || 
								 kar == 'e'))
				{
					--x;					
				}

				++x;
				kar = optSuperscript(isSuperScript, aMath.matrix[y][x].value);
				if (kar == '-' || // skip unary operator
						kar == '+')
				{
					++x;
				}

				result = Position{ x, y };
			}
			else
			{
				result = Position{ x, y };
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

MathParser::Position MathParser::getEndExponent(
				const Math& aMath,
				const Position& aStartExponent) const
{
	Position result = aStartExponent;
	if (aMath.matrix[result.y][result.x].value != SUBMATRIX)
	{
		for (;result.x < aMath.width() - 1 && 
					aMath.matrix[result.y+1][result.x].value == SPACE; ++result.x)
			;
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
	for (int x = aLeftPosition.x; x < aRightPosition.x; ++x)
	{
		aMath.matrix[aLeftPosition.y][x].value = getSuperscript(aMath.matrix[aLeftPosition.y][x].value);
	}
}

std::tuple<bool, MathParser::OptPosition> MathParser::findPower(
				Math& aMath,
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
				auto [isSuperscript, start] = getRightToLeftSymbol(aMath, aEndBase);
				if (start)
				{
					aStartBase = *start;
					if (isSuperscript)
					{
						unsuperscript(aMath, aStartBase, aEndBase);
					}

					return std::make_tuple(isSuperscript, startExponent);
				}
			}
		}
	}
	
	for (int y = 0; y < aMath.height(); ++y)
	{
		for (int x = 1; x < aMath.width(); ++x)
		{
			if (getSuperscript(aMath.matrix[y][x].value) != BADCHAR)
			{
				auto [isSuperscript, start] = getRightToLeftSymbol(aMath, Position{ x, y });
				if (start)
				{
					aEndBase = Position{ x-1, y };
					aStartBase = *start;
					uint32_t subs;
					Position startExponent = Position{ x, y };
					int startX = x;
					while (x < aMath.width())
					{
						if (x > startX && aMath.matrix[y][x].value == SPACE)
						{
							++x;
						}
						else if ((subs = getSuperscript(aMath.matrix[y][x].value)) != BADCHAR)
						{
							aMath.matrix[y][x].value = subs;
							++x;
						}
						else
						{
							break;
						}
					}
					
					--x;
					aEndExponent = Position{ x, y };
					return std::make_tuple(false, startExponent);
				}
			}
		}
	}
	
	return std::make_tuple(false, std::nullopt);
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
			if (aMath.matrix[y][x].value == aSearchChar)
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

MathParser::OptPosition MathParser::findRepeatingRight(
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

MathParser::OptPosition MathParser::findMatchingDown(
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
	
MathParser::OptPosition MathParser::findSomethingLeft(
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

MathParser::OptPosition MathParser::findSomethingRight(
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

MathParser::OptPosition MathParser::findDiagonalRightUp(
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

MathParser::OptPosition MathParser::findUntilUp(
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

MathParser::OptPosition MathParser::findUntilDown(
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

void MathParser::error(const Position& aPosition, const std::string& aError) const
{
	std::cerr << "(" << aPosition.y << "," << aPosition.x << "): " << aError << std::endl;
}