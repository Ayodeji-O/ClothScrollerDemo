//SystemSupport.h - Contains operating system-specific includes / defines.

#ifndef	_SYSTEMSUPPORT_H
#define _SYSTEMSUPPORT_H			1

#include "assert.h"												//For debug-time assertions.
#include "windows.h"


//Validates a boolean expression, producing a failure notification in debug builds.
bool AssertExpression(const bool bAssertion);


//System-specific key assignments
namespace NSystemKeys
{
	const long	kEscapeKey =		VK_ESCAPE;
};


#endif	//#define _SYSTEMSUPPORT_H	1








