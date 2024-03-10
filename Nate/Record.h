#pragma once
#include "Type.h"
#include "Identifier.h"

#include <list>

namespace nate
{
class Record;
typedef std::shared_ptr<Record> RecordPtr;

class Record : public Type, public IIdentifiersHolder
{
public:
	Record() = default;
	Record(const std::string& aName, const TypePtr& aBaseType = TypePtr());
	virtual ~Record() = default;
		
	// IIdentifiersHolder
	Identifiers& identifiers() override { return mIdentifiers; }
	ScopeFlag scopeFlag() const override { return ScopeFlag::Member; }
	IdentifierPtr getIdentifier(const std::string& aName) override { return mIdentifiers.get(aName); }
	
	const std::string& name() const override { return mName; }
	virtual std::ostream& print(std::ostream& aStream) const override;

private:
	std::string					    mName;	
	Identifiers             mIdentifiers;
};

class Records
{
public:
	Records(Types& aTypes);
	virtual RecordPtr get(const std::string& aName);
	virtual void add(RecordPtr& aRecord, const std::string& aName);

private:			
	Container<RecordPtr>       mRecords;	
	Types&                     mTypes;			
};

class IRecordsHolder
{
public:
	virtual Records& records() = 0;
};
typedef std::shared_ptr<IRecordsHolder> IRecordsHolderPtr;

}
