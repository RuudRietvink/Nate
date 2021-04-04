#pragma once

#include "Arg.h"
#include "Expr.h"
#include "Type.h"
#include "WithFlags.h"

#include <string>
#include <vector>
#include <ostream>
#include <tuple>

class Identifier;
class Record;
class Object;
class Define;
typedef std::shared_ptr<Define> DefinePtr;
typedef std::shared_ptr<Object> ObjectPtr;

class Method;
typedef std::shared_ptr<Method> MethodPtr;

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

	virtual bool matches(const ExprNodesCIter& aBegin, const ExprNodesCIter& aEnd, int aDebug = 0) const;
	virtual MatchResult checkArgTypes(const ExprNodesCIter& aBegin, const ExprNodesCIter& aEnd, int aDebug = 0) const;
	virtual EvaluateResult createCode(const DefinePtr& aCurDefine, const ExprNodesCIter& aBegin, const ExprNodesCIter& aEnd, int aDebug = 0) const;
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
	ExprNodesCIter      getOwnerNode(const ExprNodesCIter& aNodeIter) const;
	Record*             getOwner(const ExprNodesCIter& aNodeIter) const;
	TypePtr             getTemplateType(const ExprNodesCIter& aNodeIter) const;
	bool						    isStatic() const;
	bool						    isObjectMethod() const;
	virtual bool		    isCodeMethod() const { return false; }
		
	void                setPriority(int aValue);
	void                setType(const TypePtr& aType);
	std::string         setFlagString(const std::string& aFlag) override;
	void                setObject(const ObjectPtr& aObject);
	const std::string&  code() const;
	std::string&        code();
	ObjectPtr           object() const;

	static const size_t Highest      = 1;
	static const size_t Num          = 2;
	static const size_t Same         = 3;
	static const size_t RightLeft    = 4;
	static const size_t None         = 5;
	static const size_t Last         = 6;
	static const size_t ConstExpr    = 7;
	static const size_t ConstMethod  = 8;
	static const size_t Defined      = 9;
	static const size_t Undeclared   =10;
	static const size_t Final        =11;
	static const size_t Overriden    =12;
	static const size_t Me           =13;
	static const size_t Static       =14;
  
private:
	void getTypes(
					const ExprNodesCIter& aBegin,
					Record* aOwner,
					TypePtr& aFirstType,
					TypePtr& aHighestType) const;
	void handleOwnerMember(
					const ExprNodesCIter& aNodeIter,
					Record* aOwner,
					const ExprNodesCIter& aOwnerNode,
					Method::EvaluateResult& aResult,
					std::string& aNodeCode,
					TypePtr& aNodeType) const;
	Expr createTypeCastNode(
					const ExprNodesCIter& aNodeIter,
					const Arg& aArg,
					const TypePtr& aTemplateType,
					const TypePtr& aFirstType,
					const TypePtr& aHighestType,
					std::string& aNodeCode) const;
	void createArgCode(
					const Arg& aArg,
					const Expr& aNode,
					const DefinePtr& aCurDefine,
					const std::string& aNodeCode,
					bool aIsObjectArg,
					// ->
					std::string& resultCode) const;

	std::string          mCode;
	TypePtr              mType;
	ArgVector						 mArgs;
	std::string          mSignature;
	mutable std::string  mPattern;
	int                  mPriority = 0;
	ObjectPtr            mObject;
	ArgConstIterator		 mObjectArg;
	ArgConstIterator		 mOwnerArg;
	ArgConstIterator		 mMemberArg;
	ArgConstIterator		 mTemplateArg;
	ArgConstIterator		 mTypenameArg;
};
std::ostream& operator<<(std::ostream& aStream, const Method& aValue);



