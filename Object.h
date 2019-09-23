#pragma once

#include "Identifier.h"
#include "Record.h"
#include "Define.h"
#include "Type.h"

#include <list>
#include <ostream>
#include <sstream>
#include <map>

class Object;
typedef std::shared_ptr<Object> ObjectPtr;

class Object : public Record, public IRecordsHolder, 
							 public ITypesHolder, public IDefinesHolder
{
public:
	Object();
	Object(const std::string& aName, const TypePtr& aBaseType = TypePtr());
	virtual ~Object() = default;
	
	// ITypesHolder
	Types& types() override { return mTypes; }
	std::string typeScopeName() const override { return name(); }
	
	// IDefinesHolderPtr
	Defines& defines() override { return mDefines; }
	
	// IRecordsHolder
	Records& records() override { return mRecords; }

	std::stringstream& getImplOut();
	std::stringstream& getNormalOut();

	ObjectPtr getBase();
	void setBase(const ObjectPtr& aBase);
	std::multimap<IdentifierPtr, std::string>& propertyMethods() { return mPropertyMethods; }

private:		
	Types                    mTypes;			
	Records                  mRecords;						
	Defines                  mDefines;
	ObjectPtr                mBase;
	std::stringstream        mImplOut;			
	std::stringstream        mNormalOut;	
	std::multimap<IdentifierPtr, std::string> mPropertyMethods;
};

std::ostream& operator<<(std::ostream& aStream, const Method& aValue);