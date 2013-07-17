

#ifndef _CLOTHSCROLLERSCENESCOMMON_H
#define _CLOTHSCROLLERSCENESCOMMON_H	1

#include "PrimaryHeaders.h"
#include "SceneRepOpenGL.h"
#include "ModelGeometryOpenGL.h"
#include "ClothNodeNetwork.h"
#include "ExternalNodeForces.h"
#include "ClothScrollerModels.h"
#include "ScrollerManager.h"


class CBaseClothScrollerSceneParamBlock
{
public:
	CBaseClothScrollerSceneParamBlock(
		const QuantityType nodesPerRow,
		const QuantityType nodesPerColumn,
		const FloatCoord nodeNetworkWidth,
		const FloatCoord ndoeNetworkHeight,
		const CFloatPoint networkCoordOffset,
		const ScalarType nodeMass,
		const ScalarType structSpringConstant,
		const ScalarType shearSpringConstant,
		const ScalarType flexionSpringConstant,
		const ScalarType gravitationalConstant,
		const CVector& gravityVector,
		const ScalarType simulationTimeQuantum,
		const CFloatColor& clothColor,
		const ScalarType clothSpecularity,
		const ScalarType maxWindMagnitude,
		const CVector& windDirection,
		const ScalarType viscousCoefficient,
		const CFloatColor& backDropCylinderColor);
	~CBaseClothScrollerSceneParamBlock();

	QuantityType					mNodesPerRow;
	QuantityType					mNodesPerColumn;
	FloatCoord						mNodeNetworkWidth;
	FloatCoord						mNodeNetworkHeight;
	CFloatPoint						mNetworkCoordOffset;
	ScalarType						mNodeMass;
	ScalarType						mStructSpringConstant;
	ScalarType						mShearSpringConstant;
	ScalarType						mFlexionSpringConstant;
	ScalarType						mGravitationalConstant;
	CVector							mGravityVector;
	ScalarType						mSimulationTimeQuantum;
	CFloatColor						mClothColor;
	ScalarType						mClothSpecularity;
	ScalarType						mMaxWindMagnitude;
	CVector							mWindDirection;
	ScalarType						mViscousCoefficient;

	CFloatColor						mBackdropCylinderColor;
};

class CBaseClothScrollerScene : public CSceneOpenGL
{
public:

	CBaseClothScrollerScene(const CBaseClothScrollerSceneParamBlock& parameterBlock);
	~CBaseClothScrollerScene();

	// Adds all external forces to the external force collection.
	bool							BuildExternalForceCollection();

protected:
	// Adds a scroller texture layer to the texture collection object.
	bool							AddScrollerTextureLayer(CTextureCollection&
		textureCollection) const;

	// Renders the cloth network, using the scroller texture.
	bool							RenderClothNetworkWithScrollerTexture() const;

	// Constructs the scene backdrop (default - cylinder alinged with the Z-axis).
	virtual bool					SetupBackDrop();

	// Cloth node network.
	CClothNodeNetwork				mClothNodeNetwork;

	// External force to be applied to the cloth - gravity.
	CGravityNodeForce				mGravityForce;

	// External force to be applied to the cloth - wind.
	CWindNodeForce					mWindForce;

	// External force to be applied to the cloth - viscous damping.
	CViscousNodeForce				mViscousForce;

	// Collection of external forces (facilitates net external node
	// force calculations).
	CExternalNodeForceCollection	mExternalForceCollection;

	// Scroller text color.
	CFloatColor						mScrollerTextColor;

	// Scroller background color.
	CFloatColor						mScrollerBackgroundColor;

	// Backdrop mesh.
	COpenGLStaticMesh*				mpBackDropMesh;

	CFloatColor						mBackdropCylinderColor;

	static const QuantityType		mDefaultBDxGridPoints;
	static const QuantityType		mDefaultBDyGridPoints;
	static const ScalarType			mDefaultCylinderRadius;
	static const ScalarType			mDefaultCylinderHeight;
};


#endif	// #ifndef _CLOTHSCROLLERSCENESCOMMON_H