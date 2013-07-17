//ModelGeometry.cpp - Contains implementations pertinent to geometric representation and management

#include "ModelGeometry.h"

/////
// CStaticMeshBase
/////
CStaticMeshBase::CStaticMeshBase()
{
}


CStaticMeshBase::CStaticMeshBase(const PolygonList& polygonList)
{
	::AssertExpression(this->SetModelGeometry(polygonList));
}


CStaticMeshBase::~CStaticMeshBase()
{
}


bool CStaticMeshBase::SetModelGeometry(const PolygonList& polygonList)
{
	bool							bModelGeometrySet = false;

	if(!polygonList.empty()) {
		//Store the polygon list in the static mesh class.
		this->mPolygonList = polygonList;

		bModelGeometrySet = true;
	}

	return(bModelGeometrySet);
}

bool CStaticMeshBase::SetModelTexture(const CTextureCollection& modelTexture)
{
	bool							bModelTextureSet = false;

	if (modelTexture.GetNumTextureLayers() > 0) {
		// Store the model texture.
		this->mModelTexture = modelTexture;

		bModelTextureSet = true;
	}

	return(bModelTextureSet);
}


const CTextureCollection& CStaticMeshBase::GetTextureCollection() const
{
	// Return the collection of textures.
	return(this->mModelTexture);
}


/////
// CGeneratedModel
/////

CGeneratedModel::CGeneratedModel()
{
}

CGeneratedModel::~CGeneratedModel()
{
}

const CStaticMeshBase& CGeneratedModel::GetGeneratedMesh() const
{
	//Return the generated mesh.
	return(this->mGeneratedMesh);
}



