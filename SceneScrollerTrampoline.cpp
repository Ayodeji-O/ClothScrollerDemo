// SceneScrollerTrampoline.cpp - Cloth restrained in a "trampoline" fashion (all edges are
//                               immobile), being blown from the rear by wind, with a scrolling
//                               message imprinted on the cloth - definitions.

#include "SceneScrollerTrampoline.h"


CSceneScrollerTrampoline::CSceneScrollerTrampoline() :
	CBaseClothScrollerScene(mkTrampolineSceneConstants)
{

}

CSceneScrollerTrampoline::~CSceneScrollerTrampoline()
{

}


bool CSceneScrollerTrampoline::InitializeScene()
{
	bool							bSceneInitializedSuccessfully = false;

	// Set the time quantum used for force application calculations.
	this->mClothNodeNetwork.SetForceTimeQuantum(mkTrampolineSceneConstants.mSimulationTimeQuantum);

	// Set the rising cloth color and specularity values (for rendering).
	this->mClothNodeNetwork.SetClothBaseColor(mkTrampolineSceneConstants.mClothColor);
	this->mClothNodeNetwork.SetClothSpecularity(mkTrampolineSceneConstants.mClothSpecularity);


	// Set the nodes on the bottom of the cloth to be anchor nodes.
	for (IndexType anchorNodeSetLoopRow = 1;
		anchorNodeSetLoopRow < this->mkTrampolineSceneConstants.mNodesPerRow - 1;
		anchorNodeSetLoopRow++)
	{
		this->mClothNodeNetwork.SetAnchorNode(anchorNodeSetLoopRow, 0, true);
		this->mClothNodeNetwork.SetAnchorNode(anchorNodeSetLoopRow, 
			this->mkTrampolineSceneConstants.mNodesPerColumn - 1, true);
	}

	for (IndexType anchorNodeSetLoopColumn = 0;
		anchorNodeSetLoopColumn < this->mkTrampolineSceneConstants.mNodesPerColumn;
		anchorNodeSetLoopColumn++)
	{
		this->mClothNodeNetwork.SetAnchorNode(0, anchorNodeSetLoopColumn, true);
		this->mClothNodeNetwork.SetAnchorNode(
			this->mkTrampolineSceneConstants.mNodesPerColumn - 1,
			anchorNodeSetLoopColumn, true);
	}
		
	// Create the scene backdrop.
	bSceneInitializedSuccessfully = this->SetupBackDrop();

	return(bSceneInitializedSuccessfully);
}

	
void CSceneScrollerTrampoline::PositionSceneCamera(const QuantityType currentClockTick)
{
	CSceneCameraOpenGL				sceneCamera;

	// Magnitude of panning
	const ScalarType				kPanMagnitude = 2.0;

	// Magnitude of Y "zooming"
	const ScalarType				kYZoomMagnitude = 1.0;

	const ScalarType				kClockDivisor = 1117.0;

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

	const FloatCoord				cameraYShift = kYZoomMagnitude *
		::cos((ScalarType)currentClockTick / kClockDivisor * 2.0);

	const CFloatPoint				kCameraPosition(cameraXPosition, -4.5 + cameraYShift, 2.7);
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


bool CSceneScrollerTrampoline::UpdateScene(const QuantityType currentClockTick)
{
	bool							bUpdatedSuccessfully = false;

	// Clock divisor.
	const ScalarType				kClockDivisor = 167.0;

	const ScalarType				currentTextIntensity = 
		(::cos((ScalarType)currentClockTick / kClockDivisor) + 4.0) / 5.0;

	this->mScrollerTextColor = CFloatColor(currentTextIntensity,
		currentTextIntensity, currentTextIntensity);

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


bool CSceneScrollerTrampoline::RenderScene() const
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

// Rising cloth scene constants.
const CBaseClothScrollerSceneParamBlock
									CSceneScrollerTrampoline::mkTrampolineSceneConstants(
		10,											// Nodes per row
		10,											// Nodes per column
		12.0,										// Node network physical width
		10.0,										// Node network physical height
		CFloatPoint(-4.5, -2.5, 0.0),				// Node network physical offset
		0.04,										// Mass (kg) per node
		15.0,										// Structural spring constant.
		3.0,										// Shear spring constant.
		3.0,										// Flexion spring constant.
		9.8,										// Gravitational acceleration constant (N)
		CVector(0.0, 0.0, -1.0),					// Gravity vector
		0.01,										// Simulation time quantum (s).
		CFloatColor(0.5, 0.5, 0.5),					// Trampoline color
		0.99,										// Trampoline specularity
		1.7,										// Wind magnitude
        CVector(0.0, 0.0, 1.0),						// Wind direction
		0.02,										// Viscous drag coefficient
		CFloatColor(0.0, 0.0, 0.8));				// Backdrop cylinder color.
