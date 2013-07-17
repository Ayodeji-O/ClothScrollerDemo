//SceneManagerOpenGL.h - Contains declarations pertinent to the OpenGL scene execution manager.

#ifndef _SCENEMANAGEROPENGL_H
#define _SCENEMANAGEROPENGL_H 1

#include "PrimaryHeaders.h"
#include "SceneRepOpenGL.h"


class CSceneManagerOpenGL
{
public:
	//Executes a scene manager iteration.
	static void						ExecuteSceneManagerStep();

	//Adds a scene to the scene manager.
	static bool						AddScene(CSceneOpenGL* pSceneOpenGL);

	//Sets the scene duration (independent of transition duration).
	static void						SetSceneDuration(const QuantityType sceneDuration);

	//Sets the scene transition duration (independent of scene duration).
	static void						SetTransitionDuration(const QuantityType transitionDuration);

	//Sets the target/maximum scene refresh rate.
	static void						SetUpdateRate(const QuantityType updateRate);

	//Initializes the scene manager, and commences scene manager execution.
	static bool						StartSceneManager();

	//Ends scene manager functionality.
	static bool						ConcludeSceneManager();


protected:
	//Only one scene manager object can exist.
	CSceneManagerOpenGL();
	~CSceneManagerOpenGL();

	//Executes a render step for the current scene (rendering is not necessarily displayed
	//immediately).
	static bool						ExecuteCurrentSceneStep(const QuantityType currentSceneTime);

	//Updates the OpenGL camera in accorance with the current scene.
	static bool						UpdateSceneCamera(const CSceneCameraOpenGL& sceneCamera);

	//Refreshes the OpenGL buffer, effectively rendering the scene.
	static bool						RefreshOpenGLDisplay();

	//Initializes the current scene, and begins scene execution.
	static bool						StartCurrentScene();

	//Attempts to initiate a transition for the current scene.
	static bool						BeginCurrentSceneTransition(ESceneState eSceneTransition);

	//Progresses on to the next scene.
	static bool						IncrementScene();

	//"Locks" scene manager data, allowing exclusive access for a particular
	//scene manager routine.
	static bool						LockSceneManagerData();

	//"Unlocks" scene manager data, permitting all scene manager routines
	//to access data.
	static bool						UnlockSceneManagerData();

	//Determines if scene manager data is locked.
	static bool						IsSceneManagerDataLocked();

	//Determines if the scene manager is initialized, and if all functionality
	//is immediately available.
	static bool						IsSceneManagerReady();

	//Obtains a count from the sytem timer (milliseconds).
	static QuantityType				GetCurrentClockCount();

	//Computes an event duration (milliseconds) from a provided starting
	//system timer count (past event).
	static QuantityType				ComputeCountDuration(const QuantityType startCount);

	//"Base count" - indicates time scene manager was started (used to calculate time
	//.
	static QuantityType				mSceneManagerBaseTickCount;

	//Maximum scene duration, in milliseconds.
	static QuantityType				mSceneDuration;

	//Maximum transition duration, in milliseconds.
	static QuantityType				mTransitionDuration;

	//Current scene start time - will be used to determine execution time count (milliseconds).
	static QuantityType				mCurrentSceneStartTime;

	//Current transition start time - will be used to determine execution time count
	//(millisectonds).
	static QuantityType				mCurrentTransitionStartTime;

	//Maximum/target scene update rate (milliseconds / frame - reciprocal of frame rate).
	static QuantityType				mSceneUpdateRate;

	//Time reference used to compute time since last scene refresh (milliseconds).
	static QuantityType				mSceneUpdateTimeReference;

	//Type definition - list of OpenGL scenes.
	typedef std::vector<CSceneOpenGL*>
									SceneList;

	//Internal list of OpenGL scenes.
	static SceneList				mSceneList;

	//Current scene being executed in the scene list.
	static SceneList::iterator		mCurrentScene;

	//Scene manager data lock flag (mutex).
	static bool						mbSceneManagerDataLockFlag;

	//Scene manager initialization flag.
	static bool						mbSceneManagerInitialized;
};


#endif	//_SCENEMANAGEROPENGL_H