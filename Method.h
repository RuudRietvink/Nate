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

	virtual bool matches(const std::string& aPattern) const;
	virtual std::tuple<std::string, bool> checkArgTypes(ExprNodesCIter& aBegin, ExprNodesCIter& aEnd) const;
	virtual std::tuple<std::string, std::string, Type> evaluate(ExprNodesCIter& aBegin, ExprNodesCIter& aEnd) const = 0;
	virtual void addArgWord(const std::string& aWord);
	virtual void addArgId(const Identifier& aId);
	virtual std::string toCodeWord(const std::string& aWord) const;
	
	int                         priority() const;
	void                        setPriority(int aValue);
	const Type&                 type() const;
	void                        setType(const Type& aType);
	const std::vector<Arg>&     args() const;
	std::vector<Arg>&           args();
	Arg&                        curArg();
	const std::string&          pattern() const;
	const std::string&          signature() const;
	void                        setReturnFlag(const std::string& aFlag);

	static const size_t Highest    = 1;
	static const size_t Num        = 2;
	static const size_t Same       = 3;
	static const size_t RightLeft  = 4;
	static const size_t None       = 5;

private:
	Type                 mType;
	std::vector<Arg>     mArgs;
	std::string          mSignature;
	mutable std::string  mPattern;
	mutable bool         mIsRegex = false;
	int                  mPriority = 0;
};
std::ostream& operator<<(std::ostream& aStream, const Method& aValue);



