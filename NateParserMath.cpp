#include "NateParser.h"
#include "core/Core.h"
#include "lex.yy.h"

extern int gDebug;

namespace {
	const uint32_t SPACE                        = 0x20;  
	const uint32_t LEFT_PARENTHESIS_UPPER_HOOK  = 0x239B; // ⎛
	const uint32_t LEFT_PARENTHESIS_EXTENSION   = 0x239C; // ⎜
	const uint32_t LEFT_PARENTHESIS_LOWER_HOOK  = 0x239D; // ⎝
	const uint32_t RIGHT_PARENTHESIS_UPPER_HOOK = 0x239E; // ⎞
	const uint32_t RIGHT_PARENTHESIS_EXTENSION  = 0x239F; // ⎟
	const uint32_t RIGHT_PARENTHESIS_LOWER_HOOK = 0x23A0; // ⎠
	const uint32_t HORIZONTAL_BAR               = 0x2015; // ―
	const uint32_t ROOT_BAR                     = 0x5F;   // _
	const std::string OPER_LEFT_TO_RIGHT        = "left_to_right";
	const std::string OPER_PARENS               = "()";
	const std::string OPER_DIVIDE               = "/";
}

void NateParser::doMath(Math& aMath)
{
	fillUpMath(aMath);
	printMath(aMath);
	doMathParsing(aMath);
	printMath(aMath);
	std::cerr << mathString(aMath) << std::endl;
}

void NateParser::fillUpMath(Math& aMath)
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
	
void NateParser::printMath(const Math& aMath) const
{
	std::cerr <<"--------------------------" << std::endl;
	for (auto& row : aMath.matrix)
	{
		for (auto& col : row)
		{
			if (col.value != 0)
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

std::string NateParser::mathString(const Math& aMath) const
{
	std::stringstream ss;
	
	for (auto& row : aMath.matrix)
	{
		for (auto& col : row)
		{
			if (col.value == 0)
			{
				if (col.oper == OPER_LEFT_TO_RIGHT)
				{
					ss << mathString(col.embedded1) << mathString(col.embedded2);
				}
				else if (col.oper == OPER_PARENS)
				{
					ss << "(" << mathString(col.embedded1) << ")";
				}
				else if (col.oper == OPER_DIVIDE)
				{
					ss << "((" << mathString(col.embedded1) << ") / (" 
						         << mathString(col.embedded2) << "))";
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

NateParser::Math NateParser::getSubMath(
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

void NateParser::embedSubMath(
				Math& aMath, 
				const Math& aSubMath1, 
				const Math& aSubMath2, 
				const std::string& aOper,
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
	
	aMath.matrix[aLeftUpperPosition.y][aLeftUpperPosition.x] = MathValue(aSubMath1, aSubMath2, aOper);
}

void NateParser::doMathParsing(Math& aMath)
{
	doMathParentheses(aMath);
	doMathDivision(aMath);
}

void NateParser::doMathParentheses(Math& aMath)
{
	OptPosition leftUpperParenthesis = findAny(aMath, LEFT_PARENTHESIS_UPPER_HOOK);
	if (leftUpperParenthesis)
	{
		OptPosition rightUpperParenthesis = findMatchingRight(aMath, *leftUpperParenthesis, 
																													RIGHT_PARENTHESIS_UPPER_HOOK);
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
					std::cerr << "OK" << std::endl;
					Position leftPos = Position{ leftUpperParenthesis->x+1, leftUpperParenthesis->y };
					Position rightPos = Position{ rightLowerParenthesis->x - 1, rightLowerParenthesis->y };
					Math sub = getSubMath(aMath, leftPos, rightPos);
					printMath(sub);
					doMathParsing(sub);

					Math left;
					if (leftUpperParenthesis->x > 1)
					{
						leftPos = Position{ 1, leftUpperParenthesis->y };
						rightPos = Position{ leftLowerParenthesis->x - 1, leftUpperParenthesis->y };
						left = getSubMath(aMath, leftPos, rightPos);
					  embedSubMath(aMath, left, Math(), OPER_LEFT_TO_RIGHT, leftPos, rightPos);
					}

					embedSubMath(aMath, sub, Math(), OPER_PARENS, *leftUpperParenthesis, *rightLowerParenthesis);
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

void NateParser::doMathDivision(Math& aMath)
{
	OptPosition leftHorizontalBar = findAny(aMath, HORIZONTAL_BAR);
	if (leftHorizontalBar)
	{
		OptPosition rightHorizontalBar = findRepeatingRight(aMath, *leftHorizontalBar, HORIZONTAL_BAR);

		Math subNumerator;
		Math subDenomenator;
		OptPosition leftUpperPosition = findUntilUp(aMath, *leftHorizontalBar, ROOT_BAR);
		if (leftUpperPosition)
		{
			Position rightPos = Position{ rightHorizontalBar->x, rightHorizontalBar->y - 1};
			subNumerator = getSubMath(aMath, *leftUpperPosition, rightPos);
			printMath(subNumerator);
			doMathParsing(subNumerator);
		}

		OptPosition leftLowerPosition = findUntilDown(aMath, *leftHorizontalBar, HORIZONTAL_BAR);
		OptPosition rightLowerPosition = leftLowerPosition;
		if (leftLowerPosition)
		{
			Position leftPos = Position{ leftHorizontalBar->x, leftHorizontalBar->y + 1};
			rightLowerPosition->x = rightHorizontalBar->x;
			subDenomenator = getSubMath(aMath, leftPos, *rightLowerPosition);
			printMath(subDenomenator);
			doMathParsing(subDenomenator);
		}

		embedSubMath(aMath, subNumerator, subDenomenator, OPER_DIVIDE, *leftUpperPosition, *rightLowerPosition);
		doMathParsing(aMath);
	}
}

NateParser::OptPosition NateParser::findAny(const Math& aMath, uint32_t aSearchChar) const
{
	OptPosition result = std::nullopt;

	for (int y = 0; result == std::nullopt && y < (int)aMath.matrix.size(); ++y)
	{
		for (int x = 0; result == std::nullopt && x < (int)aMath.matrix[y].size(); ++x)
		{
			if (aMath.matrix[y][x].value == aSearchChar)
			{
				result = Position{ x, y };
			}
		}
	}

	return result;
}

NateParser::OptPosition NateParser::findMatchingRight(
				const Math& aMath, 
				const Position& aLeftPosition, 
				uint32_t aSearchChar) const
{
	OptPosition result = std::nullopt;

	int y = aLeftPosition.y;

	for (int x = (int)aMath.matrix[y].size() - 1; result == std::nullopt && x > aLeftPosition.x; --x)
	{
		if (aMath.matrix[y][x].value == aSearchChar)
		{
			result = Position{ x, y };
		}
	}

	return result;
}

NateParser::OptPosition NateParser::findRepeatingRight(
				const Math& aMath, 
				const Position& aLeftPosition, 
				uint32_t aSearchChar) const
{
	OptPosition result = std::nullopt;

	int y = aLeftPosition.y;

	for (int x = aLeftPosition.x; result == std::nullopt && x < (int)aMath.matrix[y].size(); ++x)
	{
		if (aMath.matrix[y][x].value != aSearchChar)
		{
			result = Position{ x - 1, y };
		}
	}

	return result;
}

NateParser::OptPosition NateParser::findMatchingDown(
				const Math& aMath, 
				const Position& aUpperPosition,
				uint32_t aInbetweenChar,
				uint32_t aSearchChar) const
{
	OptPosition result = std::nullopt;

	int x = aUpperPosition.x;

	for (int y = aUpperPosition.y + 1; result == std::nullopt && y < (int)aMath.matrix.size(); ++y)
	{
		uint32_t kar = aMath.matrix[y][x].value;
		if (aInbetweenChar != 0 && kar == aInbetweenChar)
		{
			// skip
		}
		else if (kar == aSearchChar)
		{
			result = Position{ x, y };
		}
		else
		{
			mathError(aMath, Position{ x, y }, kar, aInbetweenChar, aSearchChar);
			break;
		}
	}

	return result;
}
	
NateParser::OptPosition NateParser::findUntilUp(
				const Math& aMath, 
				const Position& aLowerPosition,
				uint32_t aSearchChar) const
{
	OptPosition result = std::nullopt;

	int x = aLowerPosition.x;

	for (int y = aLowerPosition.y - 1; result == std::nullopt && y >= 0; --y)
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

NateParser::OptPosition NateParser::findUntilDown(
				const Math& aMath, 
				const Position& aUpperPosition,
				uint32_t aSearchChar) const
{
	OptPosition result = std::nullopt;

	int x = aUpperPosition.x;

	for (int y = aUpperPosition.y + 1; result == std::nullopt && y < (int)aMath.matrix.size(); ++y)
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
		if (aUpperPosition.y < (int)aMath.matrix.size() - 1)
		{
			result = Position{ x, (int)aMath.matrix.size() - 1 };
		}
		else
		{
			error(mathPos(aMath, aUpperPosition), "expected some expression below division bar");
		}
	}

	return result;
}

yy::position NateParser::mathPos(const Math& aMath, const Position& aPosition) const
{
	return yy::position(nullptr, (unsigned int)(aMath.y + aPosition.y),
											         (unsigned int)(aMath.x + aPosition.x));
}

void NateParser::mathError(
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
