#pragma once

#include <string>
#include <vector>
#include <optional>

class ParserMath
{
public:
	virtual ~ParserMath() {}

	struct Math;

	void doMath(Math& aMath);
	
	class Position;
	struct MathValue;

	typedef std::vector<MathValue> MathVector;
	typedef std::vector<MathVector> MathMatrix;
	typedef std::optional<Position> OptPosition;
	
	struct Math
	{
		size_t x = 0;
		size_t y = 0;
		MathMatrix matrix;
		int height() const { return (int)matrix.size(); }
		int width() const { return matrix.size() > 0 ? (int)matrix[0].size() : 0; }
	};
	
	class Position
	{
	public:
		int x = 0;
		int y = 0;

		Position left() const
		{
			return *this - Position{ 1, 0 };
		}
		
		Position right() const
		{
			return *this + Position{ 1, 0 };
		}
		
		Position up() const
		{
			return *this - Position{ 0, 1 };
		}

		Position down() const
		{
			return *this + Position{ 0, 1 };
		}
		
		friend Position operator+(const Position& aLeft, const Position aRight)
		{
			return Position{ aLeft.x + aRight.x, aLeft.y + aRight.y };
		}

		friend Position operator-(const Position& aLeft, const Position aRight)
		{
			return Position{ aLeft.x - aRight.x, aLeft.y - aRight.y };
		}
	};

	struct MathValue
	{
		MathValue(uint32_t aValue)
			: value(aValue) {}
		
		MathValue(const Math& aMath1, const Math& aMath2, const std::string& aOper)
			: value(0U), 
			  oper(aOper), 
			  embedded1(aMath1), 
			  embedded2(aMath2) {}

		uint32_t value;
		std::string oper;
		Math embedded1;
		Math embedded2;
	};

	
protected:
	virtual void error(const Position& aPosition, const std::string& aError) const;

	virtual std::string operParens(const Math& aMath) const;
	virtual std::string operDivide(const Math& aMathLeft, const Math& aMathRight) const;
	virtual std::string operSquareRoot(const Math& aMath) const;
	virtual std::string operPower(const Math& aMathBase, const Math& aMathExp) const;
	virtual std::string operExp(const Math& aMathExp) const;
	virtual std::string operMultiply(const Math& aMathLeft, const Math& aMathRight) const;

	Position mathPos(const Math& aMath, 
									 const Position& aPosition) const;
	void fillUpMath(Math& aMath);
	void printMath(const Math& aMath) const;
	void printDebugMath(const Math& aMath) const;
	std::string mathString(const Math& aMath) const;
	void doMathParsing(Math& aMath);
	void embedSubMath(Math& aMath, 
										const Math& aSubMath1, 
										const Math& aSubMath2, 
										const std::string& aOper,
										const Position& aLeftUpperPosition,
										const Position& aRightLowerPosition,
										const Position& aPlacePosition);
	void embedSubMath(Math& aMath, 
										const Math& aSubMath, 
										const std::string& aOper,
										const Position& aLeftUpperPosition,
										const Position& aRightLowerPosition,
										const Position& aPlacePosition);
	void clearMath(Math& aMath, 
							   const Position& aLeftUpperPosition,
								 const Position& aRightLowerPosition);

	void doMathParentheses(Math& aMath);
	void doMathDivision(Math& aMath);
	void doMathSquareRoot(Math& aMath);
	void doMathPower(Math& aMath);
	void doMathMultiplication(Math& aMath);

	OptPosition findAny(const Math& aMath, 
											uint32_t aSearchChar) const;
	OptPosition findAnyOf(const Math& aMath, 
									  		const std::initializer_list<uint32_t>& aSearchChars) const;
	OptPosition getSymbol(const Math& aMath, 
												const Position& aLeftPosition,
												bool aAllowSpaces = false) const;
	OptPosition getRightToLeftSymbol(const Math& aMath, 
												           const Position& aRightPosition,
																	 bool aAllowSpaces = false) const;
	OptPosition findMatchingBigParens(const Math& aMath, 
																 const Position& aLeftPosition) const;
	OptPosition findRepeatingRight(const Math& aMath, 
																 const Position& aLeftPosition,
																 uint32_t aSearchChar) const;
	OptPosition findMatchingDown(const Math& aMath, 
														   const Position& aUpperPosition,
															 uint32_t aInbetweenChar,
															 uint32_t aSearchChar) const;
	OptPosition findDiagonalRightUp(const Math& aMath, 
																	const Position& aLeftLowerPosition,
																	uint32_t aInbetweenChar,
																	uint32_t aSearchChar) const;
	OptPosition findUntilUp(const Math& aMath, 
													const Position& aLowerPosition,
													uint32_t aSearchChar) const;
	OptPosition findUntilDown(const Math& aMath, 
												  	const Position& aUpperPosition,
												  	uint32_t aSearchChar) const;
	OptPosition findSomethingLeft(const Math& aMath, 
												        const Position& aLeftUpperPosition,
															  const Position& aLeftLowerPosition) const;
	OptPosition findSomethingRight(const Math& aMath, 
												        const Position& aRightUpperPosition,
															  const Position& aRightLowerPosition) const;
	Position getEndExponent(const Math& aMath,
													const Position& aStartExponent) const;
	OptPosition findPower(const Math& aMath,
												Position& aEndExponent,
												Position& aStartBase,
												Position& aEndBase) const;

	void mathError(const Math& aMath, 
								 const Position& aPosition,
								 uint32_t aBadChar,
							 	 uint32_t aInbetweenChar,
								 uint32_t aSearchChar) const;
	Math getSubMath(const Math& aMath, 
									const Position& aLeftUpperPosition, 
									const Position& aRightLowerPosition);
};


