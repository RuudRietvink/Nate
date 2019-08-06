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
class Record;

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
	virtual void endDecl();
	
	using ArgVector        = std::vector<Arg>;
	using ArgConstIterator = ArgVector::const_iterator;

	int                 priority() const;
	void                setPriority(int aValue);
  const TypePtr&      type() const;
	void                setType(const TypePtr& aType);
	const ArgVector&    args() const;
	ArgVector&          args();
	Arg&                curArg();
	const std::string&	pattern() const;
	const std::string&  signature() const;
	void                setReturnFlag(const std::string& aFlag);
	Record*             getOwner(const ExprNodesCIter& aNodeIter) const;
		
	const std::string&          code() const;
	std::string&                code();

	static const size_t Highest      = 1;
	static const size_t Num          = 2;
	static const size_t Same         = 3;
	static const size_t RightLeft    = 4;
	static const size_t None         = 5;
	static const size_t Last         = 6;
	static const size_t Unary        = 7;
	static const size_t ConstExpr    = 8;
  
private:
	std::string          mCode;
	TypePtr              mType;
	ArgVector						 mArgs;
	std::string          mSignature;
	mutable std::string  mPattern;
	int                  mPriority = 0;
	ArgConstIterator		 mOwnerArg;
	ArgConstIterator		 mPropArg;
};
std::ostream& operator<<(std::ostream& aStream, const Method& aValue);



