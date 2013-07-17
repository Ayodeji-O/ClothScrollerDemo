// SceneRepOpenGL.h - Contains declarations pertinent to scene representation under OpenGL

#ifndef _SCENEREPOPENGL_H
#define _SCENEREPOPENGL_H			1

#include "PrimitiveSupport.h"
#include "RenderSupport.h"

// OpenGL scene camera representation. (Ideally, this class should
// derive from a base interface specification class if multiple
// renderers are to be supported).
class CSceneCameraOpenGL
{
public:
	CSceneCameraOpenGL();
	~CSceneCameraOpenGL();

	//Camera Positioning Routines

	//Obtains the camera "look-at" point
	const CFloatPoint&				GetFocusPoint() const;
	//Obtains the camera position.
	const CFloatPoint&				GetCameraPosition() const;
	//Obtains the vector that indicates the normal to a plane that is parallel
	//to the top of the camera ("yaw").
	const CVector&					GetCameraUpVector() const;

	//Sets the camera "look-at" point.
	void							SetFocusPoint(const CFloatPoint& focusPoint);
	//Sets the camera position.
	void							SetCameraPosition(const CFloatPoint& cameraPosition);
	//Sets the vector that specifies the camera "yaw".
	void							SetCameraUpVector(const CVector& upVector);



	//View frustrum definition routines.

	FloatCoord						GetNearClipPlane() const;
	FloatCoord						GetFarClipPlane() const;
	FloatCoord						GetFOV() const;
	FloatCoord						GetAspectRatio() const;

	void							SetNearClipPlane(const FloatCoord nearClipPlane);

	void							SetFarClipPlane(const FloatCoord farClipPlane);

	void							SetFOV(const FloatCoord fovAngle);

	void							SetAspectRatio(const FloatCoord aspectRatio);

protected:
	//Camera "look" at point.
	CFloatPoint						mFocusPoint;
	//Camera position.
	CFloatPoint						mCameraPosition;
	//Vector (normal to camera top plane) that specifies camera "yaw".
	CVector							mCameraUpVector;

	//Field-of-view angle.
	FloatCoord						mFOVAngle;
	//Aspect ratio (width / height).
	FloatCoord						mAspectRatio;

	//Near clip plane (distance from camera).
	FloatCoord						mNearClipPlane;
	//Far clip plane (distance from camera).
	FloatCoord						mFarClipPlane;
};


//OpenGL scene transition representation base functionality/interface
//specification.
class CSceneTransitionOpenGL
{
public:

	CSceneTransitionOpenGL();
	virtual ~CSceneTransitionOpenGL() = 0;

	//Initiates and opening transition.
	virtual bool					OpenTransitionBegin();
	//Initiates a closing transition.
	virtual bool					CloseTransitionBegin();
	//Sets the length of the transition, if possible.
	virtual bool					SetTransitionLength(const QuantityType lengthInTicks);

	//Performs a stepwise update of the transition.
	virtual bool					UpdateTransition(const QuantityType currentClockTick);

	virtual CSceneCameraOpenGL		ModifyCamera(const CSceneCameraOpenGL& sceneCamera) const;

protected:
	//Scene transition state enumeration
	typedef enum
	{
		eDormant,					//Transition is idle.
		eOpening,					//Transition is in opening sequence mode.
		eClosing,					//Transition is in closing sequence mode.
	} ETransitionState;


	//Obtains the current transition state.
	ETransitionState				GetTransitionState();
	//Sets the transition state.
	bool							SetTransitionState(const ETransitionState& transitionState);

private:
	//Current transition state.
	ETransitionState				mTransitionState;
};


//Scene state enumeration
typedef enum
{
	eSceneUninitialized,			//Scene has not been initialized
	eSceneOpen,						//Scene is executing opening sequence.
	eSceneActive,					//Scene is active, in steady-state loop operation.
	eSceneClose						//Scene is executing closing sequence.
} ESceneState;

//OpenGL scene representation base functionality/interface specification.
//Provides the programmatic interface for scene generation and updating.
//(Ideally, this class should derive from a base interface specification
//class if multiple renderers are to be supported).
class CSceneOpenGL
{
public:
	CSceneOpenGL();
	virtual ~CSceneOpenGL() = 0;

	//Initializes the scene - this function must be called before generating
	//a scene.
	virtual bool					InitializeScene();

	//Determines if the scene has been initialized.
	virtual bool					IsSceneInitialized() const;

	//Destroys all constructs pertaining to the scene, returning the scene to
	//its uninitialized state.
	virtual bool					DestroyScene();

	//Produces a request for the scene to execute its introductory sequence.
	bool							CommenceScene();
	//Produces a request for the scene to execute its closing sequence.
	bool							ConcludeScene();

	//Performs a stepwise scene update.
	virtual bool					UpdateScene(const QuantityType currentClockTick) = 0;
	
	//Retrieves the current camera configuration specified by the scene.
	const CSceneCameraOpenGL&		GetSceneCamera() const;

	//Renders the scene (does not perform a flush or buffer swap).
	virtual bool					RenderScene() const = 0;

	//Adds a transition object to the scene.
	bool							SetTransition(const CSceneTransitionOpenGL& sceneTransition);

	//Activates the scene transition - the transition appearance is contextual, based
	//upon the current scene state.
	bool							ActivateTransition() const;

protected:
	//Sets the current scene camera.
	void							SetSceneCamera(const CSceneCameraOpenGL& sceneCamera);

	//Retrieves the current scene state.
	ESceneState						GetSceneState() const;

	//Determines if a transition has been activated.
	bool							IsTransitionActive() const;

	//Contains the current scene state.
	ESceneState						mSceneState;

	//Will be TRUE if a transition has been activated.
	bool							mbTransitionActive;

private:
	//Camera associated with scene.
	CSceneCameraOpenGL				mSceneCamera;

	//Transition that is currently assigned to the scene.
	//CSceneTransitionOpenGL&			mSceneTransition;
};

#endif	//#ifndef _SCENEREPOPENGL_H