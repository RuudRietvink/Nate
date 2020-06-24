#pragma once

#ifdef MATHPARSER_EXPORTS
#define MATHPARSER_API __declspec(dllexport)
#else
#define MATHPARSER_API __declspec(dllimport)
#endif

#include <string>
#include <vector>
#include <optional>
#include <tuple>

class MATHPARSER_API MathParser
{
public:
	static const uint32_t SPACE                        = 0x20;  
	static const uint32_t SUBMATRIX                    = 0x1; 
	static const uint32_t BADCHAR                      = 0x0; 

	virtual ~MathParser() {}

	struct Math;

	void setTabSize(uint32_t aTabSize);
	std::string doMath(std::istream& aStream, int line = 0);
	std::string doMath(Math& aMath);
	
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
		inline int height() const { return (int)matrix.size(); }
		inline int width() const { return matrix.size() > 0 ? (int)matrix[0].size() : 0; }
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
			: value(1U), 
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
	virtual uint32_t operatorMultiply() const;
	virtual uint32_t operatorDivide() const;

private:
	Position mathPos(const Math& aMath, 
									 const Position& aPosition) const;
	void fillUpMath(Math& aMath) const;
	void printMath(const Math& aMath) const;
	void printDebugMath(const Math& aMath) const;
	void printDebugMath(const Math& aMath,
										  const Position& aLeftPosition,
										  const Position& aRightPosition) const;
	std::string mathString(const Math& aMath) const;
	void doStartMathParsing(Math& aMath);
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
	void doMathFractionBar(Math& aMath);
	void doMathSquareRoot(Math& aMath);
	void doMathPower(Math& aMath);
	void doMathSimpleOperators(Math& aMath);

	OptPosition findAny(const Math& aMath, 
											uint32_t aSearchChar) const;
	OptPosition findAnyOf(const Math& aMath, 
									  		const std::initializer_list<uint32_t>& aSearchChars) const;
	OptPosition getSymbol(const Math& aMath, 
												const Position& aLeftPosition,
												bool aAllowSpaces = false) const;
	std::tuple<bool, OptPosition> getRightToLeftSymbol(const Math& aMath, 
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
	std::tuple<bool, OptPosition> findPower(Math& aMath,
																					Position& aEndExponent,
																					Position& aStartBase,
																					Position& aEndBase) const;
	bool isSymbolSuffix(uint32_t aKar) const;
	uint32_t getSuperscript(uint32_t aKar) const;
	uint32_t optSuperscript(bool aCheckSuperScript, uint32_t aKar) const;
	void unsuperscript(Math& aMath,
										 const Position& aLeftPosition,
										 const Position& aRightPosition) const;

	void mathError(const Math& aMath, 
								 const Position& aPosition,
								 uint32_t aBadChar,
							 	 uint32_t aInbetweenChar,
								 uint32_t aSearchChar) const;
	Math getSubMath(const Math& aMath, 
									const Position& aLeftUpperPosition, 
									const Position& aRightLowerPosition) const;

	uint32_t mTabSize = 4;
};

MATHPARSER_API extern MathParser mathParser;
