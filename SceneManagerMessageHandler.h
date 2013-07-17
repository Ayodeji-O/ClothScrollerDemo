//SceneManagerMessageHandler - Implementation for a class that contains a modular interface for
//the main GLUT message handler.

#include "GLUTInstanceMessageHandler.h"
#include "SceneManagerOpenGL.h"


class CSceneManagerMessageHandler : public CGLUTInstanceMessageHandler
{
public:
	CSceneManagerMessageHandler();
	~CSceneManagerMessageHandler();

	//Screen update routine for GLUT.
	void							GLUTHandleUpdate();
	//Keyboard message handler for GLUT.
	void							GLUTHandleKeyboard(unsigned char key, int mouseX,
		int mouseY);
	//Mouse message handler for GLUT.
	void							GLUTHandleMouse(int button, int buttonState, int mouseX,
		int mouseY);
	//Mouse drag hander for GLUT.
	void							GLUTHandleMotion(int mouseX, int mouseY);
	//Idle loop handler for GLUT.
	void							GLUTHandleIdle();

	//Determines whether or not a message has been received that corresponds to a user
	//exit request signal.
	bool							HasUserRequestedProgramExit() const;
protected:
	//Will be TRUE if a message has been received that indicates that the user
	//requested to exit the program.
	bool							mbExitRequestedFlag;
};

