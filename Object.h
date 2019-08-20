#pragma once

#include "Record.h"
#include "Define.h"
#include "Type.h"
#include "Container.h"

#include <list>

class Object : public Type, public IRecordHolder, public ITypeHolder
{
public:
	using Type::Type;

	const std::list<Define>& getDefines() const;
	void addDefine(const Define& aDefine);
	
	// IRecordHolder
	RecordPtr getRecord(const std::string& aName) override;
	void addRecord(RecordPtr& aRecord, const std::string& aName) override;

	// ITypeHolder
	TypePtr getType(const std::string& aName) override;
	void addType(const TypePtr& aType, const std::string& aName = "") override;

private:
	::Container<RecordPtr>   mRecords;				
	::Container<TypePtr>     mTypes;							
	std::list<Define>        mDefines;					
};

typedef std::shared_ptr<Object> ObjectPtr;