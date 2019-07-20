#pragma once

#include <string>
#include <map>

class Identifier;

class Scope
{
public:
	Scope(const std::string& aName = "");
	virtual ~Scope() = default;

	const std::string& name() const;
	Identifier* getIdentifier(const std::string& aName);
	void addIdentifier(const Identifier& aIdentifier);

private:
	std::string mName;
	std::map<std::string, Identifier> mIdentifiers;
};