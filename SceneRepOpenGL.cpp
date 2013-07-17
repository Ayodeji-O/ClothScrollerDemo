// SceneRepOpenGL.cpp - Contains implementations pertinent to scene representation under OpenGL

#include "SceneRepOpenGL.h"


/////
// CSceneCameraOpenGL class
/////

CSceneCameraOpenGL::CSceneCameraOpenGL() :
	mFocusPoint(0.0, 0.0, 0.0), mCameraPosition(0.0, 0.0, 0.0), mCameraUpVector(0.0, 0.0, 0.0)
{
}

CSceneCameraOpenGL::~CSceneCameraOpenGL()
{
}

const CFloatPoint&	CSceneCameraOpenGL::GetFocusPoint() const
{
	//Return the camera focus point.
	return(this->mFocusPoint);
}

const CFloatPoint& CSceneCameraOpenGL::GetCameraPosition() const
{
	//Return the camera position.
	return(this->mCameraPosition);
}

const CVector& CSceneCameraOpenGL::GetCameraUpVector() const
{
	//Return the vector that determines the camera "yaw"
	return(this->mCameraUpVector);
}

void CSceneCameraOpenGL::SetFocusPoint(const CFloatPoint& focusPoint)
{
	//Set the point of camera focus.
	this->mFocusPoint = focusPoint;
}

void CSceneCameraOpenGL::SetCameraPosition(const CFloatPoint& cameraPosition)
{
	//Set the actual camera position
	this->mCameraPosition = cameraPosition;
}

void CSceneCameraOpenGL::SetCameraUpVector(const CVector& upVector)
{
	//Set the vector that is normal to a plane parallel to the top
	//of the camera.
	this->mCameraUpVector = upVector;

	//Normalize the vector.
	this->mCameraUpVector.Normalize();
}


FloatCoord	CSceneCameraOpenGL::GetNearClipPlane() const
{
	//Return the "near" clip plane of the view frustrum (distance from camera).
	return(this->mNearClipPlane);
}

FloatCoord	CSceneCameraOpenGL::GetFarClipPlane() const
{
	//Return the "far" clip plane of the view frustrum (distance from camera).
	return(this->mFarClipPlane);
}


FloatCoord CSceneCameraOpenGL::GetFOV() const
{
	//Return the field-of-view angle (degrees).
	return(this->mFOVAngle);
}

FloatCoord CSceneCameraOpenGL::GetAspectRatio() const
{
	//Retrieve the view aspect ratio (width / height).
	return(this->mAspectRatio);
}

void CSceneCameraOpenGL::SetNearClipPlane(const FloatCoord nearClipPlane)
{
	//Set distance of the near clip plane from the camera.
	this->mNearClipPlane = nearClipPlane;
}

void CSceneCameraOpenGL::SetFarClipPlane(const FloatCoord farClipPlane)
{
	//Set the distance of the far clip plane from the camera.
	this->mFarClipPlane = farClipPlane;
}

void CSceneCameraOpenGL::SetFOV(const FloatCoord fovAngle)
{
	//Set the field-of-view angle.
	this->mFOVAngle = fovAngle;
}

void CSceneCameraOpenGL::SetAspectRatio(const FloatCoord aspectRatio)
{
	//Set the view aspect ratio (width / height).
	this->mAspectRatio = aspectRatio;
}


/////
// CSceneTransitionOpenGL class
/////

CSceneTransitionOpenGL::CSceneTransitionOpenGL()
{
}

CSceneTransitionOpenGL::~CSceneTransitionOpenGL()
{
}

//Initiates and opening transition.
bool CSceneTransitionOpenGL::OpenTransitionBegin()
{
	bool							bTransitionSet = false;

	if(this->mTransitionState != eOpening) {
		this->mTransitionState = eOpening;
		bTransitionSet = true;
	}

	return(bTransitionSet);
}


//Initiates a closing transition.
bool CSceneTransitionOpenGL::CloseTransitionBegin()
{
	this->mTransitionState = eClosing;

	return(true);
}


//Sets the length of the transition, if possible.
bool CSceneTransitionOpenGL::SetTransitionLength(const QuantityType lengthInTicks)
{
	return(false);
}

//Performs a stepwise update of the transition.
bool CSceneTransitionOpenGL::UpdateTransition(const QuantityType currentClockTick)
{
	return(false);
}

CSceneCameraOpenGL CSceneTransitionOpenGL::ModifyCamera(const CSceneCameraOpenGL& sceneCamera) const
{
	//Base functionality - do not modify camera.
	return(sceneCamera);
}


/////
// CSceneOpenGL class
/////

CSceneOpenGL::CSceneOpenGL() : mSceneState(eSceneUninitialized)
{
}

CSceneOpenGL::~CSceneOpenGL()
{
}


bool CSceneOpenGL::InitializeScene()
{
	//Ideally, this routine should be overloaded by subclasses.
	return(false);
}

bool CSceneOpenGL::IsSceneInitialized() const
{	
	//Determine if the scene has been initialized.
	return(this->mSceneState != eSceneUninitialized);
}


bool CSceneOpenGL::DestroyScene()
{
	//...This routine should be overloaded by subclasses along with
	//InitializeScene().
	return(false);
}

//Produces a request for the scene to execute its introductory sequence.
bool CSceneOpenGL::CommenceScene()
{
	//!!! (Should be "opening" while transition is active).
	this->mSceneState = eSceneActive;

	return(true);
}

//Produces a request for the scene to execute its closing sequence.
bool CSceneOpenGL::ConcludeScene()
{
	this->mSceneState = eSceneClose;

	return(true);
}

//Performs a stepwise scene update.
bool CSceneOpenGL::UpdateScene(const QuantityType currentClockTick)
{
	return(false);
}
	
//Retrieves the current camera configuration specified by the scene.
const CSceneCameraOpenGL& CSceneOpenGL::GetSceneCamera() const
{
	//Return the camera defined by the scene.
	return(this->mSceneCamera);
}

//Adds a transition object to the scene.
bool CSceneOpenGL::SetTransition(const CSceneTransitionOpenGL& sceneTransition)
{
	//Assign the current scene transition;
	//this->mSceneTransition = sceneTransition;

	return(false);
}

//Activates the scene transition - the transition appearance is contextual, based
//upon the current scene state.
bool CSceneOpenGL::ActivateTransition() const
{
	return(false);
}

void CSceneOpenGL::SetSceneCamera(const CSceneCameraOpenGL& sceneCamera)
{
	//Assign the new scene camera.
	this->mSceneCamera = sceneCamera;
}

//Retrieves the current scene state.
ESceneState CSceneOpenGL::GetSceneState() const
{
	//Return the current scene state.
	return(this->mSceneState);
}

//Determines if a transition has been activated.
bool CSceneOpenGL::IsTransitionActive() const
{
	//Return the transition activation state.
	return(this->mbTransitionActive);
}
