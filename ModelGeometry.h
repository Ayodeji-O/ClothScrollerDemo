//ModelGeometry.h - Contains declarations pertinent to geometric representation and management
//of three-dimensional objects and primitives.

#ifndef _MODELGEOMETRY_H
#define _MODELGEOMETRY_H			1

#include "PrimaryHeaders.h"
#include "ClothScrollerDefines.h"
#include "PrimitiveSupport.h"


class CStaticMeshBase
{
public:
	CStaticMeshBase();
	CStaticMeshBase(const PolygonList& polygonList);
	~CStaticMeshBase();

	//Stores geometry in the internal polygon list.
	bool							SetModelGeometry(const PolygonList& polygonList);

	//Sets the texture that is to be used for the model.
	bool							SetModelTexture(const CTextureCollection& modelTexture);

	//Retrieves the textures associated with the model.
	const CTextureCollection&		GetTextureCollection() const;

	//!!!Temporary...
	PolygonList						GetPolygonList() const { return(this->mPolygonList); }

protected:
	PolygonList						mPolygonList;

	CTextureCollection				mModelTexture;
};


class CGeneratedModel
{
public:
	CGeneratedModel();
	~CGeneratedModel();

	//Retrieves the internal mesh representation of the model.
	const CStaticMeshBase&			GetGeneratedMesh() const;

protected:
	//Generates the model.
	virtual bool					GenerateModel() = 0;

	//Internal mesh representation of the model.
	CStaticMeshBase					mGeneratedMesh;
};


#endif //#ifndef _MODELGEOMETRY_H