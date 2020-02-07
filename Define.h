#pragma once

#include "Method.h"

#include <list>
#include <ostream>

class Identifier;
class Define;

class Defines
{
public:
	virtual std::list<Define>& get();
	virtual void add(const Define& aDefine);
	virtual Define* getLike(const Define* aDefine);

private:			
	std::list<Define> mDefines;			
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

