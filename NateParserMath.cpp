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
}

void NateParser::doMath(Math& aMath)
{
	fillUpMath(aMath);
	doMathEvaluation(aMath);
	printMath(aMath);
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
	
void NateParser::printMath(Math& aMath) const
{
	std::cerr <<"--------------------------" << std::endl;
	for (auto& row : aMath.matrix)
	{
		for (auto& col : row)
		{
			utf8::append(std::get<uint32_t>(col), std::ostream_iterator<char>(std::cerr));
		}

		std::cerr << std::endl;
	}
		
	std::cerr <<"--------------------------" << std::endl;
}

yy::position NateParser::mathPos(const Math& aMath, const Position& aPosition) const
{
	return yy::position(nullptr, (unsigned int)(aMath.y + aPosition.y),
											         (unsigned int)(aMath.x + aPosition.x));
}

void NateParser::doMathEvaluation(Math& aMath)
{
	doMathParentheses(aMath);
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
					Math sub = getSubMath(aMath, *leftUpperParenthesis, *rightLowerParenthesis);
					printMath(sub);
					doMathEvaluation(sub);
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

NateParser::OptPosition NateParser::findAny(const Math& aMath, uint32_t aSearchChar) const
{
	OptPosition result = std::nullopt;

	for (size_t y = 0; result == std::nullopt && y < aMath.matrix.size(); ++y)
	{
		for (size_t x = 0; result == std::nullopt && x < aMath.matrix[y].size(); ++x)
		{
			if (std::get<uint32_t>(aMath.matrix[y][x]) == aSearchChar)
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

	size_t y = aLeftPosition.y;

	for (size_t x = aMath.matrix[y].size() - 1; result == std::nullopt && x > aLeftPosition.x; --x)
	{
		if (std::get<uint32_t>(aMath.matrix[y][x]) == aSearchChar)
		{
			result = Position{ x, y };
		}
	}

	return result;
}

NateParser::OptPosition NateParser::findMatchingDown(
				const Math& aMath, 
				const Position& aTopPosition,
				uint32_t aInbetweenChar,
				uint32_t aSearchChar) const
{
	OptPosition result = std::nullopt;

	size_t x = aTopPosition.x;

	for (size_t y = aTopPosition.y + 1; result == std::nullopt && y < aMath.matrix.size(); ++y)
	{
		uint32_t kar = std::get<uint32_t>(aMath.matrix[y][x]);
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
	
void NateParser::mathError(
				const Math& aMath, 
				const Position& aPosition,
				uint32_t aBadChar,
				uint32_t aInbetweenChar,
				uint32_t aSearchChar) const
{
	std::string ss;

	ss = "bad character found '";
	utf8::append(aBadChar, std::back_inserter(ss));
	ss += "', expected '";

	if (aInbetweenChar != 0)
	{
		utf8::append(aInbetweenChar, std::back_inserter(ss));
		ss += "', or '";
	}

	utf8::append(aSearchChar, std::back_inserter(ss));
	ss += "'";

	error(mathPos(aMath, aPosition), ss);
}

NateParser::Math NateParser::getSubMath(
				const Math& aMath, 
			  const Position& aLeftUpperPosition, 
			  const Position& aRightLowerPosition)
{
	Math result;
	result.x = aMath.x + aLeftUpperPosition.x;
	result.y = aMath.y + aLeftUpperPosition.y;

	for (size_t y = aLeftUpperPosition.y; y <= aRightLowerPosition.y; ++y)
	{
		result.matrix.push_back(MathVector());

		for (size_t x = aLeftUpperPosition.x + 1; x < aRightLowerPosition.x; ++x)
		{
			result.matrix.back().push_back(aMath.matrix[y][x]);
		}
	}

	fillUpMath(result);
	return result;
}