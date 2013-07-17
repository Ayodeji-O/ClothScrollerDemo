//SceneManagerOpenGL.cpp - Contains the OpenGL scene manager implementation

#include "SceneManagerOpenGL.h"




//Executes a scene manager iteration.
void CSceneManagerOpenGL::ExecuteSceneManagerStep()
{
	if(CSceneManagerOpenGL::mCurrentScene &&
		(CSceneManagerOpenGL::mCurrentScene != CSceneManagerOpenGL::mSceneList.end()) &&
		*CSceneManagerOpenGL::mCurrentScene)
	{
		//Start the current scene if it has not been initialized (this process currently needs
		//to occur after GLUT has been initialized, so it occurs here, in the event loop).
		if(!(*CSceneManagerOpenGL::mCurrentScene)->IsSceneInitialized()) {
			CSceneManagerOpenGL::StartCurrentScene();
		}

		const QuantityType			currentSceneTime =
			CSceneManagerOpenGL::ComputeCountDuration(CSceneManagerOpenGL::mCurrentSceneStartTime);

		//Determine if the current scene should be switched...
		if(currentSceneTime < CSceneManagerOpenGL::mSceneDuration) {
			//Enforce the update rate limit (frame rate limiter).
			if(CSceneManagerOpenGL::ComputeCountDuration(CSceneManagerOpenGL::mSceneUpdateTimeReference) >
				CSceneManagerOpenGL::mSceneUpdateRate)
			{
				//Set the scene update time reference.
				CSceneManagerOpenGL::mSceneUpdateTimeReference =
					CSceneManagerOpenGL::GetCurrentClockCount();
				//Execute a scene step.
				CSceneManagerOpenGL::ExecuteCurrentSceneStep(currentSceneTime);
			}

			//Update the OpenGL camera.
			//CSceneManagerOpenGL::UpdateSceneCamera(
			//	(*CSceneManagerOpenGL::mCurrentScene)->GetSceneCamera());

			//Render the scene.
			//(*CSceneManagerOpenGL::mCurrentScene)->RenderScene();
		}
		else {
			//End the current scene.
			(*CSceneManagerOpenGL::mCurrentScene)->ConcludeScene();
			//Continue to the next scene.
			CSceneManagerOpenGL::IncrementScene();

			//Initialize and begin the next scene.
			CSceneManagerOpenGL::StartCurrentScene();
		}
	}

	//Show the Output.
	CSceneManagerOpenGL::RefreshOpenGLDisplay();
}

//Adds a scene to the scene manager.
bool CSceneManagerOpenGL::AddScene(CSceneOpenGL* pSceneOpenGL)
{

	bool							bSceneAddedSuccessfully = false;

	if (pSceneOpenGL) {
		//Wait for the scene manager resources to become available...
		while (CSceneManagerOpenGL::IsSceneManagerDataLocked()) { }


		//Acquire exclusive access to scene manager resources.
		CSceneManagerOpenGL::LockSceneManagerData();

		//Add the scene to the scene list.
		CSceneManagerOpenGL::mSceneList.push_back(pSceneOpenGL);

		bSceneAddedSuccessfully = true;

		//Relinquish control of the scene manager resources.
		CSceneManagerOpenGL::UnlockSceneManagerData();
	}


	return(bSceneAddedSuccessfully);
}

//Sets the scene duration (independent of transition duration).
void CSceneManagerOpenGL::SetSceneDuration(const QuantityType sceneDuration)
{

	//Wait for the scene manager resources to become available....
	while (CSceneManagerOpenGL::IsSceneManagerDataLocked()) { }

	//Acquire exclusive access to scene manager resources.
	CSceneManagerOpenGL::LockSceneManagerData();

	//Set the scene duration.
	CSceneManagerOpenGL::mSceneDuration = sceneDuration;

	//Relinquish control of the scene manager resources.
	CSceneManagerOpenGL::UnlockSceneManagerData();
}

//Sets the scene transition duration (independent of scene duration).
void CSceneManagerOpenGL::SetTransitionDuration(const QuantityType transitionDuration)
{
	//Wait for the scene manager resources to become available....
	while (CSceneManagerOpenGL::IsSceneManagerDataLocked()) { }

	//Acquire exclusive access to scene manager resources.
	CSceneManagerOpenGL::LockSceneManagerData();

	//Set the transition duration.
	CSceneManagerOpenGL::mTransitionDuration = transitionDuration;

	//Relinquish control of the scene manager resources.
	CSceneManagerOpenGL::UnlockSceneManagerData();
}


//Sets the target/maximum scene refresh rate.
void CSceneManagerOpenGL::SetUpdateRate(const QuantityType updateRate)
{
	//Wait for the scene manager resources to become available....
	while (CSceneManagerOpenGL::IsSceneManagerDataLocked()) { }

	//Acquire exclusive access to scene manager resources.
	CSceneManagerOpenGL::LockSceneManagerData();

	//Set the delay between scene updates.
	CSceneManagerOpenGL::mSceneUpdateRate = updateRate;

	//Relinquish control of the scene manager resources.
	CSceneManagerOpenGL::UnlockSceneManagerData();
}


//Initializes the scene manager, and commences scene manager execution.
bool CSceneManagerOpenGL::StartSceneManager()
{
	bool							bSceneManagerStarted = false;

	//The scene manager cannot be initialized twice; also, the scene
	//manager must contain at least one scene to display, and the
	//scene duration must be a valid, non-zero value.
	if (!CSceneManagerOpenGL::mbSceneManagerInitialized &&
		!CSceneManagerOpenGL::mSceneList.empty() &&
		(CSceneManagerOpenGL::mSceneDuration != 0) &&
		(CSceneManagerOpenGL::mSceneUpdateRate != 0))
	{
		//Initialize data lock flag...
		CSceneManagerOpenGL::mbSceneManagerDataLockFlag = false;

		//Indicate that the manager has been initialized
		//(at this point, initialization will be successful,
		//as rudimentary variable initializations remain).
		CSceneManagerOpenGL::mbSceneManagerInitialized = true;

		//Explicitly lock the scene manager data.
		CSceneManagerOpenGL::LockSceneManagerData();

		//Set the current scene to the first scene in the list.
		CSceneManagerOpenGL::mCurrentScene =
			CSceneManagerOpenGL::mSceneList.begin();

		//Reset the current scene and transition start times to zero.
		CSceneManagerOpenGL::mCurrentSceneStartTime = 0;
		CSceneManagerOpenGL::mCurrentTransitionStartTime = 0;

		//Set the scene update time reference.
		CSceneManagerOpenGL::mSceneUpdateTimeReference = CSceneManagerOpenGL::GetCurrentClockCount();

		//Initialize and begin the current scene.
		//!!! Cannot be performed here, as GLUT may not have been initialized yet.
		//CSceneManagerOpenGL::StartCurrentScene();

		bSceneManagerStarted = true;

		//Unlock the scene manager data.
		CSceneManagerOpenGL::UnlockSceneManagerData();
	}

	return(bSceneManagerStarted);
}


//Ends scene manager functionality.
bool CSceneManagerOpenGL::ConcludeSceneManager()
{
	bool							bSceneManagerConcluded = false;

	CSceneManagerOpenGL::LockSceneManagerData();

	//Remove all scenes from the scene queue.
	while(!CSceneManagerOpenGL::mSceneList.empty()) {
		CSceneManagerOpenGL::mSceneList.pop_back();
	}

	//Remove the current scene reference
	CSceneManagerOpenGL::mCurrentScene = NULL;

	return(bSceneManagerConcluded);
}


CSceneManagerOpenGL::CSceneManagerOpenGL()
{
	//Static class - constructor is never called.
}

CSceneManagerOpenGL::~CSceneManagerOpenGL()
{
	//Static class - destructor is never called.
}

//Executes a render step for the current scene (rendering is not necessarily displayed
//immediately).
bool CSceneManagerOpenGL::ExecuteCurrentSceneStep(const QuantityType currentSceneTime)
{
	bool							bStepExecutedSuccessfully = false;


	if (CSceneManagerOpenGL::IsSceneManagerReady()) {

		//Wait for the scene manager resources to become available....
		while (CSceneManagerOpenGL::IsSceneManagerDataLocked()) { }
			
		if (CSceneManagerOpenGL::mCurrentScene != CSceneManagerOpenGL::mSceneList.end()) {
			bStepExecutedSuccessfully = (*CSceneManagerOpenGL::mCurrentScene)->UpdateScene(currentSceneTime);
		}
	}
	




	return(bStepExecutedSuccessfully);
}

//Updates the OpenGL camera in accorance with the current scene.
bool CSceneManagerOpenGL::UpdateSceneCamera(const CSceneCameraOpenGL& sceneCamera)
{
	bool							bCameraUpdatedSuccessfully = false;

	const CFloatPoint				cameraPosition = sceneCamera.GetCameraPosition();
	const CVector					cameraUpVector = sceneCamera.GetCameraUpVector();
	const CFloatPoint				cameraFocusPoint = sceneCamera.GetFocusPoint();

	::glMatrixMode(GL_PROJECTION);
	//Reset the current view...
	::glLoadIdentity();
	//Set the projection transformation parameters.
	::gluPerspective(sceneCamera.GetFOV(), sceneCamera.GetAspectRatio(),
		sceneCamera.GetNearClipPlane(), sceneCamera.GetFarClipPlane());

	::glMatrixMode(GL_MODELVIEW);
	//Reset the current view...
	::glLoadIdentity();
	//Set the absolute view parameters.
	::gluLookAt(cameraPosition.GetXCoord(), cameraPosition.GetYCoord(),
		cameraPosition.GetZCoord(),
		cameraFocusPoint.GetXCoord(), cameraFocusPoint.GetYCoord(),
		cameraFocusPoint.GetZCoord(),
		cameraUpVector.GetXComponent(), cameraUpVector.GetYComponent(),
		cameraUpVector.GetZComponent());

	return(bCameraUpdatedSuccessfully);
}

//Refreshes the OpenGL buffer, effectively rendering the scene.
bool CSceneManagerOpenGL::RefreshOpenGLDisplay()
{
	bool							bDisplayRefreshedSuccessfully = false;

	//Update the OpenGL display.
	//::glFlush();

	if((CSceneManagerOpenGL::mSceneList.size() > 0) && *CSceneManagerOpenGL::mCurrentScene) {
		//Update the OpenGL camera.
		CSceneManagerOpenGL::UpdateSceneCamera(
			(*CSceneManagerOpenGL::mCurrentScene)->GetSceneCamera());

		//Clear the depth buffer.
		::glClear(GL_DEPTH_BUFFER_BIT);

		//Render the scene.
		(*CSceneManagerOpenGL::mCurrentScene)->RenderScene();
	}

	::glutSwapBuffers();


	bDisplayRefreshedSuccessfully = true;

	return(bDisplayRefreshedSuccessfully);
}


bool CSceneManagerOpenGL::StartCurrentScene()
{
	bool							bStartedSuccessfully = false;

	//Initialize the scene, and begin scene execution.
	bStartedSuccessfully = (*CSceneManagerOpenGL::mCurrentScene)->InitializeScene() &&
		(*CSceneManagerOpenGL::mCurrentScene)->CommenceScene();

	//Set the start time of the current scene.
	CSceneManagerOpenGL::mCurrentSceneStartTime =
		CSceneManagerOpenGL::GetCurrentClockCount();


	return(bStartedSuccessfully);
}


//Attempts to initiate a transition for the current scene.
bool CSceneManagerOpenGL::BeginCurrentSceneTransition(ESceneState eSceneTransition)
{
	bool							bTransitionStartedSuccessfully = false;

	return(bTransitionStartedSuccessfully);
}


//Progresses to the next scene.
bool CSceneManagerOpenGL::IncrementScene()
{
	bool							bSceneIncremented = false;

	//Wait for the scene manager resources to become available...
	while (CSceneManagerOpenGL::IsSceneManagerDataLocked()) { }

	if (CSceneManagerOpenGL::IsSceneManagerReady()) {
		//Proceed to the next scene in the list...
		CSceneManagerOpenGL::mCurrentScene++;

		//If the scene list has been exhausted, restart the scene
		//display at the first scene in the list.
		if (CSceneManagerOpenGL::mCurrentScene == mSceneList.end()) {
			CSceneManagerOpenGL::mCurrentScene = mSceneList.begin();
		}


		bSceneIncremented = true;
	}


	return(bSceneIncremented);
}

//"Locks" scene manager data, allowing exclusive access for a particular
//scene manager routine.
bool CSceneManagerOpenGL::LockSceneManagerData()
{
	bool							bSceneManagerDataLocked = false;

	//Determine if the scene manager is initialized, in addition to determining
	//if the scene manager is "unlocked" (data "locking" cannot be performed
	//twice).
	if (CSceneManagerOpenGL::IsSceneManagerReady() &&
		!CSceneManagerOpenGL::mbSceneManagerDataLockFlag)
	{
		//"Lock" the data, and indicate a success condition.
		CSceneManagerOpenGL::mbSceneManagerDataLockFlag = true;

		bSceneManagerDataLocked = true;
	}

	return(bSceneManagerDataLocked);
}

//"Unlocks" scene manager data, permitting all scene manager routines
//to access data.
bool CSceneManagerOpenGL::UnlockSceneManagerData()
{
	bool							bSceneManagerDataUnlocked = false;

	if (CSceneManagerOpenGL::IsSceneManagerReady() &&
		CSceneManagerOpenGL::mbSceneManagerDataLockFlag)
	{
		//"Unlock" the data, and indicate a success condition.
		CSceneManagerOpenGL::mbSceneManagerDataLockFlag = false;

		bSceneManagerDataUnlocked = true;
	}

	return(bSceneManagerDataUnlocked);
}

//Determines if scene manager data is locked.
bool CSceneManagerOpenGL::IsSceneManagerDataLocked()
{
	//Return the status of the data lock flag; also, the scene manager
	//can only be locked if it has been first initialized.
	return(CSceneManagerOpenGL::IsSceneManagerReady() &&
		CSceneManagerOpenGL::mbSceneManagerDataLockFlag);
}

//Determines if the scene manager is initialized, and if all functionality
//is immediately available.
bool CSceneManagerOpenGL::IsSceneManagerReady()
{
	return(CSceneManagerOpenGL::mbSceneManagerInitialized);
}


QuantityType CSceneManagerOpenGL::GetCurrentClockCount()
{
	//Retrieve a count from the system timer (Windows; should probably use
	//GetTickCount64() eventually to virtually eliminate the possibility of
	//timer saturation).
	return((QuantityType)::GetTickCount());
}

//Computes an event duration (milliseconds) from a provided starting
//system timer count (past event).
QuantityType CSceneManagerOpenGL::ComputeCountDuration(const QuantityType startCount)
{
	QuantityType					countDuration = 0;

	const QuantityType				currentClockCount = 
		CSceneManagerOpenGL::GetCurrentClockCount();

	if(startCount < currentClockCount) {
		//Compute the event duration.
		countDuration = currentClockCount - startCount;
	}
	else {
		//Handle clock counter overflow condition (~49.7 days for a millisecond
		//timer).
		countDuration = currentClockCount + (((sizeof(QuantityType) << 1) - 1)  - startCount);
	}

	return(countDuration);
}


//Scene manager static variable instantiation section.

//Maximum scene duration, in milliseconds.
QuantityType						CSceneManagerOpenGL::mSceneDuration = 0;

//Maximum transition duration, in milliseconds.
QuantityType						CSceneManagerOpenGL::mTransitionDuration = 0;

//Current scene execution start time (milliseconds).
QuantityType						CSceneManagerOpenGL::mCurrentSceneStartTime = 0;

//Current transition execution start time (milliseconds).
QuantityType						CSceneManagerOpenGL::mCurrentTransitionStartTime = 0;

//Maximum/target scene update rate (seconds / frame - reciprocal of frame rate).
QuantityType						CSceneManagerOpenGL::mSceneUpdateRate = 0;

//Time reference used to compute time since last scene refresh (milliseconds).
QuantityType						CSceneManagerOpenGL::mSceneUpdateTimeReference = 0;

//Internal list of OpenGL scenes.
CSceneManagerOpenGL::SceneList		CSceneManagerOpenGL::mSceneList;

//Current scene being executed in the scene list.
CSceneManagerOpenGL::SceneList::iterator
									CSceneManagerOpenGL::mCurrentScene =
									CSceneManagerOpenGL::mSceneList.begin();

//Scene manager data lock flag (mutex).
bool								CSceneManagerOpenGL::mbSceneManagerDataLockFlag = false;

//Scene manager initialization flag.
bool								CSceneManagerOpenGL::mbSceneManagerInitialized = false;