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
	
	// IIdentifiersHolder
	ScopeFlag scopeFlag() const override { return ScopeFlag::ObjectImpl; }

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
	
	enum class PropType
	{
		Get = 0,
		Set = 1
	};

	enum class PropState
	{
		Unknown,
		Declared,
		Defined
	};

	struct PropData
	{
		PropState states[2];
	};
	
	std::map<IdentifierPtr, PropData>& propertyMethods() { return mPropertyMethods; }
	
	PropState getPropState(const IdentifierPtr& anId, PropType aPropType) const;
	void     setPropState(const IdentifierPtr& anId, PropType aPropType, PropState aPropState);
	bool     isPropDeclared(const IdentifierPtr& anId, PropType aPropType) const;
	bool     isPropDefined(const IdentifierPtr& anId, PropType aPropType) const;

private:		
	Types															mTypes;			
	Records														mRecords;						
	Defines														mDefines;
	ObjectPtr													mBase;
	std::stringstream									mImplOut;			
	std::stringstream									mNormalOut;	
	std::map<IdentifierPtr, PropData>	mPropertyMethods;
};

std::ostream& operator<<(std::ostream& aStream, const Method& aValue);