#pragma once

#include <string>
#include <vector>
#include <optional>
#include <tuple>
#include <map>
#include <memory>
#include <tuple>

namespace nate
{
	
static const uint32_t SPACE                        = 0x20;  
static const uint32_t SUBMATRIXREFERENCE           = 0x2;  
static const uint32_t SUBMATRIX                    = 0x1; 
static const uint32_t BADCHAR                      = 0x0; 

struct Math;
struct Symbol;

struct InputPosition 
{
	int x = 0;
	int y = 0;
};

class Position : public InputPosition
{
public:
	Position left() const;
		
	Position right() const;
		
	Position up() const;

	Position down() const;
		
	friend Position operator+(const Position& aLeft, const Position aRight);

	friend Position operator-(const Position& aLeft, const Position aRight);
};
	
class Area
{
public:
	Position upperLeft;
	Position lowerRight;
	
	Area merge(const Area& otherArea);
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
	
enum class Oper
{
	Unknown,
	Nested,
	Parentheses,
	Brackets,
	Matrix,
	Absolute,
	Floor,
	Ceiling,
	Division,
	SquareRoot,
	Power,
	Exponential,
	Multiplication,
	Addition,
	Subtraction,
	Symbol,
	FunctionCall,
	FunctionName,
	Constant,
	Number,
	Monomial,
	UnaryMinus,
	UnaryPlus,
	Assignment
};
	
enum class NumberType
{
	Real,
	Int,
	Complex,
	Imaginary
};

struct Symbol
{
	enum class Type
	{
		Variable,
		Constant,
		Number,
		Function
	};

	Type type = Type::Number;
	std::string name;
	std::string intName;
	std::string codeName;
	NumberType valueType = NumberType::Real;
};

struct Math
{
	size_t x = 0;
	size_t y = 0;
	MathMatrix matrix;
	int height() const;
	int width() const;
	uint32_t operator()(int y, int x) const;
	uint32_t& operator()(int y, int x);
	bool isSubMatrix(int y, int x) const;
	bool hasSubMatrix(int y, int x) const;
	bool isSuperscript(int y, int x) const;
	const MathValue& mathValue(int y, int x) const;
	uint32_t operator()(const Position& aPosition) const;
	uint32_t& operator()(const Position& aPosition);
	bool isSubMatrix(const Position& aPosition) const;
	bool hasSubMatrix(const Position& aPosition) const;
	bool isSuperscript(const Position& aPosition) const;
	const MathValue& mathValue(const Position& aPosition) const;
};

struct MathValue
{
	MathValue(uint32_t aValue);
		
	MathValue(const Math& aMath1, const Math& aMath2, Oper aOper, const Size& aSize, const Symbol& aSymbol);
		
	MathValue(const MathValueSPtr& aMathValue, const Size& aSize);

	MathValue(Oper aOper, const std::vector<Math> aMatrixCells, Size aMatrixSize);
		
	bool isSubMatrix() const;
	bool hasSubMatrix() const;
	bool isSuperscript() const;
	Area getArea(int x, int y) const;

	uint32_t value = BADCHAR;
	Oper oper = Oper::Unknown;
	Symbol symbol;
	Math embedded1;
	Math embedded2;
	Size size;
	Position lowerRight;
	bool superscript = false;
	NumberType type = NumberType::Real;
	std::vector<Math> matrixCells;
	Size matrixSize;
	MathValueSPtr mathValue;
};

	

} // namespace nate
