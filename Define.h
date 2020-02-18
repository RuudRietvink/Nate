#pragma once

#include "Method.h"

#include <list>
#include <ostream>

class Identifier;
class Define;

typedef std::shared_ptr<Define> DefinePtr;

class Defines
{
public:
	virtual std::list<DefinePtr>& get();
	virtual void add(const DefinePtr& aDefine);
	virtual DefinePtr getLike(const DefinePtr& aDefine);

private:			
	std::list<DefinePtr> mDefines;			
};

class IDefinesHolder
{
public:
	virtual Defines& defines() = 0;
};
typedef std::shared_ptr<IDefinesHolder> IDefinesHolderPtr;

class Define : public Method
{
public:
	Define();
	virtual ~Define() = default;
  
	void createCodeCall();
	std::string createCodeDeclArgs(const std::vector<Arg>& aArgs);
	std::string createCodeDecl(const std::string& aObjectName = "");

private:
};
std::ostream& operator<<(std::ostream& aStream, const Define& aValue);

