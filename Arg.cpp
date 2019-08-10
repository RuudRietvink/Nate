
#include "Arg.h"

Arg::Arg(const IdentifierPtr& aIdentifier)
	: mIdentifier(aIdentifier),
	  mIsIdentifier(true)
{}

Arg::Arg(const std::string& aWord)
	: mIdentifier(), 
	  mWord(aWord),
	  mIsIdentifier(false)
{}

bool                  Arg::isIdentifier()    const { return mIsIdentifier; }
const IdentifierPtr&  Arg::identifier()      const { return mIdentifier; }
const std::string&    Arg::word()            const { return mWord; }

bool Arg::setArgFlag(const std::string& aFlag)
{
  bool ok = true;

	if (aFlag == "cmp")
	{
		setFlag(Cmp, true);
	}
	else if (aFlag == "same")
	{
		setFlag(Same, true);
	}
	else if (aFlag == "comphigh")
	{
		setFlag(CompHigh, true);
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
	else if (aFlag == "owner")
	{
		setFlag(Owner, true);
	}
	else if (aFlag == "prop")
	{
		setFlag(Prop, true);
	}
	else if (aFlag == "template")
	{
		setFlag(Template, true);
	}
	else if (aFlag == "typename")
	{
		setFlag(Typename, true);
	}
	else if (aFlag == "exact")
	{
		setFlag(Exact, true);
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
		aStream << *aValue.identifier();
	}
	else
	{
		aStream << aValue.word();
	}

	if (aValue.is(Arg::Cmp)) aStream << ",Cmp";
	if (aValue.is(Arg::Same)) aStream << ",Same";
	if (aValue.is(Arg::CompHigh)) aStream << ",CompHigh";
	if (aValue.is(Arg::Out)) aStream << ",Out";
	if (aValue.is(Arg::InOut)) aStream << ",InOut";
	if (aValue.is(Arg::Owner)) aStream << ",Owner";
	if (aValue.is(Arg::Prop)) aStream << ",Prop";
	if (aValue.is(Arg::Exact)) aStream << ",Exact";
	if (aValue.is(Arg::Typename)) aStream << ",Typename";
	if (aValue.is(Arg::Template)) aStream << ",Template";

	aStream << "}";

	return aStream;
}
