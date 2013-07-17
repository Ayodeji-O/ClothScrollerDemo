// SceneScrollerRisingCloth.cpp - Cloth teathered to the ground on one end, being suspended
//                              by upward-blowing wind, with a scrolling message displayed
//                              on the cloth - implementation.


#include "SceneScrollerRisingCloth.h"



CSceneScrollerRisingCloth::CSceneScrollerRisingCloth() :
	CBaseClothScrollerScene(mkRisingClothSceneConstants)
{

}


CSceneScrollerRisingCloth::~CSceneScrollerRisingCloth()
{

}


bool CSceneScrollerRisingCloth::InitializeScene()
{
	bool							bSceneInitializedSuccessfully = false;

	// Set the time quantum used for force application calculations.
	this->mClothNodeNetwork.SetForceTimeQuantum(mkRisingClothSceneConstants.mSimulationTimeQuantum);

	// Set the rising cloth color and specularity values (for rendering).
	this->mClothNodeNetwork.SetClothBaseColor(mkRisingClothSceneConstants.mClothColor);
	this->mClothNodeNetwork.SetClothSpecularity(mkRisingClothSceneConstants.mClothSpecularity);


	// Set the nodes on the bottom of the cloth to be anchor nodes.
	for (IndexType anchorNodeSetLoop = 0;
		anchorNodeSetLoop < this->mkRisingClothSceneConstants.mNodesPerRow;
		anchorNodeSetLoop++)
	{
		this->mClothNodeNetwork.SetAnchorNode(anchorNodeSetLoop, 0, true);
	}

	// Create the scene backdrop.
	bSceneInitializedSuccessfully = this->SetupBackDrop();

	return(bSceneInitializedSuccessfully);
}


void CSceneScrollerRisingCloth::PositionSceneCamera(const QuantityType currentClockTick)
{
	CSceneCameraOpenGL				sceneCamera;

	// Magnitude of panning
	const ScalarType				kPanMagnitude = 3.0;

	const ScalarType				kClockDivisor = 1489.0;

	//View frustrum definition
	//90° Field of view.
	const FloatCoord				kFieldOfView = 90.0;
	//4:3 Aspect ratio.
	const FloatCoord				kAspectRatio = (FloatCoord)4.0 / (FloatCoord)3.0;
	//Near and far clip plane distances
	const FloatCoord				kNearClipDistance = 1.0;
	const FloatCoord				kFarClipDistance = 9000.0;

	//Camera position and orientation
	//Camera position.

	const FloatCoord				cameraXPosition = kPanMagnitude *
		::cos((ScalarType)currentClockTick / kClockDivisor);

	const CFloatPoint				kCameraPosition(cameraXPosition, 0.0, 6.0);
	//Camera focus point
	const CFloatPoint				kCameraFocusPoint(0.0, 0.0, 0.0);
	//Camera orientation vector
	const CVector					kCameraOrientVector(0.0, 1.0, 0.0);

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


bool CSceneScrollerRisingCloth::UpdateScene(const QuantityType currentClockTick)
{
	bool							bUpdatedSuccessfully = false;

	// Brighten/fade rising cloth colors.

	// Clock divisor.
	const ScalarType				kClockDivisor = 229.0;

	const ScalarType				currentDynamicComponentMult = 
		(::cos((ScalarType)currentClockTick / kClockDivisor) + 2.0) / 3.0;

	this->mScrollerBackgroundColor = CFloatColor(
		mkRisingClothSceneConstants.mClothColor.GetBlueValue() ,
		mkRisingClothSceneConstants.mClothColor.GetGreenValue() * currentDynamicComponentMult,
		mkRisingClothSceneConstants.mClothColor.GetRedValue() * currentDynamicComponentMult);

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


bool CSceneScrollerRisingCloth::RenderScene() const
{
	bool							bRenderedSuccessfully = false;

	// Render the backdrop mesh if it has been created.
	if (this->mpBackDropMesh) {
		(this->mpBackDropMesh)->RenderMesh();
	}

	// Render the rising cloth.
	bRenderedSuccessfully = this->RenderClothNetworkWithScrollerTexture();

	return(bRenderedSuccessfully);
}


const CBaseClothScrollerSceneParamBlock
									CSceneScrollerRisingCloth::mkRisingClothSceneConstants(
		10,											// Nodes per row
		10,											// Nodes per column
		10.0,										// Node network physical width
		7.0,										// Node network physical height
		CFloatPoint(-4.5, -2.5, 0.0),				// Node network physical offset
		0.03,										// Mass (kg) per node
		7.0,										// Structural spring constant.
		0.2,										// Shear spring constant.
		0.2,										// Flexion spring constant.
		9.8,										// Gravitational acceleration constant (N)
		CVector(0.0, -1.0, 0.0),					// Gravity vector
		0.01,										// Simulation time quantum (s).
		CFloatColor(0.7, 0.7, 0.3),					// Rising cloth color
		0.99,										// Rising cloth specularity
		1.0,										// Wind magnitude
        CVector(0.0, 1.0, 0.0),						// Wind direction
		0.01,										// Viscous drag coefficient
		CFloatColor(0.2, 0.2, 0.2));				// Backdrop cylinder color.