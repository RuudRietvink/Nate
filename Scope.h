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
	Identifiers& identifiers() { return mIdentifiers; }
	
	// ITypesHolder
	Types& types() { return mTypes; }

	// IDefinesHolder
	Defines& defines() { return mDefines; }

	// IRecordHolder
	Records& records() { return mRecords; }

private:					
	std::string					     mName;	
	Identifiers              mIdentifiers;		
	Types                    mTypes;		
	Records                  mRecords;							
	Defines                  mDefines;						
};    

typedef std::shared_ptr<Scope> ScopePtr;