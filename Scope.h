#pragma once

#include "Container.h"
#include "Identifier.h"
#include "Define.h"
#include "Record.h"
#include "Type.h"

class Scope : public IIdentifiersHolder, public IRecordsHolder, public ITypesHolder, public IDefinesHolder
{
public:
	Scope(const std::string& aName = "");
	virtual ~Scope() = default;

	const std::string& name() const;
	
	// IIdentifiersHolder
	Identifiers& identifiers() override { return mIdentifiers; }
	std::string typeScopeName() const override { return ""; }
	
	// ITypesHolder
	Types& types() override { return mTypes; }

	// IDefinesHolder
	Defines& defines() override { return mDefines; }

	// IRecordHolder
	Records& records() override { return mRecords; }

private:					
	std::string					     mName;	
	Identifiers              mIdentifiers;		
	Types                    mTypes;		
	Records                  mRecords;							
	Defines                  mDefines;						
};    

typedef std::shared_ptr<Scope> ScopePtr;