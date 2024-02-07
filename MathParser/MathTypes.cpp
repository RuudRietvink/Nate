#pragma once

#include "MathTypes.h"

namespace nate
{

Position Position::left() const
{
	return *this - Position{ 1, 0 };
}
		
Position Position::right() const
{
	return *this + Position{ 1, 0 };
}
		
Position Position::up() const
{
	return *this - Position{ 0, 1 };
}

Position Position::down() const
{
	return *this + Position{ 0, 1 };
}
		
Position operator+(const Position& aLeft, const Position aRight)
{
	return Position{ aLeft.x + aRight.x, aLeft.y + aRight.y };
}

Position operator-(const Position& aLeft, const Position aRight)
{
	return Position{ aLeft.x - aRight.x, aLeft.y - aRight.y };
}


Area Area::merge(const Area& otherArea)
{
	Position newUpperLeft { std::min(upperLeft.x,  otherArea.upperLeft.x),
		                      std::min(upperLeft.y,  otherArea.upperLeft.y)};
	Position newLowerRight{ std::max(lowerRight.x, otherArea.lowerRight.x),
		                      std::max(lowerRight.y, otherArea.lowerRight.y)};
	return Area{ newUpperLeft, newLowerRight };
}


MathValue::MathValue(uint32_t aValue)
	: value(aValue),
		oper(Oper::Number),
		size{0, 0}
{}
		
MathValue::MathValue(const Math& aMath1, const Math& aMath2, Oper aOper, const Size& aSize, const Symbol& aSymbol)
	: value(SUBMATRIX), 
		oper(aOper), 
	  symbol(aSymbol),
		embedded1(aMath1), 
		embedded2(aMath2),
		size(aSize)
{
	mathValue = std::make_shared<MathValue>(*this);
}
		
MathValue::MathValue(const MathValueSPtr& aMathValue, const Size& aSize)
	: value(SUBMATRIXREFERENCE), 
		oper(aMathValue->oper),
		mathValue(aMathValue),
		size(aSize)
{}
		
bool MathValue::isSubMatrix() const { return value == SUBMATRIX; }
bool MathValue::hasSubMatrix() const { return isSubMatrix() || value == SUBMATRIXREFERENCE; }
bool MathValue::isSuperscript() const { return superscript; }
Area MathValue::getArea(int x, int y) const
{
	Area result;

	if (value == SUBMATRIX)
	{
		result = Area{ Position{ x, y }, Position{ x + size.width - 1, y + size.height - 1 } };
	}
	else
	{
		result = Area{ Position{ x + size.width, y + size.height }, 
					          Position{ x + mathValue->size.width + size.width - 1, y + mathValue->size.height + size.height - 1 }};
	}

	return result;
}

int Math::height() const { return (int)matrix.size(); }
int Math::width() const { return matrix.size() > 0 ? (int)matrix[0].size() : 0; }
uint32_t Math::operator()(int y, int x) const { return matrix[y][x].value; }
uint32_t& Math::operator()(int y, int x) { return matrix[y][x].value; }
bool Math::isSubMatrix(int y, int x) const { return matrix[y][x].isSubMatrix(); }
bool Math::hasSubMatrix(int y, int x) const { return matrix[y][x].hasSubMatrix(); }
bool Math::isSuperscript(int y, int x) const { return matrix[y][x].isSuperscript(); }
const MathValue& Math::mathValue(int y, int x) const { return matrix[y][x]; }
uint32_t Math::operator()(const Position& aPosition) const { return matrix[aPosition.y][aPosition.x].value; }
uint32_t& Math::operator()(const Position& aPosition) { return matrix[aPosition.y][aPosition.x].value; }
bool Math::isSubMatrix(const Position& aPosition) const { return matrix[aPosition.y][aPosition.x].isSubMatrix(); }
bool Math::hasSubMatrix(const Position& aPosition) const { return matrix[aPosition.y][aPosition.x].hasSubMatrix(); }
bool Math::isSuperscript(const Position& aPosition) const { return matrix[aPosition.y][aPosition.x].isSuperscript(); }
const MathValue& Math::mathValue(const Position& aPosition) const { return matrix[aPosition.y][aPosition.x]; }

} // namespace nate