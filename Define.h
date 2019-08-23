#pragma once

#include "Method.h"

#include <list>
#include <ostream>

class Identifier;
class Define;

class IDefineHolder
{
public:
	virtual std::list<Define>& getDefines() = 0;
	virtual void addDefine(const Define& aDefine) = 0;
};
typedef std::shared_ptr<IDefineHolder> IDefineHolderPtr;

class Define : public Method
{
public:
	Define();
	virtual ~Define() = default;
  
	void createCodeCall();
	std::string createCodeDecl();

private:
};
std::ostream& operator<<(std::ostream& aStream, const Define& aValue);

