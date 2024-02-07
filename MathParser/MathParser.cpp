#include "MathParser.h"
#include "utf8.h"

#include <iostream>
#include <sstream>
#define NOMINMAX  
#include <windows.h>
#include <algorithm>
#include <cctype>
#include <clocale>
#include <format>

namespace nate 
{
//   _
// √╱  ― ⌊⌋⌈⌉|⏐⎛⎞⎝⎠⎜⎟
// ⏨ю
// ⁰¹²³⁴⁵⁶⁷⁸⁹ᴬᴮᴰᴱᴳᴴᴵᴶᴷᴸᴹᴺᴼᴾᴿᵀᵁⱽᵂᵃᵇᶜᵈᵉᶠᵍʰⁱʲᵏˡᵐⁿᵒᵖʳˢᵗᵘᵛʷˣʸᶻ⁻⁺⁽⁾
// ₀₁₂₃₄₅₆₇₈₉ ₊₋₍₎ₐₑₒₓ
// ⅒⅑⅛⅐⅙⅕¼⅓½⅖⅔⅜⅗¾⅘⅝⅚⅞
// πτ𝑖𝑒⋅÷⁄×⇑⇓←

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
const uint32_t PI																	= 0x03C0;// π
const uint32_t MULTIPLY_X													= 0x00D7; // ×
const uint32_t MULTIPLY_STAR											= 0x2217; // ∗
const uint32_t MULTIPLY_DOT												= 0x22C5; // ⋅
const uint32_t DIVIDE_SLASH												= 0x002F; // /
const uint32_t DIVIDE_SIGN												= 0x00F7; // ÷
const uint32_t SUPER_OPEN													= 0x207D; // ⁽
const uint32_t SUPER_CLOSE												= 0x207E; // ⁾
const uint32_t ASSIGNMENT 												= 0x2190; // ←
		
////////////////////////// public  /////////////////////////////////

void MathParser::setTabSize(uint32_t aTabSize)
{
	mTabSize = aTabSize;
}

void MathParser::addVariable(const std::string& aName, const std::string& aCodeName, NumberType aType)
{
	addSymbol({ Symbol::Type::Variable, aName, aName, aCodeName, aType });
}

void MathParser::addConstant(const std::string& aName, const std::string& aIntName, const std::string& aCodeName, NumberType aType)
{
	addSymbol({ Symbol::Type::Constant, aName, aIntName, aCodeName, aType });
}

void MathParser::addFunction(const std::string& aName, const std::string& aCodeName, NumberType aType)
{
	addSymbol({ Symbol::Type::Function, aName, aName, aCodeName, aType });
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
	init();

	fillUpMath(aMath);
	printDebugMath(false, aMath, "");
	doStartMathParsing(aMath);
	printDebugMath(false, aMath, "");
	return mathString(aMath);
}

////////////////////////// protected /////////////////////////////////

void MathParser::init()
{
}

void MathParser::printDebugMath(bool print, const Math& aMath, const std::string& aFunction, const std::string& aText) const
{
	//if (print)
	{
		printMath(aMath, aFunction, aText);
	}
}

void MathParser::error(const InputPosition& aPosition, const std::string& aError) const
{
	std::cerr << "(y:" << aPosition.y << ", x:" << aPosition.x << "): " << aError << std::endl;
}

std::string MathParser::codeFunction(const std::string& aName, const Math& aMathArg) const
{
	std::stringstream ss;
			
	ss << aName << "(" << mathString(aMathArg) << ")";

	return ss.str();

}


bool MathParser::isSymbol(const std::string& aInput) const
{
	return mSymbols.find(aInput) != mSymbols.end();
}

const Symbol& MathParser::getSymbol(const std::string& aInput) const
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

std::string MathParser::operToString(Oper aOper)
{
	switch (aOper)
	{
  case Oper::Unknown: return "Unknown";
  case Oper::Nested: return "Nested";
  case Oper::Parentheses: return "Parentheses";
  case Oper::Brackets: return "Brackets";
  case Oper::Matrix: return "Matrix";
  case Oper::Absolute: return "Absolute";
  case Oper::Floor: return "Floor";
  case Oper::Ceiling: return "Ceiling";
  case Oper::SquareRoot: return "SquareRoot";
  case Oper::Power: return "Power";
  case Oper::Exponential: return "Exponential";
  case Oper::Multiplication: return "Multiplication";
  case Oper::Division: return "Division";
  case Oper::Addition: return "Addition";
  case Oper::Subtraction: return "Subtraction";
  case Oper::Symbol: return "Symbol";
  case Oper::FunctionName: return "FunctionName";
  case Oper::FunctionCall: return "FunctionCall";
  case Oper::Constant: return "Constant";
  case Oper::Number: return "Number";
  case Oper::Monomial: return "Monomial";
  case Oper::UnaryMinus: return "UnaryMinus";
  case Oper::UnaryPlus: return "UnaryPlus";
  case Oper::Assignment: return "Assignment";
	}

	return "<Unknown operator>";
}

std::string MathParser::u2s(uint32_t aChar)
{
	std::string result;
	auto back = std::back_insert_iterator<std::string>(result);
	utf8::append(aChar, back);
	return result;
}

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
	
void MathParser::printMath(const Math& aMath, const std::string& aFunction, const std::string& aText) const
{
	if (!aFunction.empty())
	{
		std::cerr <<"--------------------------" << std::endl;
		std::cerr << aFunction << " " << aText << std::endl;
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

	std::cerr << mathString(aMath) << std::endl;
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
				ss << codeOperator(col);
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

Math MathParser::createSubMath(
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

Math MathParser::getSubMath(
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

MathValue* MathParser::embedSubMath(
				Math& aMath, 
				const Math& aSubMath, 
				Oper aOper,
				const Area& aArea,
				const Symbol& aSymbol)
{
	return embedSubMath(aMath, aSubMath, Math(), aOper, aArea, aSymbol);
}

MathValue* MathParser::embedSubMath(
				Math& aMath, 
				const Math& aSubMath1, 
				const Math& aSubMath2, 
				Oper aOper,
				const Area& aArea,
				const Symbol& aSymbol)
{
	Size size{ aArea.lowerRight.x + 1 - aArea.upperLeft.x, aArea.lowerRight.y + 1 - aArea.upperLeft.y };
	auto mathValue = MathValue(aSubMath1, aSubMath2, aOper, size, aSymbol);
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
	doMathRenameSimpleOperators(aMath);
	doMathVariablesNumbers(aMath);
	doMathParsing(aMath);
}

void MathParser::doMathParsing(Math& aMath)
{
	doMathParentheses(aMath);
	doMathBrackets(aMath);
	doMathSimpleMatching(aMath, '(',          ')',           Oper::Parentheses, "parentheses");
	doMathSimpleMatching(aMath, '[',          ']',           Oper::Brackets,    "brackets", true);
	doMathSimpleMatching(aMath, '|',          '|',           Oper::Absolute,    "vertical bars");
	doMathSimpleMatching(aMath, LEFT_CEILING, RIGHT_CEILING, Oper::Floor,       "floor delimiters");
	doMathSimpleMatching(aMath, LEFT_FLOOR,   RIGHT_FLOOR,   Oper::Ceiling,     "ceiling delimiters");
	doMathSquareRoot(aMath);
	doMathFractionBar(aMath);
	doMathPower(aMath);
	doMathMonomial(aMath);
	doMathUnaryLeadingOperator(aMath, '-', Oper::UnaryMinus);
	doMathUnaryLeadingOperator(aMath, '+', Oper::UnaryPlus);
	doMathFunctionCall(aMath);
	doMathUnaryLeadingOperator(aMath, '-', Oper::UnaryMinus);
	doMathUnaryLeadingOperator(aMath, '+', Oper::UnaryPlus);
	doMathDownRightOperator(aMath, '*', Oper::Multiplication);
	doMathDownRightOperator(aMath, '/', Oper::Division);
	doMathDownRightOperator(aMath, '+', Oper::Addition);
	doMathDownRightOperator(aMath, '-', Oper::Subtraction);
	doMathUpLeftOperator(aMath, ASSIGNMENT, Oper::Assignment);
}

OptArea MathParser::findBigBlock(Math& aMath, const std::vector<uint32_t>& aLeftCodes,
																const std::vector<uint32_t>& aRightCodes, const char* aDesc)
{
	OptPosition leftUpper = findAny(aMath, aLeftCodes[0]);
	if (leftUpper)
	{
		OptPosition rightUpper = findMatchingBig(aMath, *leftUpper, aLeftCodes[0], aRightCodes[0], aDesc);
		if (rightUpper)
		{
			OptPosition leftLower = findMatchingDown(aMath, *leftUpper, aLeftCodes[1], aLeftCodes[2]);
			if (leftLower)
			{
				OptPosition rightLower = findMatchingDown(aMath, *rightUpper, aRightCodes[1], aRightCodes[2]);
				if (rightLower)
				{
					Position leftPos = Position{ leftUpper->x, leftUpper->y };
					Position rightPos = Position{ rightLower->x, rightLower->y };
					return Area{ leftPos, rightPos };
				}
				else
				{
					error(mathPos(aMath, *rightUpper), std::format("missing matching {} or {} below {}", 
																												 u2s(aRightCodes[2]), u2s(aRightCodes[1]), u2s(aRightCodes[0])));
				}
			}
			else
			{
				error(mathPos(aMath, *leftUpper), std::format("missing matching {} or {} below {}", 
																											u2s(aLeftCodes[2]), u2s(aLeftCodes[1]), u2s(aLeftCodes[0])));
			}
		}
		else
		{
			error(mathPos(aMath, *leftUpper), std::format("missing matching {} at the right of a {}", 
																											u2s(aRightCodes[0]), u2s(aLeftCodes[0])));
		}
	}

	return std::nullopt;
}


void MathParser::doMathParentheses(Math& aMath)
{
	OptArea area = findBigBlock(aMath, { LEFT_PARENTHESIS_UPPER_HOOK, LEFT_PARENTHESIS_EXTENSION, LEFT_PARENTHESIS_LOWER_HOOK},
															{ RIGHT_PARENTHESIS_UPPER_HOOK, RIGHT_PARENTHESIS_EXTENSION, RIGHT_PARENTHESIS_LOWER_HOOK},
															"parentheses");

	if (area)
	{
		Area subArea{ area->upperLeft.right(), area->lowerRight.left() };
		Math sub = getSubMath(aMath, subArea);
					
		printDebugMath(true, sub, __FUNCTION__);
		doMathParsing(sub);
		embedSubMath(aMath, sub, Oper::Parentheses, *area);

		printDebugMath(true, aMath, __FUNCTION__);
		doMathParsing(aMath);
	}
}

void MathParser::doMathBrackets(Math& aMath)
{
	OptArea area = findBigBlock(aMath, { LEFT_SQUARE_BRACKET_UPPER_CORNER, LEFT_SQUARE_BRACKET_EXTENSION, LEFT_SQUARE_BRACKET_LOWER_CORNER},
															{ RIGHT_SQUARE_BRACKET_UPPER_CORNER, RIGHT_SQUARE_BRACKET_EXTENSION, RIGHT_SQUARE_BRACKET_LOWER_CORNER},
															"brackets");

	if (area)
	{
		Area subArea{ area->upperLeft.right(), area->lowerRight.left() };
		Math sub = getSubMath(aMath, subArea);
					
		printDebugMath(true, sub, __FUNCTION__);
		doMathParsing(sub);
		embedSubMath(aMath, sub, Oper::Matrix, *area);

		printDebugMath(true, aMath, __FUNCTION__);
		doMathParsing(aMath);
	}
}

void MathParser::doMathFractionBar(Math& aMath)
{
	OptPosition leftHorizontalBar = findAny(aMath, HORIZONTAL_BAR);
	if (leftHorizontalBar)
	{
		printDebugMath(true, aMath, __FUNCTION__);
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
			printDebugMath(true, subNumerator, __FUNCTION__, "numerator");

			OptPosition leftLowerPosition = findBottomOfFraction(aMath, *leftHorizontalBar, rightHorizontalBar->x);
			if (leftLowerPosition)
			{
			  leftLowerPosition->x = leftHorizontalBar->x;
				Position leftPos = Position{ leftHorizontalBar->x, leftHorizontalBar->y + 1};
				rightPos = Position{ rightHorizontalBar->x, leftLowerPosition->y };
				Area areaDenomenator = Area{ leftPos, rightPos };
				subDenomenator = getSubMath(aMath, areaDenomenator);
				printDebugMath(true, subDenomenator, __FUNCTION__, "denomerator");
		
				doMathParsing(subNumerator);
				doMathParsing(subDenomenator);

				Area area = join(areaNumerator, areaDenomenator);
				embedSubMath(aMath, subNumerator, subDenomenator, Oper::Division, area);
				
				printDebugMath(true, aMath, __FUNCTION__);
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
			printDebugMath(true, sub, __FUNCTION__);
			doMathParsing(sub);
			leftPos = Position{ squareRoot->x, rootBar->y };
			embedSubMath(aMath, sub, Oper::SquareRoot, Area{ leftPos, rightPos });
			
			printDebugMath(true, aMath, __FUNCTION__);
			doMathParsing(aMath);
		}
		else
		{
			OptArea rightArea = getRightArea(aMath, (*squareRoot).right());
			if (rightArea)
			{
				Math sub = getSubMath(aMath, *rightArea);
				printDebugMath(false, sub, __FUNCTION__);
			  doMathParsing(sub);
				embedSubMath(aMath, sub, Oper::SquareRoot, *rightArea);
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
}

void MathParser::doMathPower(Math& aMath)
{
  auto [powerFound, isSuperscriptBase, isSuperscriptExp, expArea, baseArea] = findPower(aMath);

	if (powerFound)
	{
		printDebugMath(false, aMath, __FUNCTION__);
		Math exp = getSubMath(aMath, expArea);
		printDebugMath(false, exp, "");
		Math base = getSubMath(aMath, baseArea);
		printDebugMath(false, base, "");
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
		
		printDebugMath(false, aMath, "");
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

			if (endX < x && isSymbolStart(kar))
			{
				auto [endX, symbol] = parseSymbol(aMath, x, y);
				if (endX >= x)
				{
					Position startPos{ x, y };
					Position endPos{ endX, y };
					Math var = createSubMath(aMath, Area{ startPos, endPos}, symbol.intName);
					Oper oper = symbol.type == Symbol::Type::Function
						          ? Oper::FunctionName
						          : Oper::Symbol;
					embedSubMath(aMath, var, oper, Area{ startPos, endPos }, symbol);
				}
			}
		}
	}		
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
						if (aMath.matrix[y][x].mathValue->oper == Oper::Number)
						{
							error(Position{ x, y }, "Number without operator");
						}

						if (aMath.matrix[y][x].mathValue->oper != Oper::FunctionName)
						{
							MathValueSPtr& rightMathValue = aMath.matrix[y][x].mathValue;
							Area leftArea = lastMathValue->getArea(lastX, y);
							Area rightArea = aMath.matrix[y][x].getArea(x, y);
							Math left = getSubMath(aMath, leftArea);
							Math right = getSubMath(aMath, rightArea);
							Area area = join(leftArea, rightArea);
							if (lastMathValue->type == NumberType::Imaginary || rightMathValue->type == NumberType::Imaginary)
							{
								if (lastMathValue->type == NumberType::Imaginary && rightMathValue->type == NumberType::Imaginary)
								{
									Math number = createSubMath(aMath, area, "-1");
									embedSubMath(aMath, number, Oper::Number, area);
								}
								else if (lastMathValue->type == NumberType::Imaginary)
								{
									embedSubMath(aMath, right, Oper::Number, area)->type = NumberType::Imaginary;
								}
								else
								{
									embedSubMath(aMath, left, Oper::Number, area)->type = NumberType::Imaginary;
								}
							}
							else
							{
								printDebugMath(true, left, __FUNCTION__);
								printDebugMath(true, right, __FUNCTION__);
								embedSubMath(aMath, left, right, Oper::Monomial, area);
							}

							printDebugMath(true, aMath, __FUNCTION__);
						}
					}
					else if (aMath.matrix[y][x].mathValue->oper != Oper::FunctionName)
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
}

void MathParser::doMathFunctionCall(Math& aMath)
{
	for (int y = 0; y < aMath.height(); ++y)
	{
		int lastX = -1;
		MathValue* lastMathValue = nullptr;

		for (int x = aMath.width() - 1; x >= 0; --x)
		{
			if (aMath.hasSubMatrix(y, x))
			{
				if (lastMathValue == nullptr || aMath.matrix[y][x].mathValue != lastMathValue->mathValue)
				{
					if (lastX != -1)
					{
					  if (aMath.matrix[y][x].mathValue->oper == Oper::FunctionName)
						{
							MathValueSPtr& rightMathValue = aMath.matrix[y][x].mathValue;
							auto [isSuperscript, leftArea] = getRightToLeftSymbol(aMath, Position{ x, y });
							Area rightArea = lastMathValue->getArea(lastX, y);
							Math left = getSubMath(aMath, *leftArea);
							Math right = getSubMath(aMath, rightArea);
							Area area = join(*leftArea, rightArea);
							printDebugMath(true, left, __FUNCTION__);
							printDebugMath(true, right, __FUNCTION__);
							embedSubMath(aMath, left, right, Oper::FunctionCall, area, aMath.matrix[y][x].mathValue->symbol);
							printDebugMath(true, aMath, __FUNCTION__);
						}
					}
					else if (aMath.matrix[y][x].mathValue->oper == Oper::FunctionName)
					{
						error(Position{ x, y }, "Function without value");
					}
					else
					{
						lastMathValue = &aMath.matrix[y][x];
						lastX = x;
					}
				}
			}
			else if (!isBlank(aMath.matrix[y][x].value))
			{
				lastX = -1;
				lastMathValue = nullptr;
			}
		}
	}
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

				// not function at left?
				if (leftArea)
				{
						const auto& left = aMath.mathValue(leftArea->upperLeft);
						if (left.oper == Oper::FunctionName)
						{
							leftArea = std::nullopt;
						}
				}

				// no operand at left?
				if (!leftArea)
				{
					auto rightArea = getRightArea(aMath, Position{ x + 1, y }, true);
					if (rightArea)
					{
						Math right = getSubMath(aMath, *rightArea);
						if (aMath.mathValue(rightArea->upperLeft).oper != Oper::FunctionName)
						{
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
		auto rightArea = getRightArea(aMath, Position{ x + 1, y }, true);
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
		else
		{
			error(Position{ x, y }, std::format("Missing operand after {} operator", operToString(aOper)));
		}
	}
	else
	{
		error(Position{ x, y }, std::format("Missing operand before {} operator", operToString(aOper)));
	}
}

void MathParser::doMathRenameSimpleOperators(Math& aMath)
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
				kar = '*';
			}
			else if (kar == DIVIDE_SIGN ||
					     kar == DIVIDE_SLASH ||
							 kar == '/')
			{
				kar = '/';
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
				doStartMathParsing(superMath);
				MathValue* mathValue = embedSubMath(aMath, superMath, Oper::Nested, area);
				mathValue->superscript = true;
			}
		}
	}		
}

void MathParser::doMathSimpleMatching(Math& aMath, uint32_t left, uint32_t right, Oper oper, const char* desc, bool addLeft)
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

						for (int up = 1; y - up >= 0; ++up)
						{
							uint32_t upKar = aMath(y - up, x);
							if (!isBlank(upKar) && !badSomethingVertical(upKar))
							{
								upY = std::min(upY, y - up);
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
								downY = std::max(downY, y + down);
							}
							else
							{
								break;
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
					error(Position{ startX, y }, std::string("unbalanced ") + desc);
				}
				else
				{				
					Area parensArea{ Position{ startX + 1, upY }, Position{ x - 1, downY } };
					Math parens = getSubMath(aMath, parensArea);
					printDebugMath(true, parens, __FUNCTION__, std::format("right {}", desc));
					doMathParsing(parens);
					parensArea.upperLeft.x = startX;
					parensArea.lowerRight.x = x;

					if (addLeft)
					{
						auto [isSuperscript, leftArea] = getRightToLeftSymbol(aMath, Position{ startX - 1, y });
						if (!leftArea)
						{
							error(Position{ x, y }, std::string("expecting value before ") + desc);
						}
						else
						{
							Math left = getSubMath(aMath, *leftArea);
					    printDebugMath(true, left, __FUNCTION__, std::format("left {}", desc));
							embedSubMath(aMath, left, parens, oper, leftArea->merge(parensArea));
						}
					}
					else
					{
						embedSubMath(aMath, parens, oper, parensArea);
					}

				  printDebugMath(true, aMath, __FUNCTION__, desc);
				}
			}
			else if (kar == right)
			{
				error(Position{ x, y }, std::string("unbalanced ") + desc);
			}
		}
	}
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

bool MathParser::isSymbolStart(uint32_t kar) const
{
	return isalpha(kar, m_localeUtf8) || isSymbolGreek(kar);
}

bool MathParser::isSymbolNext(uint32_t kar) const
{
	return isSymbolStart(kar) || isdigit(kar, m_localeUtf8) || isSymbolSuffix(kar) || kar == '_';
}

std::tuple<int, Symbol> MathParser::parseSymbol(
				const Math& aMath,
				int x,
				int y) const
{
	Symbol symbol;
	if (isSymbolStart(aMath(y, x)))
	{
		int start = x;
		std::string variable;
		auto backIns = std::back_inserter(variable);

		int size = aMath.width();
		for (; x < size && isSymbolNext(aMath(y, x)); ++x)
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

Area MathParser::join(const Area& aLeftArea, const Area& aRightArea) const
{
  return Area{ Position{ std::min(aLeftArea.upperLeft.x, aRightArea.upperLeft.x), std::min(aLeftArea.upperLeft.y, aRightArea.upperLeft.y) },
						   Position{ std::max(aLeftArea.lowerRight.x, aRightArea.lowerRight.x), std::max(aLeftArea.lowerRight.y, aRightArea.lowerRight.y) } };
}

Area MathParser::totalArea(const Math& aMath, const Area& aArea) const
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
	return kar == '/' || 
		     kar == '*' ||
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

OptArea MathParser::getRightArea(
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

std::tuple<bool, OptArea> MathParser::getRightToLeftSymbol(
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

bool MathParser::isSymbolGreek(uint32_t aKar) const
{
	return ((aKar >= 0x388U && aKar <= 0x3E1U));
}

Area MathParser::getEndExponent(
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

std::tuple<bool, bool, bool, Area, Area> MathParser::findPower(
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

OptPosition MathParser::findAnyOf(
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

OptPosition MathParser::findAny(
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

OptPosition MathParser::findMatchingBig(
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

OptPosition MathParser::findRepeatingRight(
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

OptPosition MathParser::findMatchingDown(
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

OptPosition MathParser::findDiagonalRightUp(
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

OptPosition MathParser::findTopOfFraction(
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

OptPosition MathParser::findBottomOfFraction(
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

Position MathParser::mathPos(const Math& aMath, const Position& aPosition) const
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
	if (mSymbols.find(aSymbol.name) != mSymbols.end())
	{
		error(Position(), "Duplicate name: " + aSymbol.name);
	}
	else
	{
		mSymbols[aSymbol.name] = aSymbol;
	}
}

} // namespace nate