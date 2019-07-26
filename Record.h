#pragma once
#include "Type.h"
#include "Identifier.h"

#include <list>

class Scope;

class Record : public Type
{
public:
	Record() = default;
	Record(const std::string& aName);
	virtual ~Record() = default;

	std::shared_ptr<Scope>& scope();

	IdentifierPtr getIdentifier(const std::string& aName);
	void addIdentifier(const IdentifierPtr& aIdentifier);
	const std::list<IdentifierPtr>& getIdentifiers() const;
	virtual std::ostream& print(std::ostream& aStream) const override;

private:
	std::shared_ptr<Scope>	mScope;
};

typedef std::shared_ptr<Record> RecordPtr;
