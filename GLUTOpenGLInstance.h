//GLUTOpenGLInstance.h - Contains declarations for main class that handles GLUT/OpenGL
//initialization and message handling.

#ifndef _GLUTOPENGLINSTANCE_H
#define _GLUTOPENGLINSTANCE_H 1
#include "PrimaryHeaders.h"
#include "PrimitiveSupport.h"
#include "GLUTInstanceMessageHandler.h"


class CGLUTOpenGLInstance
{
public:
	CGLUTOpenGLInstance(CGLUTInstanceMessageHandler* pInstanceMessageHandler,
		const QuantityType windowWidth, const QuantityType windowHeight,
		const bool bFullScreen = false);
	~CGLUTOpenGLInstance();

	//Concludes GLUT, removing message handlers, and destroying the created window.
	static bool						ConcludeGLUT();

private:
	//Routine called by GLUT in order to update the OpenGL display.
	static void						GLUTUpdateRoutine();
	//Routine called by GLUT when a keyboard message is generated.
	static void						GLUTKeyboardRoutine(unsigned char key, int mouseX, int mouseY);
	//Routine called by GLUT when a mouse message is generated.
	static void						GLUTMouseRoutine(int button, int buttonState, int mouseX,
		int mouseY);
	//Routine called by GLUT when the mouse is moved while a button is depressed.
	static void						GLUTMotionRoutine(int mouseX, int mouseY);
	//Routine called by GLUT when the event loop is idle.
	static void						GLUTIdleRoutine();

	//Modular GLUT instance message handler class.
	static CGLUTInstanceMessageHandler*
									mpGLUTInstanceMessageHandler;

	//GLUT window reference
	static int						mGLUTWindow;
};

#endif	//#ifndef _GLUTOPENGLINSTANCE_H


