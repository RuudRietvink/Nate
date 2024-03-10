#include "Record.h"
#include "Scope.h"

#include "NateFunctions.h"

namespace nate
{
Record::Record(const std::string& aName, const TypePtr& aBaseType)
	: Type("record", aBaseType),
	  mName(aName)
{
	setCodeType(toCodeName(aName));
}

std::ostream& Record::print(std::ostream& aStream) const
{
	aStream << "Record(";
	Type::print(aStream);
	aStream << "Identifiers(";
	for (auto id : mIdentifiers.get())
	{
		aStream << *id << ",";
	}
	aStream << ")";
	aStream << ")";
	return aStream;
}

Records::Records(Types& aTypes)
	: mTypes(aTypes)
{}

RecordPtr Records::get(const std::string& aName)
{
	return mRecords.getData(aName);
}

void Records::add(RecordPtr& aRecord, const std::string& aName)
{
	aRecord->setFlag(Type::Record, true);
	mRecords.addData(aRecord);
	mTypes.add(aRecord, aName);
}

}
