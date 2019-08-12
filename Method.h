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

	struct MatchResult
	{
		bool matches = true;
		std::string error;
		int32_t castCount = 0;
	};
	
	struct EvaluateResult
	{
		std::string error;
		std::string code;
		std::string origText;
		TypePtr type;
		Flags flags;
	};

	virtual bool matches(const ExprNodesCIter& aBegin, const ExprNodesCIter& aEnd, bool aDebug = false) const;
	virtual MatchResult checkArgTypes(const ExprNodesCIter& aBegin, const ExprNodesCIter& aEnd, bool aDebug = false) const;
	virtual EvaluateResult evaluate(const ExprNodesCIter& aBegin, const ExprNodesCIter& aEnd, bool aDebug = false) const;
	virtual void addArgWord(const std::string& aWord);
	virtual void addArgId(const IdentifierPtr& aId);
	virtual std::string toCodeWord(const std::string& aWord) const;
	virtual void endDecl();
	
	void copyFrom(const Method& aMethod);

	using ArgVector        = std::vector<Arg>;
	using ArgConstIterator = ArgVector::const_iterator;

	int                 priority() const;
  const TypePtr&      type() const;
	const ArgVector&    args() const;
	ArgVector&          args();
	Arg&                curArg();
	const std::string&	pattern() const;
	const std::string&  signature() const;
	Record*             getOwner(const ExprNodesCIter& aNodeIter) const;
	TypePtr             getTemplateType(const ExprNodesCIter& aNodeIter) const;
		
	void                setPriority(int aValue);
	void                setType(const TypePtr& aType);
	void                setReturnFlag(const std::string& aFlag);
	const std::string&  code() const;
	std::string&        code();

	static const size_t Highest      = 1;
	static const size_t Num          = 2;
	static const size_t Same         = 3;
	static const size_t RightLeft    = 4;
	static const size_t None         = 5;
	static const size_t Last         = 6;
	static const size_t ConstExpr    = 7;
  
private:
	std::string          mCode;
	TypePtr              mType;
	ArgVector						 mArgs;
	std::string          mSignature;
	mutable std::string  mPattern;
	int                  mPriority = 0;
	ArgConstIterator		 mOwnerArg;
	ArgConstIterator		 mPropArg;
	ArgConstIterator		 mTemplateArg;
	ArgConstIterator		 mTypenameArg;
};
std::ostream& operator<<(std::ostream& aStream, const Method& aValue);



