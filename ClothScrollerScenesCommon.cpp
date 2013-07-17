
#include "ClothScrollerScenesCommon.h"


CBaseClothScrollerSceneParamBlock::CBaseClothScrollerSceneParamBlock(
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
		const CFloatColor& backDropCylinderColor) : 
	
	mNodesPerRow(nodesPerRow),
	mNodesPerColumn(nodesPerColumn),
	mNodeNetworkWidth(nodeNetworkWidth),
	mNodeNetworkHeight(ndoeNetworkHeight),
	mNetworkCoordOffset(networkCoordOffset),
	mNodeMass(nodeMass),
	mStructSpringConstant(structSpringConstant),
	mShearSpringConstant(shearSpringConstant),
	mFlexionSpringConstant(flexionSpringConstant),
	mGravitationalConstant(gravitationalConstant),
	mGravityVector(gravityVector),
	mSimulationTimeQuantum(simulationTimeQuantum),
	mClothColor(clothColor),
	mClothSpecularity(clothSpecularity),
	mMaxWindMagnitude(maxWindMagnitude),
	mWindDirection(windDirection),
	mViscousCoefficient(viscousCoefficient),
	mBackdropCylinderColor(backDropCylinderColor)
{

}

CBaseClothScrollerSceneParamBlock::~CBaseClothScrollerSceneParamBlock()
{

}

CBaseClothScrollerScene::CBaseClothScrollerScene(const CBaseClothScrollerSceneParamBlock&
												 initParameterBlock) :
	mClothNodeNetwork(initParameterBlock.mNodesPerRow, initParameterBlock.mNodesPerColumn,
		initParameterBlock.mNodeNetworkWidth, initParameterBlock.mNodeNetworkHeight,
		initParameterBlock.mNetworkCoordOffset, initParameterBlock.mNodeMass,
		initParameterBlock.mStructSpringConstant, initParameterBlock.mShearSpringConstant,
		initParameterBlock.mFlexionSpringConstant),
	mGravityForce(initParameterBlock.mGravitationalConstant, initParameterBlock.mGravityVector),
	mWindForce(initParameterBlock.mWindDirection, initParameterBlock.mMaxWindMagnitude),
	mViscousForce(initParameterBlock.mViscousCoefficient),
	mScrollerTextColor(0.0, 0.0, 0.0), mScrollerBackgroundColor(1.0, 1.0, 1.0),
	mpBackDropMesh(NULL), mBackdropCylinderColor(initParameterBlock.mBackdropCylinderColor)
{
	//Add external forces to the external force collection.
	this->BuildExternalForceCollection();
}


CBaseClothScrollerScene::~CBaseClothScrollerScene()
{


}


bool CBaseClothScrollerScene::BuildExternalForceCollection()
{
	// Add gravity, wind and viscous damping forces to the collection of forces to be
	// applied to the cloth nodes.
	bool							bForceCollectionBuiltSuccessfully =
		(this->mExternalForceCollection).AddExternalNodeForce(this->mGravityForce) &&
		(this->mExternalForceCollection).AddExternalNodeForce(this->mWindForce) &&
		(this->mExternalForceCollection).AddExternalNodeForce(this->mViscousForce);

	return(bForceCollectionBuiltSuccessfully);
}

bool CBaseClothScrollerScene::AddScrollerTextureLayer(CTextureCollection& textureCollection) const
{
	bool							bLayerAddedSuccessfully = false;

	// Retrieve a scroller image pointer from the scroller manager.
	void*							pScrollerImagePointer = NULL;
	CScrollerManager::SetScrollerBackgroundColor(this->mScrollerBackgroundColor);
	CScrollerManager::SetScrollerTextColor(this->mScrollerTextColor);
	CScrollerManager::GetScrollerImageDataPointer(pScrollerImagePointer);

	// Retrieve the image format from the scroller manager (dimensions, image size,
	// bit depth)...
	QuantityType					textureWidth = 0;
	QuantityType					textureHeight = 0;
	QuantityType					textureBitDepth = 0;
	QuantityType					textureDataSize = 0;
	if (CScrollerManager::GetScrollerImageFormat(textureWidth, textureHeight,
		textureBitDepth, textureDataSize))
	{
		// Create a texture object using the retrieve texture, and add it to the
		// cloth mesh.
		CBaseTextureData				clothTexture(textureWidth, textureHeight,
			textureBitDepth, textureDataSize, pScrollerImagePointer);

		textureCollection.AddTextureLayer(clothTexture);
	}

	return(bLayerAddedSuccessfully);
}


bool CBaseClothScrollerScene::RenderClothNetworkWithScrollerTexture() const
{
	bool							bClothRenderedSuccessfully = false;

	// Create a mesh for the current cloth network...
	CStaticMeshBase					clothMesh;
	this->mClothNodeNetwork.GenerateNodeNetworkMesh(clothMesh);

	// Add the scroller texture to the cloth mesh.
	CTextureCollection				clothTextureCollection;
	this->AddScrollerTextureLayer(clothTextureCollection);

	clothMesh.SetModelTexture(clothTextureCollection);

	// Create an OpenGL display list, and render the cloth geometry.
	COpenGLStaticMesh				clothClothDisplayMesh(clothMesh);
	bClothRenderedSuccessfully = clothClothDisplayMesh.RenderMesh();

	return(bClothRenderedSuccessfully);
}

bool CBaseClothScrollerScene::SetupBackDrop()
{
	bool							bSetupSuccessfully = false;
	
	// Create the backdrop mesh if it does not already exist...
	if (!this->mpBackDropMesh) {
		CGeneratedCylinder			backDropCylinder =
			CGeneratedCylinder(CBaseClothScrollerScene::mDefaultBDxGridPoints,
			CBaseClothScrollerScene::mDefaultBDyGridPoints,
			CBaseClothScrollerScene::mBackdropCylinderColor,
			CBaseClothScrollerScene::mDefaultCylinderRadius,
			CBaseClothScrollerScene::mDefaultCylinderHeight);

		mpBackDropMesh = new COpenGLStaticMesh(backDropCylinder);

		bSetupSuccessfully = (mpBackDropMesh != NULL);

	}

	return(bSetupSuccessfully);
}


const QuantityType					CBaseClothScrollerScene::mDefaultBDxGridPoints = 10;
const QuantityType					CBaseClothScrollerScene::mDefaultBDyGridPoints = 10;
const ScalarType					CBaseClothScrollerScene::mDefaultCylinderRadius = 15.0;
const ScalarType					CBaseClothScrollerScene::mDefaultCylinderHeight = 5000.0;
