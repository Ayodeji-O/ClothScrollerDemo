// Started January 16, 2007
// Main application file

// Portions of this application utilize files that 
// began development under the "Balloons" project:

// MemoryUtility.cpp
// GLUTOpenGLInstance.h
// GLUTOpenGLInstance.cpp
// GLUTInstanceMessageHandler.h
// BackgroundAudio.h
// BackgroundAudio.cpp
// SystemSupport.h
// SystemSupport.cpp
// SceneRepOpenGL.h
// SceneRepOpenGL.cpp
// SceneManagerOpenGL.h
// SceneManagerOpenGL.cpp
// SceneManagerMessageHandler.cpp
// SceneManagerMessageHandler.h
// RenderSupport.h
// PrimitiveSupport.h
// PrimitiveSupport.cpp
// PrimaryHeaders.h
// ModelGeometryOpenGL.h
// ModelGeometryOpenGL.cpp
// ModelGeometry.h
// ModelGeometry.cpp
// MemoryUtility.h

// [AAO, 1-16-2007]

#include "PrimaryHeaders.h"
#include "GLUTOpenGLInstance.h"
#include "SceneManagerMessageHandler.h"

#include "ScrollerManager.h"

#include "ClothScrollerScenes.h"

#include "ClothScrollerResources.h"
#include "BackgroundAudio.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine, int nCmdShow)
{
	const QuantityType				kScreenWidth = 640;
	const QuantityType				kScreenHeight = 480;

	//Scene duration (in milliseconds).
	const QuantityType				kSceneDuration = 25000;

	//Scene transition duration (in milliseconds).
	const QuantityType				kSceneTransitionDuration = 5000;

	//Scene update rate (in milliseconds).
	const QuantityType				kSceneUpdateRate = (QuantityType) (1000.0 / 60.0);

	// Width of scroller image/texture (pixels).
	const QuantityType				kScrollerImageWidth = 256;

	// Height of scroller image/texture (pixels).
	const QuantityType				kScrollerImageHeight = 64;

	// Scrolling rate for scroller (pixels/step).
	const QuantityType				kScrollerRate = 2;

	// Initialize the scroller manager.
	CScrollerManager::InitializeScrollerManager(std::string(kClothScrollerTextMessageFileName), 
		kScrollerImageWidth, kScrollerImageHeight, kScrollerRate);

	// Create the cloth scroller scenes, and add them to the scene manager.
	CSceneScrollerFlag				sceneScrollerFlag;
	CSceneScrollerTeatheredCloth	sceneScrollerTeatheredCloth;
	CSceneScrollerTapestry			sceneScrollerTapestry;
	CSceneScrollerRisingCloth		sceneScrollerRisingCloth;
	CSceneScrollerTrampoline		sceneScrollerTrampoline;

	CSceneManagerOpenGL::AddScene(&sceneScrollerFlag);
	CSceneManagerOpenGL::AddScene(&sceneScrollerTeatheredCloth);
	CSceneManagerOpenGL::AddScene(&sceneScrollerTapestry);
	CSceneManagerOpenGL::AddScene(&sceneScrollerRisingCloth);
	CSceneManagerOpenGL::AddScene(&sceneScrollerTrampoline);

	// Set the scene transition durations and scene duration values.
	CSceneManagerOpenGL::SetSceneDuration(kSceneDuration);
	CSceneManagerOpenGL::SetTransitionDuration(kSceneTransitionDuration);

	// Set the scene update rate (delay between successive frame updates).
	CSceneManagerOpenGL::SetUpdateRate(kSceneUpdateRate);

	// ...Activate the scene manager (scenes will begin execution
	// when GLUT is initialized).
	CSceneManagerOpenGL::StartSceneManager();

	// Create a scene manager message handler object (this adapts a compliant
	// CGLUTOpenGLInstance for the scene manager).
	CSceneManagerMessageHandler		sceneManagerMessageHandler;

	// Load the background music, and begin playing the music.
	const std::string				musicFileName(kClothScrollerMusicFileName);
	CBackgroundAudio				backgroundMusic(musicFileName);
	backgroundMusic.PlayAudio();

	//Create a GLUT OpenGL instance - this process intializes GLUT, and activates
	//message handlers - the program loop begins at this point.
	CGLUTOpenGLInstance				glutOpenGLInstance(&sceneManagerMessageHandler,
		kScreenWidth, kScreenHeight, true);

	return 0;
}
