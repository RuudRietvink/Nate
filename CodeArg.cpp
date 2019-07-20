
#include "CodeArg.h"

CodeArg::CodeArg(const Identifier& aIdentifier)
	: mIdentifier(aIdentifier),
	  mIsIdentifier(true)
{}

CodeArg::CodeArg(const std::string& aWord)
	: mIdentifier("", Type()), 
	  mWord(aWord),
	  mIsIdentifier(false)
{}

bool               CodeArg::isIdentifier()    const { return mIsIdentifier; }
const Identifier&  CodeArg::identifier()      const { return mIdentifier; }
const std::string& CodeArg::word()            const { return mWord; }

void CodeArg::setArgFlag(const std::string& aFlag)
{
	if (aFlag == "Num")
	{
		setFlag(Num, true);
	}
	else if (aFlag == "Any")
	{
		setFlag(Any, true);
	}
	else if (aFlag == "Cmp")
	{
		setFlag(Cmp, true);
	}
	else if (aFlag == "Same")
	{
		setFlag(Same, true);
	}
}

std::ostream& operator<<(std::ostream& aStream, const CodeArg& aValue)
{
	aStream << "CodeArg(" << aValue.isIdentifier() << ",";
	if (aValue.isIdentifier())
	{
		aStream << aValue.identifier();
	}
	else
	{
		aStream << aValue.word();
	}

	if (aValue.is(CodeArg::Num)) aStream << ",Num";
	if (aValue.is(CodeArg::Any)) aStream << ",Any";
	if (aValue.is(CodeArg::Cmp)) aStream << ",Cmp";

	aStream << "}";

	return aStream;
}
