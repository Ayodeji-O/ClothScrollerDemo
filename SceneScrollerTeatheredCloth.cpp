// SceneScrollerTeatheredCloth.cpp - Cloth teathered on four corners, being blown by wind from
//                                   directly behind, with a scrolling message imprinted on
//                                   the cloth - implementation.

#include "SceneScrollerTeatheredCloth.h"



CSceneScrollerTeatheredCloth::CSceneScrollerTeatheredCloth() :
	CBaseClothScrollerScene(CSceneScrollerTeatheredCloth::mkTeatheredClothSceneConstants)
{


}

CSceneScrollerTeatheredCloth::~CSceneScrollerTeatheredCloth()
{

}

bool CSceneScrollerTeatheredCloth::InitializeScene()
{
	bool							bSceneInitializedSuccessfully = false;

	// Set the time quantum used for force application calculations.
	this->mClothNodeNetwork.SetForceTimeQuantum(mkTeatheredClothSceneConstants.mSimulationTimeQuantum);

	// Set the teathered cloth color and specularity values (for rendering).
	this->mClothNodeNetwork.SetClothBaseColor(mkTeatheredClothSceneConstants.mClothColor);
	this->mClothNodeNetwork.SetClothSpecularity(mkTeatheredClothSceneConstants.mClothSpecularity);

	// Set the nodes at the corners of the flag node network to be anchor nodes.

	bSceneInitializedSuccessfully =
		this->mClothNodeNetwork.SetAnchorNode(0, 0, true) &&

		this->mClothNodeNetwork.SetAnchorNode(this->mkTeatheredClothSceneConstants.mNodesPerRow - 1,
			0, true) &&

		this->mClothNodeNetwork.SetAnchorNode(0, this->mkTeatheredClothSceneConstants.mNodesPerColumn - 1,
			true) &&

		this->mClothNodeNetwork.SetAnchorNode(this->mkTeatheredClothSceneConstants.mNodesPerRow - 1,
			this->mkTeatheredClothSceneConstants.mNodesPerColumn - 1,
			true);

	// Create the scene backdrop.
	bSceneInitializedSuccessfully = this->SetupBackDrop() && bSceneInitializedSuccessfully;

	return(bSceneInitializedSuccessfully);
}

void CSceneScrollerTeatheredCloth::PositionSceneCamera(const QuantityType currentClockTick)
{
	CSceneCameraOpenGL				sceneCamera;

	//View frustrum definition
	//90° Field of view.
	const FloatCoord				kFieldOfView = 90.0;
	//4:3 Aspect ratio.
	const FloatCoord				kAspectRatio = (FloatCoord)4.0 / (FloatCoord)3.0;
	//Near and far clip plane distances
	const FloatCoord				kNearClipDistance = 1.0;
	const FloatCoord				kFarClipDistance = 90.0;

	const ScalarType				kClockTickSpinDivisor = 700.0;
	const FloatCoord				kMaxZBounce = 5.0;
	const FloatCoord				kMinZCoord = 2.5;

	// Range of values for t required to plot Z = n - t^2
	const FloatCoord				inputRange = 2.0 * ::sqrt(kMaxZBounce);

	// Compute Z = n - t^2, with t ranging from (-range / 2) to (range / 2). This parabolic
	// curve produces a "bouncing" efffect.
	const FloatCoord				currentZ = kMaxZBounce -
		::pow(::fmod(((FloatCoord)currentClockTick / kClockTickSpinDivisor),
		inputRange) - inputRange / 2.0, 2.0);

	//Camera position and orientation
	//Camera position.
	const CFloatPoint				kCameraPosition(0.0, 0.0, currentZ + kMinZCoord);
	//Camera focus point
	const CFloatPoint				kCameraFocusPoint(0.0, 0.0, 0.0);
	//Camera orientation vector
	const CVector					kCameraOrientVector(::cos((ScalarType)currentClockTick / 
		kClockTickSpinDivisor), ::sin((ScalarType)currentClockTick / kClockTickSpinDivisor), 0.0);

	//Adjust the scene camera.
	sceneCamera.SetFOV(kFieldOfView);
	sceneCamera.SetAspectRatio(kAspectRatio);
	sceneCamera.SetNearClipPlane(kNearClipDistance);
	sceneCamera.SetFarClipPlane(kFarClipDistance);

	sceneCamera.SetCameraPosition(kCameraPosition);
	sceneCamera.SetFocusPoint(kCameraFocusPoint);
	sceneCamera.SetCameraUpVector(kCameraOrientVector);

	SetSceneCamera(sceneCamera);
}

//Performs a stepwise scene update.
bool CSceneScrollerTeatheredCloth::UpdateScene(const QuantityType currentClockTick)
{
	bool							bUpdatedSuccessfully = false;

	const ScalarType				kClockDivisor = 947.0;

	const ScalarType				variedColorComponent = 
		::pow(::cos(currentClockTick / kClockDivisor), 2.0);

	this->mScrollerBackgroundColor = CFloatColor(variedColorComponent, variedColorComponent,
		1.0);
	this->mScrollerTextColor = CFloatColor(1.0 - variedColorComponent, 1.0 - variedColorComponent,
		1.0);

	// Scroll the scroller.
	CScrollerManager::IncrementScroller();

	// Position the scene camera.
	this->PositionSceneCamera(currentClockTick);

	this->mWindForce.SetAlterationConstant(currentClockTick);

	// Perform a cloth network simulation step.
	bUpdatedSuccessfully =
		this->mClothNodeNetwork.EvaluateClothNodeNetworkForces(this->mExternalForceCollection);
	
	return(bUpdatedSuccessfully);
}

//Renders the scene (does not perform a flush or buffer swap).
bool CSceneScrollerTeatheredCloth::RenderScene() const
{
	bool							bRenderedSuccessfully = false;

	// Render the backdrop mesh if it has been created.
	if (this->mpBackDropMesh) {
		(this->mpBackDropMesh)->RenderMesh();
	}

	// Render the teathered cloth.
	bRenderedSuccessfully = this->RenderClothNetworkWithScrollerTexture();

	return(bRenderedSuccessfully);
}

const CBaseClothScrollerSceneParamBlock
									CSceneScrollerTeatheredCloth::mkTeatheredClothSceneConstants(
		5,											// Nodes per row
		10,											// Nodes per column
		10.0,										// Node network physical width
		10.0,										// Node network physical height
		CFloatPoint(-4.0, -4.5, 0.0),				// Node network physical offset
		0.01,										// Mass (kg) per node
		20.0,										// Structural spring constant.
		15.0,										// Shear spring constant.
		3.0,										// Flexion spring constant.
		9.8,										// Gravitational acceleration constant (N)
		CVector(0.0, 0.0, -1.0),					// Gravity vector
		0.01,										// Simulation time quantum (s).
		CFloatColor(0.7, 0.7, 0.7),					// Teathered cloth color
		0.99,										// Teathered cloth specularity
		0.8,										// Wind magnitude
        CVector(0.0, 0.0, 1.0),						// Wind direction
		0.001,										// Viscous drag coefficient
		CFloatColor(0.0, 0.0, 0.5));				// Backdrop cylinder color.