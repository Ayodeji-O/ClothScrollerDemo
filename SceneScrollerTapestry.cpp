// SceneScrollerTapestry.cpp - Tapestry suspended on top end, being blown by wind from the rear
//                             in a diagonal-upward direction, with a scrolling message on the
//                             tapestry- implementation.

#include "SceneScrollerTapestry.h"





CSceneScrollerTapestry::CSceneScrollerTapestry() :
	CBaseClothScrollerScene(CSceneScrollerTapestry::mkTapestrySceneConstants)
{

}

CSceneScrollerTapestry::~CSceneScrollerTapestry()
{

}


bool CSceneScrollerTapestry::InitializeScene()
{
	bool							bSceneInitializedSuccessfully = false;

	// Set the time quantum used for force application calculations.
	this->mClothNodeNetwork.SetForceTimeQuantum(mkTapestrySceneConstants.mSimulationTimeQuantum);

	// Set the tapestry color and specularity values (for rendering).
	this->mClothNodeNetwork.SetClothBaseColor(mkTapestrySceneConstants.mClothColor);
	this->mClothNodeNetwork.SetClothSpecularity(mkTapestrySceneConstants.mClothSpecularity);


	// Set the nodes on the top of the tapestry to be anchor nodes.
	for (IndexType anchorNodeSetLoop = 0;
		anchorNodeSetLoop < this->mkTapestrySceneConstants.mNodesPerRow;
		anchorNodeSetLoop++)
	{
		this->mClothNodeNetwork.SetAnchorNode(anchorNodeSetLoop, 
			(this->mkTapestrySceneConstants.mNodesPerColumn - 1), true);
	}

	// Create the scene backdrop.
	bSceneInitializedSuccessfully = this->SetupBackDrop();

	return(bSceneInitializedSuccessfully);
}

void CSceneScrollerTapestry::PositionSceneCamera(const QuantityType currentClockTick)
{
	CSceneCameraOpenGL				sceneCamera;

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
	const CFloatPoint				kCameraPosition(0.0, 0.0, 6.0);
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


bool CSceneScrollerTapestry::UpdateScene(const QuantityType currentClockTick)
{
	bool							bUpdatedSuccessfully = false;

	// Cycle through text and background colors, using the current clock tick and relatively
	// large prime numbers as clock tick divisors.
	const ScalarType				kClockDivisor1 = 1229;
	const ScalarType				kClockDivisor2 = 839;
	const ScalarType				kClockDivisor3 = 2003;

	const ScalarType				kColorShiftFraction = 0.1;

	const ScalarType				kStaticColorPortion = 1.0 - kColorShiftFraction;

	const ScalarType				colorComponentShift1 =
		::cos((ScalarType)currentClockTick / kClockDivisor1) * 2.0 * kColorShiftFraction;
	const ScalarType				colorComponentShift2 =
		::cos((ScalarType)currentClockTick / kClockDivisor2) * 2.0 * kColorShiftFraction;
	const ScalarType				colorComponentShift3 =
		::cos((ScalarType)currentClockTick / kClockDivisor3) * 2.0 * kColorShiftFraction;

	this->mScrollerBackgroundColor = CFloatColor(
		colorComponentShift1 / 7.0 + kColorShiftFraction * 4.5,
		colorComponentShift2 / 7.0 + kColorShiftFraction * 4.5,
		colorComponentShift3 / 7.0 + kColorShiftFraction * 4.5);
	this->mScrollerTextColor = CFloatColor(colorComponentShift1 + kStaticColorPortion,
		 colorComponentShift2 + kStaticColorPortion, colorComponentShift3 + kStaticColorPortion);


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


bool CSceneScrollerTapestry::RenderScene() const
{
	bool							bRenderedSuccessfully = false;

	// Render the backdrop mesh if it has been created.
	if (this->mpBackDropMesh) {
		(this->mpBackDropMesh)->RenderMesh();
	}

	// Render the tapestry.
	bRenderedSuccessfully = this->RenderClothNetworkWithScrollerTexture();

	return(bRenderedSuccessfully);
}




const CBaseClothScrollerSceneParamBlock
									CSceneScrollerTapestry::mkTapestrySceneConstants(
		10,											// Nodes per row
		10,											// Nodes per column
		10.0,										// Node network physical width
		7.0,										// Node network physical height
		CFloatPoint(-4.5, -2.5, 0.0),				// Node network physical offset
		0.03,										// Mass (kg) per node
		10.0,										// Structural spring constant.
		1.0,										// Shear spring constant.
		1.0,										// Flexion spring constant.
		9.8,										// Gravitational acceleration constant (N)
		CVector(0.0, -1.0, 0.0),					// Gravity vector
		0.01,										// Simulation time quantum (s).
		CFloatColor(0.7, 0.7, 0.7),					// Tapestry color
		0.99,										// Tapestry specularity
		0.4,										// Wind magnitude
        CVector(0.0, 1.0, 1.0),						// Wind direction
		0.01,										// Viscous drag coefficient
		CFloatColor(0.4, 0.4, 0.4));				// Backdrop cylinder color.