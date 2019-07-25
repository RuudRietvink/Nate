
#include "Arg.h"

Arg::Arg(const Identifier& aIdentifier)
	: mIdentifier(aIdentifier),
	  mIsIdentifier(true)
{}

Arg::Arg(const std::string& aWord)
	: mIdentifier("", Type()), 
	  mWord(aWord),
	  mIsIdentifier(false)
{}

bool               Arg::isIdentifier()    const { return mIsIdentifier; }
const Identifier&  Arg::identifier()      const { return mIdentifier; }
const std::string& Arg::word()            const { return mWord; }

bool Arg::setArgFlag(const std::string& aFlag)
{
  bool ok = true;

	if (aFlag == "num")
	{
		setFlag(Num, true);
	}
	else if (aFlag == "any")
	{
		setFlag(Any, true);
	}
	else if (aFlag == "cmp")
	{
		setFlag(Cmp, true);
	}
	else if (aFlag == "same")
	{
		setFlag(Same, true);
	}
	else if (aFlag == "out")
	{
		setFlag(Out, true);
	}
	else if (aFlag == "inout")
	{
		setFlag(Out, true);
		setFlag(InOut, true);
	}
  else
  {
    ok = false;
  }

  return ok;
}

std::ostream& operator<<(std::ostream& aStream, const Arg& aValue)
{
	aStream << "Arg(" << aValue.isIdentifier() << ",";
	if (aValue.isIdentifier())
	{
		aStream << aValue.identifier();
	}
	else
	{
		aStream << aValue.word();
	}

	if (aValue.is(Arg::Num)) aStream << ",Num";
	if (aValue.is(Arg::Any)) aStream << ",Any";
	if (aValue.is(Arg::Cmp)) aStream << ",Cmp";

	aStream << "}";

	return aStream;
}
