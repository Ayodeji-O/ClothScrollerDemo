//SystemSupport.cpp - Contains operating system-specific routine implementations
#include "SystemSupport.h"

bool AssertExpression(const bool bAssertion)
{
#if _DEBUG 
	//Debug-build - call the assert macro in order to indicate a condition failure,
	//Return the value of the boolean experssion if no failure has occured.
	assert(bAssertion);
	return(bAssertion);
#else
	//Release-build - "Quietly" return the value of the condition.
	return(bAssertion);
#endif

}