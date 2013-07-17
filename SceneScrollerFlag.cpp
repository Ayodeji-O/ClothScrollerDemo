// SceneScrollerFlag.h - Scene of flag waving in the wind, with a scrolling message -
//                       implementation.


#include "SceneScrollerFlag.h"

#include "DataStreamServer.h"


CSceneScrollerFlag::CSceneScrollerFlag() : CBaseClothScrollerScene(mkFlagSceneConstants)
{

}


CSceneScrollerFlag::~CSceneScrollerFlag()
{
	// Delete the backdrop mesh.
	if (this->mpBackDropMesh) {
		delete(this->mpBackDropMesh);
	}
}

bool CSceneScrollerFlag::InitializeScene()
{
	bool							bSceneInitializedSuccessfully = false;

	// Set the time quantum used for force application calculations.
	this->mClothNodeNetwork.SetForceTimeQuantum(mkFlagSceneConstants.mSimulationTimeQuantum);

	// Set the flag color and specularity values (for rendering).
	this->mClothNodeNetwork.SetClothBaseColor(mkFlagSceneConstants.mClothColor);
	this->mClothNodeNetwork.SetClothSpecularity(mkFlagSceneConstants.mClothSpecularity);

	// Set the nodes on the left side of the flag to be anchor nodes.
	for (IndexType anchorNodeSetLoop = 0;
		anchorNodeSetLoop < this->mkFlagSceneConstants.mNodesPerColumn;
		anchorNodeSetLoop++)
	{
		this->mClothNodeNetwork.SetAnchorNode(0, anchorNodeSetLoop, true);
	}
	
	// Create the scene backdrop.
	bSceneInitializedSuccessfully = this->SetupBackDrop();

	return(bSceneInitializedSuccessfully);
}

void CSceneScrollerFlag::PositionSceneCamera(const QuantityType currentClockTick)
{
	CSceneCameraOpenGL				sceneCamera;

	// Amplitude of camera "sway" in the x and y axis.
	const ScalarType				kMaxCameraXSway = 0.5;
	const ScalarType				kMaxCameraYSway = 0.5;

	// Clock tick divisor for camera sway rate (tick is used as input into trigonometric
	// functions that govern sway motion).
	const ScalarType				kCameraSwayRateDivisor = 2000.0;


	//View frustrum definition
	//90° Field of view.
	const FloatCoord				kFieldOfView = 90.0;
	//4:3 Aspect ratio.
	const FloatCoord				kAspectRatio = (FloatCoord)4.0 / (FloatCoord)3.0;
	//Near and far clip plane distances
	const FloatCoord				kNearClipDistance = 1.0;
	const FloatCoord				kFarClipDistance = 5000.0;

	//Camera position and orientation
	//Camera position.
	const CFloatPoint				kCameraPosition(
		::cos((ScalarType)currentClockTick / kCameraSwayRateDivisor) * kMaxCameraXSway,
		::sin((ScalarType)currentClockTick / kCameraSwayRateDivisor) * kMaxCameraYSway, 5.0);
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

bool CSceneScrollerFlag::UpdateScene(const QuantityType currentClockTick)
{
	bool							bUpdatedSuccessfully = false;

	// Cycle through text colors, using the current clock tick and relatively
	// large prime numbers as clock tick divisors.
	const ScalarType				kClockDivisor1 = 1163.0;
	const ScalarType				kClockDivisor2 = 2903.0;
	const ScalarType				kClockDivisor3 = 3541.0;

	this->mScrollerTextColor = CFloatColor(
		::pow(::cos((ScalarType)currentClockTick / kClockDivisor1), 2.0),
		::pow(::cos((ScalarType)currentClockTick / kClockDivisor2), 2.0),
		::pow(::cos((ScalarType)currentClockTick / kClockDivisor3), 2.0));

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


bool CSceneScrollerFlag::RenderScene() const
{
	bool							bRenderedSuccessfully = false;

	// Render the backdrop mesh if it has been created.
	if (this->mpBackDropMesh) {
		(this->mpBackDropMesh)->RenderMesh();
	}

	// Render the flag.
	bRenderedSuccessfully = this->RenderClothNetworkWithScrollerTexture();

	return(bRenderedSuccessfully);
}



const CBaseClothScrollerSceneParamBlock
									CSceneScrollerFlag::mkFlagSceneConstants(
		10,											// Nodes per row
		10,											// Nodes per column
		10.0,										// Node network physical width
		5.0,										// Node network physical height
		CFloatPoint(-4.5, -1.0, 0.0),				// Node network physical offset
		0.02,										// Mass (kg) per node
		10.0,										// Structural spring constant.
		3.5,										// Shear spring constant.
		1.0,										// Flexion spring constant.
		9.8,										// Gravitational acceleration constant (N)
		CVector(0.0, -1.0, 0.0),					// Gravity vector
		0.01,										// Simulation time quantum (s).
		CFloatColor(0.7, 0.7, 0.7),					// Flag color
		0.99,										// Flag specularity
		0.8,										// Wind magnitude
        CVector(1.0, 0.0, 0.0),						// Wind direction
		0.03,										// Viscous drag coefficient
		CFloatColor(0.6, 0.0, 0.0));				// Backdrop cylinder color.


// Flag scroller scene constants.
/*const QuantityType					CSceneScrollerFlag::mkNodesPerRow = 10;
const QuantityType					CSceneScrollerFlag::mkNodesPerColumn = 10;
const FloatCoord					CSceneScrollerFlag::mkNodeNetworkWidth = 10.0;
const FloatCoord					CSceneScrollerFlag::mkNodeNetworkHeight = 5.0;
const CFloatPoint					CSceneScrollerFlag::mkNetworkCoordOffset =
	CFloatPoint(-4.5, -1.0, 0.0);
const ScalarType					CSceneScrollerFlag::mkNodeMass = 0.02;
const ScalarType					CSceneScrollerFlag::mkStructSpringConstant = 10.0;
const ScalarType					CSceneScrollerFlag::mkShearSpringConstant = 3.5;
const ScalarType					CSceneScrollerFlag::mkFlexionSpringConstant = 1.0;
const ScalarType					CSceneScrollerFlag::mkGravitationalConstant = 9.8;
const CVector						CSceneScrollerFlag::mkGravityVector = CVector(0.0, -1.0, 0.0);
const ScalarType					CSceneScrollerFlag::mkSimulationTimeQuantum = 0.01;
const CFloatColor					CSceneScrollerFlag::mkFlagColor = CFloatColor(0.7, 0.7, 0.7);
const ScalarType					CSceneScrollerFlag::mkFlagSpecularity = 0.99;
const ScalarType					CSceneScrollerFlag::mkMaxWindMagnitude = 0.8;
const CVector						CSceneScrollerFlag::mkWindDirection(1.0, 0.0, 0.0);
const ScalarType					CSceneScrollerFlag::mkViscousCoefficient = 0.03;*/
