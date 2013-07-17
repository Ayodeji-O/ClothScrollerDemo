#include "PrimaryHeaders.h"
#include "ModelGeometry.h"

#ifndef _MODELGEOMETRYOPENGL_H
#define _MODELGEOMETRYOPENGL_H 1

class COpenGLStaticMesh
{
public:
	COpenGLStaticMesh(const CStaticMeshBase& staticMesh);
	COpenGLStaticMesh(const CGeneratedModel& staticMesh);
	~COpenGLStaticMesh();

	//Renders the OpenGL mesh, translating the obejct to the internal object reference origin.
	bool							RenderMesh() const;

	//Obtains the current mesh position (reference origin).
	const CFloatPoint&				GetMeshPosition() const;

	//Sets a new absolution mesh position (reference origin).
	void							SetMeshPosition(const CFloatPoint& newMeshPosition);

	//Translates the mesh by a specified distance.
	void							TranslateMesh(const CVector& translationVector);
protected:
	//Generates an internal OpenGL display list from the specified object mesh.
	bool							GenerateDisplayListFromMesh(const CStaticMeshBase& staticMesh);

	IndexType						GetDisplayListReference() const;

	void							SetDisplayListReference(const IndexType displayListReference);

	//Determines if the display list reference is valid.
	bool							IsDisplayListReferenceValid() const;

	// Stores a reference for a texture - this reference is stored implicitly as a separate texture
	// layer.
	void							AddTextureLayerReference(const IndexType textureLayerReference);

	// Retrieves a reference for a texture at a specified layer.
	bool							GetTextureLayerReference(const IndexType textureLayerIndex,
		IndexType& textureLayerReference) const;

	// Returns the number of textures currently allocated for the model.
	QuantityType					GetNumAllocatedTextures() const;

	// De-allocates memory allocated for texture objects.
	bool							ReleaseTextureLayers();

	//OpenGL Display list number.
	IndexType						mDisplayListReference;

	//TRUE if a display list has been created, and the reference has been successfully set.
	bool							mbDisplayListReferenceSet;

	// Type that defines references to layered textures.
	typedef std::vector<IndexType>	TextureLayerReferenceList;

	// List of texture layers.
	TextureLayerReferenceList		mTextureLayerReferenceList;

	//Position of the model.
	CFloatPoint						mModelReferenceOrigin;
};


#endif	//#ifndef _MODELGEOMETRYOPENGL_H