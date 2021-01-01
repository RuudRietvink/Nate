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
#include <map>
#include <memory>

class MathParser
{
public:
	static const uint32_t SPACE                        = 0x20;  
	static const uint32_t SUBMATRIXREFERENCE           = 0x2;  
	static const uint32_t SUBMATRIX                    = 0x1; 
	static const uint32_t BADCHAR                      = 0x0; 

	virtual MATHPARSER_API ~MathParser() {}

	struct Math;
	struct Symbol;

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
	
	struct Area
	{
		Position upperLeft;
		Position lowerRight;
	};
	
	struct Size
	{
		int width;
		int height;
	};

	struct MathValue;
	using MathValueSPtr = std::shared_ptr<MathValue>;

	typedef std::vector<MathValue> MathVector;
	typedef std::vector<MathVector> MathMatrix;
	typedef std::optional<Position> OptPosition;
	typedef std::optional<Area> OptArea;
	
	struct Math
	{
		size_t x = 0;
		size_t y = 0;
		MathMatrix matrix;
		inline int height() const { return (int)matrix.size(); }
		inline int width() const { return matrix.size() > 0 ? (int)matrix[0].size() : 0; }
		uint32_t operator()(int y, int x) const { return matrix[y][x].value; }
		uint32_t& operator()(int y, int x) { return matrix[y][x].value; }
		bool isSubMatrix(int y, int x) const { return matrix[y][x].isSubMatrix(); }
		bool hasSubMatrix(int y, int x) const { return matrix[y][x].hasSubMatrix(); }
		bool isSuperscript(int y, int x) const { return matrix[y][x].isSuperscript(); }
		const MathValue& mathValue(int y, int x) const { return matrix[y][x]; }
		uint32_t operator()(const Position& aPosition) const { return matrix[aPosition.y][aPosition.x].value; }
		uint32_t& operator()(const Position& aPosition) { return matrix[aPosition.y][aPosition.x].value; }
		bool isSubMatrix(const Position& aPosition) const { return matrix[aPosition.y][aPosition.x].isSubMatrix(); }
		bool hasSubMatrix(const Position& aPosition) const { return matrix[aPosition.y][aPosition.x].hasSubMatrix(); }
		bool isSuperscript(const Position& aPosition) const { return matrix[aPosition.y][aPosition.x].isSuperscript(); }
		const MathValue& mathValue(const Position& aPosition) const { return matrix[aPosition.y][aPosition.x]; }
	};
	
	enum class Oper
	{
		Unknown,
		Nested,
		Parentheses,
		Division,
		SquareRoot,
		Power,
		Exponential,
		Multiplication,
		Addition,
		Subtraction,
		Symbol,
		Number,
		Monomial,
		UnaryMinus,
		UnaryPlus
	};
	
	struct MathValue
	{
		enum class Type
		{
			Real,
			Int,
			Complex,
			Imaginary
		};

		MathValue(uint32_t aValue)
			: value(aValue),
		    oper(Oper::Number){}
		
		MathValue(const Math& aMath1, const Math& aMath2, Oper aOper, const Size& aSize)
			: value(SUBMATRIX), 
			  oper(aOper), 
			  embedded1(aMath1), 
			  embedded2(aMath2),
				size(aSize)
		{
			mathValue = std::make_shared<MathValue>(*this);
		}
		
		MathValue(const MathValueSPtr& aMathValue, const Size& aSize)
			: value(SUBMATRIXREFERENCE), 
			  oper(aMathValue->oper),
				mathValue(aMathValue),
				size(aSize)
		{}
		
		bool isSubMatrix() const { return value == SUBMATRIX; }
		bool hasSubMatrix() const { return isSubMatrix() || value == SUBMATRIXREFERENCE; }
		bool isSuperscript() const { return mathValue->superscript; }
		Area getArea(int x, int y) const
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

		uint32_t value = BADCHAR;
		Oper oper = Oper::Unknown;
		Math embedded1;
		Math embedded2;
		Size size;
		Position lowerRight;
		bool superscript = false;
		Type type = Type::Real;
		MathValueSPtr mathValue;
	};
	
	struct Symbol
	{
		enum class Type
		{
			Variable,
			Constant,
			Number
		};

		Type type = Type::Number;
		std::string name;
		MathValue::Type valueType = MathValue::Type::Real;
	};
	
	MATHPARSER_API void setTabSize(uint32_t aTabSize);
	MATHPARSER_API void addVariable(const std::string& aName);
	MATHPARSER_API void addConstant(const std::string& aName, MathValue::Type aType);
	MATHPARSER_API std::string doMath(std::istream& aStream, int line = 0);
	MATHPARSER_API std::string doMath(Math& aMath);
	
protected:
	MATHPARSER_API virtual void error(const Position& aPosition, const std::string& aError) const;
	
	MATHPARSER_API virtual std::string code(Oper aOper, const Math& aMathLeft, const Math& aMathRight = Math()) const;
	MATHPARSER_API virtual uint32_t operatorMultiply() const;
	MATHPARSER_API virtual uint32_t operatorDivide() const;

	virtual bool MATHPARSER_API isSymbol(const std::string& aInput) const;
	virtual bool MATHPARSER_API isNumber(const std::string& aInput) const;
	virtual bool MATHPARSER_API isVarStart(uint32_t kar) const;
	virtual bool MATHPARSER_API isVarNext(uint32_t kar) const;

	std::string mathString(const Math& aMath) const;
	bool needsParens(const Math& aMath) const;

private:
	Position mathPos(const Math& aMath, 
									 const Position& aPosition) const;
	void fillUpMath(Math& aMath) const;
	void printMath(const Math& aMath, const std::string& aText = "") const;
	void printDebugMath(const Math& aMath, const std::string& aText = "") const;
	void doStartMathParsing(Math& aMath);
	void doPrepareMathParsing(Math& aMath);
	void doMathParsing(Math& aMath);
	MathValue* embedSubMath(Math& aMath, 
										const Math& aSubMath1, 
										const Math& aSubMath2, 
										Oper aOper,
										const Area& aArea);
	MathValue* embedSubMath(Math& aMath, 
										const Math& aSubMath, 
										Oper aOper,
										const Area& aArea);
	void fillerMath(Math& aMath, 
								 const Area& aArea,
								 const MathValueSPtr& aClearValue);
	void spaceMath(Math& aMath, 
								 const Area& aArea);
	
	void doMathSuperscript(Math& aMath);
	void doMathSimpleParentheses(Math& aMath);
	void doMathSimpleOperators(Math& aMath);
	void doMathVariablesNumbers(Math& aMath);

	void doMathMonomial(Math& aMath);
	void doMathParentheses(Math& aMath);
	void doMathFractionBar(Math& aMath);
	void doMathSquareRoot(Math& aMath);
	void doMathPower(Math& aMath);
	void doMathOperator(Math& aMath, int aOperChar, Oper aOper);
	void doMathUnaryLeadingOperator(Math& aMath, int aOperChar, Oper aOper);

	OptPosition findAny(const Math& aMath, 
											uint32_t aSearchChar) const;
	OptPosition findAnyOf(const Math& aMath, 
									  		const std::initializer_list<uint32_t>& aSearchChars) const;
	OptArea getSymbol(const Math& aMath, 
										const Position& aLeftPosition,
										bool aAllowSpaces = false) const;
	std::tuple<bool, OptArea> getRightToLeftSymbol(const Math& aMath, 
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
	
	bool isBlank(uint32_t kar) const;
	bool isEmpty(uint32_t kar) const;
	bool badSomething(uint32_t kar) const;
	bool badSomethingVertical(uint32_t kar) const;
	OptPosition findSomethingLeft(const Math& aMath, 
												        const Position& aLeftUpperPosition,
															  const Position& aLeftLowerPosition) const;
	OptPosition findSomethingRight(const Math& aMath, 
												        const Position& aRightUpperPosition,
															  const Position& aRightLowerPosition) const;
	Area getEndExponent(const Math& aMath,
										  const Position& aStartExponent) const;
	Area join(const Area& aLeftArea, const Area& aRightArea) const;
	Area totalArea(const Math& aMath, const Area& aArea) const;
	std::tuple<bool, bool, OptArea, Area> findPower(Math& aMath) const;
	bool isSymbolSuffix(uint32_t aKar) const;
	uint32_t getSuperscript(uint32_t aKar) const;
	uint32_t optSuperscript(bool aCheckSuperScript, uint32_t aKar) const;

	void mathError(const Math& aMath, 
								 const Position& aPosition,
								 uint32_t aBadChar,
							 	 uint32_t aInbetweenChar,
								 uint32_t aSearchChar) const;
	Math getSubMath(const Math& aMath, 
									const Area& aArea) const;
	Math createSubMath(const Math& aMath, 
										 const Area& aArea, 
									   const std::string& aText) const;
	
	std::string popFront(const std::string& aInput) const;
	std::string popBack(const std::string& aInput) const;
	std::tuple<int, Symbol> parseVariable(const Math& aMath,
																  			int x,
																  			int y) const;
	
	bool isPartOfNumber(uint32_t kar) const;
	int parseNumber(const Math& aMath,
									int x,
									int y) const;
	void addSymbol(const Symbol& aSymbol);

	uint32_t mTabSize = 4;
	std::map<std::string, Symbol> mSymbols;
	static std::locale m_localeUtf8;
};

MATHPARSER_API extern MathParser mathParser;
