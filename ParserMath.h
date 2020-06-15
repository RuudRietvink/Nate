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
	
	struct Position;
	struct MathValue;

	typedef std::vector<MathValue> MathVector;
	typedef std::vector<MathVector> MathMatrix;
	typedef std::optional<Position> OptPosition;
	
	struct Math
	{
		size_t x = 0;
		size_t y = 0;
		MathMatrix matrix;
	};
	
	struct Position
	{
		int x = 0;
		int y = 0;
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

	Position mathPos(const Math& aMath, 
									 const Position& aPosition) const;
	void fillUpMath(Math& aMath);
	void printMath(const Math& aMath) const;
	std::string mathString(const Math& aMath) const;
	void doMathParsing(Math& aMath);
	void embedSubMath(Math& aMath, 
										const Math& aSubMath1, 
										const Math& aSubMath2, 
										const std::string& aOper,
										const Position& aLeftUpperPosition,
										const Position& aRightLowerPosition
										);
	void doMathParentheses(Math& aMath);
	void doMathDivision(Math& aMath);
	void doMathSquareRoot(Math& aMath);
	OptPosition findAny(const Math& aMath, 
											uint32_t aSearchChar) const;
	OptPosition findMatchingRight(const Math& aMath, 
																const Position& aLeftPosition,
																uint32_t aSearchChar) const;
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

	void mathError(const Math& aMath, 
								 const Position& aPosition,
								 uint32_t aBadChar,
							 	 uint32_t aInbetweenChar,
								 uint32_t aSearchChar) const;
	Math getSubMath(const Math& aMath, 
									const Position& aLeftUpperPosition, 
									const Position& aRightLowerPosition);
};


