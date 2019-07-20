#pragma once

#include "CodeArg.h"
#include "ExprNode.h"
#include "Type.h"
#include "WithFlags.h"

#include <string>
#include <vector>
#include <ostream>
#include <tuple>

class Identifier;

class Code : public WithFlags
{
public:
	Code();
	virtual ~Code() = default;

	bool matches(const std::string& aPattern) const;
	std::tuple<std::string, std::string, Type> evaluate(ExprNodesCIter& aBegin, ExprNodesCIter& aEnd) const;
	std::tuple<std::string, bool> checkArgTypes(ExprNodesCIter& aBegin, ExprNodesCIter& aEnd) const;
	void addCodeArgWord(const std::string& aWord);
	void addCodeArgId(const Identifier& aId);
	void addCodeStatWord(const std::string& aWord);
	void addCodeStatId(const std::string& aId);

	int                         priority() const;
	void                        setPriority(int aValue);
	const Type&                 type() const;
	void                        setType(const Type& aType);
	const std::vector<CodeArg>& codeArgs() const;
	std::vector<CodeArg>&       codeArgs();
	CodeArg&                    curCodeArg();
	const std::string&          codeStat() const;
	std::string&                codeStat();
	const std::string&          pattern() const;
	const std::string&          signature() const;
	void                        setReturnFlag(const std::string& aFlag);

	static const size_t Highest    = 1;
	static const size_t Num        = 2;
	static const size_t Same       = 3;
	static const size_t RightLeft  = 4;

private:
	int                  mPriority = 0;
	Type                 mType;
	std::vector<CodeArg> mCodeArgs;
	std::string          mCodeStat;
	std::string          mSignature;
	mutable std::string  mPattern;
	mutable bool         mIsRegex = false;
};
std::ostream& operator<<(std::ostream& aStream, const Code& aValue);

