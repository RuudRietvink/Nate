#pragma once

#include "ObjectBase.h"

class Role : public ObjectBase
{
public:
	using ObjectBase::ObjectBase;					
};

typedef std::shared_ptr<Role> RolePtr;
