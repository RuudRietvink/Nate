#pragma once

#define MATHPARSER_API 
//#ifdef MATHPARSER_EXPORTS
//#define MATHPARSER_API __declspec(dllexport)
//#else
//#define MATHPARSER_API __declspec(dllimport)
//#endif

#include "MathTypes.h"

#include <string>
#include <vector>
#include <optional>
#include <tuple>
#include <map>
#include <memory>
#include <tuple>

namespace nate
{
class MathParser
{
public:
	virtual MATHPARSER_API ~MathParser() = default;

	MATHPARSER_API virtual void setTabSize(uint32_t aTabSize);
	MATHPARSER_API virtual void addVariable(const std::string& aName, const std::string& aCodeName, NumberType aType = NumberType::Real);
	MATHPARSER_API virtual void addConstant(const std::string& aName, const std::string& aIntName, const std::string& aCodeName, NumberType aType = NumberType::Real);
	MATHPARSER_API virtual void addFunction(const std::string& aName, const std::string& aCodeName, NumberType aType = NumberType::Real);
	MATHPARSER_API virtual std::string doMath(std::istream& aStream, int line = 0);
	MATHPARSER_API virtual std::string doMath(Math& aMath);
	
protected:
	MATHPARSER_API virtual void init();
	MATHPARSER_API virtual void error(const InputPosition& aPosition, const std::string& aError) const;
	
	MATHPARSER_API virtual std::string codeOperator(const MathValue& aMathValue) const = 0;
	MATHPARSER_API virtual std::string codeFunction(const std::string& aName, const Math& aMathArg) const;

	MATHPARSER_API virtual bool isSymbol(const std::string& aInput) const;
	MATHPARSER_API virtual bool isNumber(const std::string& aInput) const;
	MATHPARSER_API virtual bool isSymbolStart(uint32_t kar) const;
	MATHPARSER_API virtual bool isSymbolNext(uint32_t kar) const;
	
	virtual std::string mathString(const Math& aMath) const;
	virtual bool needsParens(const Math& aMath) const;

private:
	Symbol getSymbol(const std::string& aInput) const;

	Position mathPos(const Math& aMath, 
									 const Position& aPosition) const;
	void fillUpMath(Math& aMath) const;
	void printMath(const Math& aMath, const std::string& aFunction, const std::string& aText = "") const;
	void printDebugMath(bool print, const Math& aMath, const std::string& aFunction, const std::string& aText = "") const;
	void doStartMathParsing(Math& aMath);
	void doMathParsing(Math& aMath);
	MathValue* embedSubMath(Math& aMath, 
										Oper aOper,
										const Math& aSubMath1, 
										const Math& aSubMath2, 
										const Area& aArea,
										const Symbol& aSymbol = {});
	MathValue* embedSubMath(Math& aMath, 
										Oper aOper,
										const Math& aSubMath, 
										const Area& aArea,
										const Symbol& aSymbol = {});
	MathValue* embedSubMath(Math& aMath,
										Oper aOper,
										const std::vector<Math> aMatrixCells,
										Size aMatrixSize);
	void fillerMath(Math& aMath, 
								 const Area& aArea,
								 const MathValueSPtr& aClearValue);
	void spaceMath(Math& aMath, 
								 const Area& aArea);
	
	void doMathSuperscript(Math& aMath);
	void doMathSimpleMatching(Math& aMath, uint32_t left, uint32_t right, Oper oper, const char* desc, bool addLeft = false);
	void doMathRenameSimpleOperators(Math& aMath);
	void doMathVariablesNumbers(Math& aMath);
	
	void doMathMonomial(Math& aMath);
	void doMathFunctionCall(Math& aMath);
	void doMathParentheses(Math& aMath);
	void doMathBrackets(Math& aMath);
	void doMathFractionBar(Math& aMath);
	void doMathSquareRoot(Math& aMath);
	void doMathPower(Math& aMath);
	std::tuple<std::vector<Math>, size_t, size_t> doMatrixCells(Math& aMath);	
	std::vector<int> getVerticalCellLines(const Math& aMath);
	std::vector<int> getHorizontalCellLines(const Math& aMath);
  std::vector<Math> doMatrixCells(Math& aMath, const std::vector<int>& verLines, const std::vector<int>& horLines);
	void doMathDownRightOperator(Math& aMath, int aOperChar, Oper aOper);
	void doMathUpLeftOperator(Math& aMath, uint32_t aOperChar, Oper aOper);
	void doMathOperator(Math& aMath, Oper aOper, int x, int y);
	void doMathUnaryLeadingOperator(Math& aMath, int aOperChar, Oper aOper);
	OptArea findBigBlock(Math& aMath, const std::vector<uint32_t>& aLeftCodes,
											const std::vector<uint32_t>& aRightCodes, const char* aDesc);

	OptPosition findAny(const Math& aMath, 
											uint32_t aSearchChar) const;
	OptPosition findAnyOf(const Math& aMath, 
									  		const std::initializer_list<uint32_t>& aSearchChars) const;
	OptArea getRightArea(const Math& aMath, 
									     const Position& aLeftPosition,
										   bool aAllowSpaces = false) const;
	std::tuple<bool, OptArea> getRightToLeftSymbol(const Math& aMath, 
																										 const Position& aRightPosition,
																										 bool aAllowSpaces = false) const;
	OptPosition findMatchingBig(const Math& aMath, 
														  const Position& aLeftPosition,
															uint32_t aLeftChar,
															uint32_t aRightChar,
															const char* aDesc) const;
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
	OptPosition findTopOfFraction(const Math& aMath, 
													const Position& aLowerPosition,
													int aRightX) const;
	OptPosition findBottomOfFraction(const Math& aMath, 
												  	const Position& aUpperPosition,
													  int aRightX) const;
	std::tuple<bool, bool> isFractionBar(const Math& aMath, 
																		   const Position& aPosition,
																			 int aRightX) const;
	
	bool isOperator(uint32_t kar) const;
	bool isBlank(uint32_t kar) const;
	bool isEmpty(uint32_t kar) const;
	bool badSomethingVertical(uint32_t kar) const;
	Area getEndExponent(const Math& aMath,
										  const Position& aStartExponent) const;
	Area join(const Area& aLeftArea, const Area& aRightArea) const;
	Area totalArea(const Math& aMath, const Area& aArea) const;
	std::tuple<bool, bool, bool, Area, Area> findPower(Math& aMath) const;
	bool isSymbolSuffix(uint32_t aKar) const;
	bool isSymbolGreek(uint32_t aKar) const;
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
	std::tuple<int, Symbol> parseSymbol(const Math& aMath,
																  			int x,
																  			int y) const;
	
	bool isPartOfNumber(uint32_t kar) const;
	int parseNumber(const Math& aMath,
									int x,
									int y) const;
	void addSymbol(const Symbol& aSymbol);
	std::string u2s(uint32_t aChar);
	std::string operToString(Oper aOper);

	uint32_t mTabSize = 4;
	std::map<std::string, Symbol> mSymbols;
	static std::locale m_localeUtf8;
};

} // namespace
