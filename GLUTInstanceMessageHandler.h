//GLUTInstanceMessageHandler.h - Defines an interface for a message handler class to be used with
//CGLUTOpenGLInstance

#ifndef _GLUTINSTANCEMESSAGEHANDLER_H
#define _GLUTINSTANCEMESSAGEHANDLER_H 1

class CGLUTInstanceMessageHandler
{

public:
	CGLUTInstanceMessageHandler() {}
	~CGLUTInstanceMessageHandler() {}

	//Screen update routine for GLUT.
	virtual void					GLUTHandleUpdate() = 0;
	//Keyboard message handler for GLUT.
	virtual void					GLUTHandleKeyboard(unsigned char key, int mouseX,
		int mouseY) = 0;
	//Mouse message handler for GLUT.
	virtual void					GLUTHandleMouse(int button, int buttonState, int mouseX,
		int mouseY) = 0;
	//Mouse drag hander for GLUT.
	virtual void					GLUTHandleMotion(int mouseX, int mouseY) = 0;
	//Idle loop handler for GLUT.
	virtual void					GLUTHandleIdle() = 0;

	//Determines whether or not a message has been received that indicates that the user has
	//requested to exit the program 
	virtual bool					HasUserRequestedProgramExit() const = 0;
protected:
};


#endif //#ifndef _GLUTINSTANCEMESSAGEHANDLER_H