#include "C:\Users\ruud\source\repos\Nate\Nate\core\Core.h"
#include "C:\Users\ruud\source\repos\Nate\Nate\core\created\BaseObject.h"
#line 6 "Input.ns"
class Input: public BaseObject
{
public:
#line 7
	bool E_me__is_ok_() const
	{
#line 8
		return stream&&stream->good();
	}

private:
};

