#pragma once

#include "Container.h"
#include "Define.h"
#include "Record.h"
#include "Type.h"

class Scope : public IRecordHolder, public ITypeHolder, public IDefineHolder
{
public:
	Scope(const std::string& aName = "");
	virtual ~Scope() = default;

	const std::string& name() const;
	
	IdentifierPtr getIdentifier(const std::string& aName);
	void addIdentifier(const IdentifierPtr& aIdentifier);
	const std::list<IdentifierPtr>& getIdentifiers() const;
	
	// IDefineHolderPtr
	std::list<Define>& getDefines() override;
	void addDefine(const Define& aDefine) override;

	// ITypeHolder
	TypePtr getType(const std::string& aName) override;
	void addType(const TypePtr& aType, const std::string& aName = "") override;

	// IRecordHolder
	RecordPtr getRecord(const std::string& aName) override;
	void addRecord(RecordPtr& aRecord, const std::string& aName) override;

private:					
	std::string					     mName;								
	Container<IdentifierPtr> mIds;
	std::list<IdentifierPtr> mIdentifierList;	
	Container<RecordPtr>     mRecords;					
	Container<TypePtr>       mTypes;						
	std::list<Define>        mDefines;						
};

typedef std::shared_ptr<Scope> ScopePtr;