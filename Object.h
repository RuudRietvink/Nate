#pragma once

#include "Identifier.h"
#include "Record.h"
#include "Define.h"
#include "Type.h"

#include <list>
#include <ostream>
#include <sstream>

class Object;
typedef std::shared_ptr<Object> ObjectPtr;

class Object : public IIdentifiersHolder, public Type, public IRecordsHolder, 
							 public ITypesHolder, public IDefinesHolder
{
public:
	Object();
	Object(const std::string& aType, const TypePtr& aBaseType = TypePtr());
	virtual ~Object() = default;
	
	// IIdentifiersHolder
	Identifiers& identifiers() { return mIdentifiers; }

	// ITypesHolder
	Types& types() { return mTypes; }
	
	// IDefinesHolderPtr
	Defines& defines() { return mDefines; }
	
	// IRecordsHolder
	Records& records() { return mRecords; }

	std::stringstream& getImplOut();
	std::stringstream& getNormalOut();

	ObjectPtr getBase();
	void setBase(const ObjectPtr& aBase);

private:	
	Identifiers              mIdentifiers;		
	Types                    mTypes;			
	Records                  mRecords;						
	Defines                  mDefines;
	ObjectPtr                mBase;
	std::stringstream        mImplOut;			
	std::stringstream        mNormalOut;					
};

std::ostream& operator<<(std::ostream& aStream, const Method& aValue);