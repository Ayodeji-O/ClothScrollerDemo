//SceneManagerMessageHandler - Implementation for a class that contains a modular interface for
//the main GLUT message handler.

#include "SceneManagerMessageHandler.h"


CSceneManagerMessageHandler::CSceneManagerMessageHandler() : mbExitRequestedFlag(false)
{

}

CSceneManagerMessageHandler::~CSceneManagerMessageHandler()
{
}

//Screen update routine for GLUT.
void CSceneManagerMessageHandler::GLUTHandleUpdate()
{
	CSceneManagerOpenGL::ExecuteSceneManagerStep();
	//CSceneManagerOpenGL::RefreshOpenGLDisplay();
}

//Keyboard message handler for GLUT.
void CSceneManagerMessageHandler::GLUTHandleKeyboard(unsigned char key, int mouseX,
		int mouseY)
{
	if((key == ((unsigned char)NSystemKeys::kEscapeKey)) &&
		(!this->mbExitRequestedFlag)) {
		//Exit the program if the <ESC> key has been pressed.
		CSceneManagerOpenGL::ConcludeSceneManager();
		//(Indicate the the user has requested a program exit - this request will be handled
		//by the client body of code).
		this->mbExitRequestedFlag = true;
	}
}


//Mouse message handler for GLUT.
void CSceneManagerMessageHandler::GLUTHandleMouse(int button, int buttonState, int mouseX,
		int mouseY)
{

}

//Mouse drag hander for GLUT.
void CSceneManagerMessageHandler::GLUTHandleMotion(int mouseX, int mouseY)
{

}


void CSceneManagerMessageHandler::GLUTHandleIdle()
{
	CSceneManagerOpenGL::ExecuteSceneManagerStep();
}


bool CSceneManagerMessageHandler::HasUserRequestedProgramExit() const
{
	//Return the state of the exit request flag.
	return(this->mbExitRequestedFlag);
}
