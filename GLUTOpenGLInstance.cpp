//GLUTOpenGLInstance.cpp - Contains implemenations for main class that handles GLUT/OpenGL
//initialization and message handling.

#include "GLUTOpenGLInstance.h"


CGLUTOpenGLInstance::CGLUTOpenGLInstance(CGLUTInstanceMessageHandler* pInstanceMessageHandler,
										 const QuantityType windowWidth,
										 const QuantityType windowHeight,
										 const bool bFullScreen)
{
	if(::AssertExpression(pInstanceMessageHandler != NULL)) {
		CGLUTOpenGLInstance::mpGLUTInstanceMessageHandler = pInstanceMessageHandler;

		int							numCommandLineArguments = 1;

		LPWSTR						pWCommandLine = (LPWSTR)::GetCommandLine();

		::CommandLineToArgvW(pWCommandLine, &numCommandLineArguments);


		//Initialize GLUT...
		::glutInit(&numCommandLineArguments, (char**)&pWCommandLine);

		//GLUT display mode options are as follows:
		//GLUT_RGBA             								
		//GLUT_RGB
		//GLUT_INDEX
		//GLUT_SINGLE
		//GLUT_DOUBLE
		//GLUT_ACCUM
		//GLUT_ALPHA
		//GLUT_DEPTH
		//GLUT_STENCIL
		//GLUT_MULTISAMPLE
		//GLUT_STEREO
		//GLUT_LUMINANCE

		//Default display mode will be RGBA, double-buffered, with a depth buffer.
		const unsigned int			kGLUTDisplayMode = GLUT_RGBA | GLUT_DOUBLE |
			GLUT_DEPTH;

		//Set the proper display mode using GLUT.
		::glutInitDisplayMode(kGLUTDisplayMode);
                 

		//Set the window size and position using GLUT
		::glutInitWindowSize(windowWidth, windowHeight);                                                                                                                                                                                              
		::glutInitWindowPosition(0, 0);

		//Create the window...
		this->mGLUTWindow = ::glutCreateWindow("");

		if(bFullScreen) {
			//Create a full screen window if such a window has been requested.
			::glutFullScreen();
		}

		//
		////
		//Init...
		::glClearColor(0.0, 0.0, 0.0, 0.0);

		::glMatrixMode(GL_PROJECTION);
		::gluPerspective(90.0, 4.0/3.0, 0.0, 500.0);
		::glLoadIdentity();
		::glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);


		//Enable back-face culling.
		//::glEnable(GL_CULL_FACE);
		//::glCullFace(GL_BACK);

		//Enable Z-buffer.
		::glEnable(GL_DEPTH_TEST);
		::glClearDepth(1.0);

		// Enable light shading.
		::glEnable(GL_LIGHTING);
		////
		//

		//
		////
		//!!!
		::glEnable(GL_LIGHT0);
		::glShadeModel(GL_SMOOTH);
		////
		//


		//
		////!!!
		// Enable texture mapping
		::glEnable(GL_TEXTURE_2D);
		////
		//

		//
		////
		//!!!
		//Add a light (however the light should be property of the scene).
		::glEnable(GL_LIGHT0);
		GLfloat	light_specular[] = {(GLfloat)0.85, (GLfloat)0.85, (GLfloat)0.85, (GLfloat)1.0};
		GLfloat	light_ambient[] = {(GLfloat)0.1, (GLfloat)0.1, (GLfloat)0.1, (GLfloat)1.0};
		GLfloat	light_diffuse[] = {(GLfloat)0.95, (GLfloat)0.95, (GLfloat)0.95, (GLfloat)1.0};
		GLfloat	light_emissive[] = {(GLfloat)0.0, (GLfloat)0.0, (GLfloat)0.0, (GLfloat)1.0};
		GLfloat	light_position[] = {(GLfloat)0.0, (GLfloat)3.0, (GLfloat)1.0, (GLfloat)0.9};
		::glLightfv(GL_LIGHT0, GL_AMBIENT, light_emissive/*light_ambient*/);
		::glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse/*light_diffuse*/);
		::glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
		::glLightfv(GL_LIGHT0, GL_EMISSION, light_emissive);
		::glLightfv(GL_LIGHT0, GL_POSITION, light_position);

		////
		//

		//Set GLUT display update handler.
		::glutDisplayFunc(CGLUTOpenGLInstance::GLUTUpdateRoutine);
		//Set GLUT keyboard message handler.
		::glutKeyboardFunc(CGLUTOpenGLInstance::GLUTKeyboardRoutine);
		//Set GLUT mouse message handler.
		::glutMouseFunc(CGLUTOpenGLInstance::GLUTMouseRoutine);
		//Set GLUT mouse drag hander.
		::glutMotionFunc(CGLUTOpenGLInstance::GLUTMotionRoutine);
		
		::glutIdleFunc(CGLUTOpenGLInstance::GLUTUpdateRoutine);

		//Begin GLUT main loop.
		::glutMainLoop();

		//Initialize the OpenGL drawing region.
		

		////
		//
	}

}


bool CGLUTOpenGLInstance::ConcludeGLUT()
{
	bool							bGLUTConcludedSuccessfully = false;

	if(CGLUTOpenGLInstance::mGLUTWindow) {
		//Clear the glut message handlers
		::glutDisplayFunc(NULL);
		::glutKeyboardFunc(NULL);
		::glutMouseFunc(NULL);
		::glutMotionFunc(NULL);
		::glutIdleFunc(NULL);


		//Destroy the GLUT window.
		::glutDestroyWindow(CGLUTOpenGLInstance::mGLUTWindow);

		bGLUTConcludedSuccessfully = true;
	}

	return(bGLUTConcludedSuccessfully);
}


void CGLUTOpenGLInstance::GLUTUpdateRoutine()
{
	if(CGLUTOpenGLInstance::mpGLUTInstanceMessageHandler) {
		//Call the provided GLUT update message handler.
		CGLUTOpenGLInstance::mpGLUTInstanceMessageHandler->GLUTHandleUpdate();
	}
}


void CGLUTOpenGLInstance::GLUTKeyboardRoutine(unsigned char key, int mouseX, int mouseY)
{
	if(CGLUTOpenGLInstance::mpGLUTInstanceMessageHandler) {
		//Call the provided GLUT keyboard message handler.
		CGLUTOpenGLInstance::mpGLUTInstanceMessageHandler->GLUTHandleKeyboard(key,
			mouseX, mouseY);

		//End GLUT execution if the user has requested a program exit.
		if(CGLUTOpenGLInstance::mpGLUTInstanceMessageHandler->HasUserRequestedProgramExit()) {
			CGLUTOpenGLInstance::ConcludeGLUT();
		}
	}
}

void CGLUTOpenGLInstance::GLUTMouseRoutine(int button, int buttonState, int mouseX, int mouseY)
{
	if(CGLUTOpenGLInstance::mpGLUTInstanceMessageHandler) {
		//Call the provided GLUT mouse routine handler.
		CGLUTOpenGLInstance::mpGLUTInstanceMessageHandler->GLUTHandleMouse(button,
			buttonState, mouseX, mouseY);

		//End GLUT execution if the user has requested a program exit.
		if(CGLUTOpenGLInstance::mpGLUTInstanceMessageHandler->HasUserRequestedProgramExit()) {
			CGLUTOpenGLInstance::ConcludeGLUT();
		}
	}
}

void CGLUTOpenGLInstance::GLUTMotionRoutine(int mouseX, int mouseY)
{
	if(CGLUTOpenGLInstance::mpGLUTInstanceMessageHandler) {
		//Call the provided GLUT mouse drag routine handler.
		CGLUTOpenGLInstance::mpGLUTInstanceMessageHandler->GLUTHandleMotion(mouseX,
			mouseY);
	}
}

void CGLUTOpenGLInstance::GLUTIdleRoutine()
{
	if(CGLUTOpenGLInstance::mpGLUTInstanceMessageHandler) {
		//Call the provided GLUT idle routine handler.
		CGLUTOpenGLInstance::mpGLUTInstanceMessageHandler->GLUTHandleIdle();
	}

}

CGLUTOpenGLInstance::~CGLUTOpenGLInstance()
{

}

//CGLUTOpenGLInstance static member function instantiation section.
CGLUTInstanceMessageHandler*		CGLUTOpenGLInstance::mpGLUTInstanceMessageHandler = NULL;
int									CGLUTOpenGLInstance::mGLUTWindow = 0;