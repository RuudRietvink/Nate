#pragma once

#include "Arg.h"
#include "ExprNode.h"
#include "Type.h"
#include "WithFlags.h"

#include <string>
#include <vector>
#include <ostream>
#include <tuple>

class Identifier;

class Method : public WithFlags
{
public:
	Method();
	virtual ~Method() = default;

	virtual bool matches(const ExprNodesCIter& aBegin, const ExprNodesCIter& aEnd) const;
	virtual std::tuple<std::string, bool> checkArgTypes(const ExprNodesCIter& aBegin, const ExprNodesCIter& aEnd) const;
	virtual std::tuple<std::string, std::string, TypePtr, Flags> evaluate(const ExprNodesCIter& aBegin, const ExprNodesCIter& aEnd) const;
	virtual void addArgWord(const std::string& aWord);
	virtual void addArgId(const IdentifierPtr& aId);
	virtual std::string toCodeWord(const std::string& aWord) const;
	
	int                         priority() const;
	void                        setPriority(int aValue);
  const TypePtr&              type() const;
	void                        setType(const TypePtr& aType);
	const std::vector<Arg>&     args() const;
	std::vector<Arg>&           args();
	Arg&                        curArg();
	const std::string&          pattern() const;
	const std::string&          signature() const;
	void                        setReturnFlag(const std::string& aFlag);
		
	const std::string&          code() const;
	std::string&                code();

	static const size_t Highest      = 1;
	static const size_t Num          = 2;
	static const size_t Same         = 3;
	static const size_t RightLeft    = 4;
	static const size_t None         = 5;
	static const size_t Last         = 6;
	static const size_t LeftMonomial = 7;
	static const size_t Unary        = 8;
  
private:
	std::string          mCode;
	TypePtr              mType;
	std::vector<Arg>     mArgs;
	std::string          mSignature;
	mutable std::string  mPattern;
	int                  mPriority = 0;
};
std::ostream& operator<<(std::ostream& aStream, const Method& aValue);



