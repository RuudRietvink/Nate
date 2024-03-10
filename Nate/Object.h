#pragma once

#include "Identifier.h"
#include "Record.h"
#include "Type.h"
#include "Property.h"
#include "NateParser.tab.h"

#include <list>
#include <ostream>
#include <sstream>
#include <map>

namespace nate
{
class Object;
typedef std::shared_ptr<Object> ObjectPtr;

class Object : public Record, public IRecordsHolder, 
							 public ITypesHolder, public IDefinesHolder
{
public:
	Object();
	Object(const std::string& aName, const TypePtr& aBaseType = TypePtr());
	virtual ~Object() = default;
	
	bool isOfType(const std::string& aType) const override;
	bool basesOfType(const std::string& aType) const;

	// IIdentifiersHolder
	ScopeFlag scopeFlag() const override { return ScopeFlag::ObjectImpl; }
	IdentifierPtr getIdentifier(const std::string& aName) override;

	// ITypesHolder
	Types& types() override { return mTypes; }
	std::string typeScopeName() const override { return name(); }
	
	// IDefinesHolderPtr
	Defines& defines() override { return mDefines; }
	
	// IRecordsHolder
	Records& records() override { return mRecords; }

	std::stringstream& getImplOut();
	std::stringstream& getNormalOut();

	const std::vector<ObjectPtr>& getBases();
	void addBase(const ObjectPtr& aBase);
	DefinePtr basesGetLike(const DefinePtr& aDefine, const ObjectPtr& inheritsFromThis = ObjectPtr());
	
	struct PropState
	{
		enum class State
		{
			Unknown,
			Declared,
			Defined
		};

		State state;
		bool  overriden;

	};

	struct PropData
	{
		PropState states[2];
		std::string filename;
		nate::parser::location_type location;
	};
	
	std::map<IdentifierPtr, PropData>& propertyMethods() { return mPropertyMethods; }
	
	void     addProp(const IdentifierPtr& anId, const nate::parser::location_type& aLocation, const std::string& filename);
	bool     hasProp(const IdentifierPtr& anId) const;
	PropState getPropState(const IdentifierPtr& anId, Property::PropType aPropType) const;
	void     setPropState(const IdentifierPtr& anId, Property::PropType aPropType, PropState aPropState);
	bool     isPropDeclared(const IdentifierPtr& anId, Property::PropType aPropType) const;
	bool     isPropDefined(const IdentifierPtr& anId, Property::PropType aPropType) const;
	bool     basesIsPropDeclared(const IdentifierPtr& anId, Property::PropType aPropType) const;

	bool isRole() const;
	void setIsRole(bool aIsRole);

private:		
	bool															mIsRole = false;
	Types															mTypes;			
	Records														mRecords;						
	Defines														mDefines;
	std::vector<ObjectPtr>						mBases;
	std::stringstream									mImplOut;			
	std::stringstream									mNormalOut;	
	std::map<IdentifierPtr, PropData>	mPropertyMethods;
};

std::ostream& operator<<(std::ostream& aStream, const Method& aValue);

}
