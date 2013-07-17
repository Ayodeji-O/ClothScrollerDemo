#include "ModelGeometryOpenGL.h"


#if _DEBUG
//Debugging pre-processor definition - when this flag is enabled, vertex normals
//on mesh objects will be rendered.
#define RENDER_VERTEX_NORMALS 0
#endif	//#if _DEBUG

COpenGLStaticMesh::COpenGLStaticMesh(const CStaticMeshBase& staticMesh) :
	mModelReferenceOrigin(0.0, 0.0, 0.0), mDisplayListReference(0),
		mbDisplayListReferenceSet(false)

{
	//Generate a display list from the static mesh.
	this->GenerateDisplayListFromMesh(staticMesh);
}


COpenGLStaticMesh::COpenGLStaticMesh(const CGeneratedModel& staticModel) :
	mModelReferenceOrigin(0.0, 0.0, 0.0), mDisplayListReference(0),
		mbDisplayListReferenceSet(false)
{
	this->mTextureLayerReferenceList.clear();

	//Retrieve the mesh from the model, and generate a display list from the
	//retrieved mesh.
	const CStaticMeshBase&			staticMesh = staticModel.GetGeneratedMesh();
	this->GenerateDisplayListFromMesh(staticMesh);
}

COpenGLStaticMesh::~COpenGLStaticMesh()
{
	//Destroy the created OpenGL display list, if the corresponding list
	//exists.
	if(this->IsDisplayListReferenceValid()) {
		::glDeleteLists(this->GetDisplayListReference(), 1);
	}

	// Release resources allocated for textures
	if (this->GetNumAllocatedTextures() > 0) {
		this->ReleaseTextureLayers();
	}
}

bool COpenGLStaticMesh::RenderMesh() const
{
	bool							bMeshRenderSuccessful = false;

	if(this->IsDisplayListReferenceValid()) {
		//Retrieve the current mesh position.
		const CFloatPoint			currentMeshPosition =
			this->GetMeshPosition();

		//Position the object according to the current position specification, first
		//set the matrix mode to the "model view", and set the model matrix to an
		//identity matrix...
		::glMatrixMode(GL_MODELVIEW);
		//Save the old view matrix...
		::glPushMatrix();
		//...Next, set the absolute translation.
		::glTranslated(currentMeshPosition.GetXCoord(), currentMeshPosition.GetYCoord(),
			currentMeshPosition.GetZCoord());
		//Render the representative OpenGL display list.
		::glCallList(this->GetDisplayListReference());
		//..Restore the view matrix.
		::glPopMatrix();

		bMeshRenderSuccessful = true;
	}

	return(bMeshRenderSuccessful);
}


const CFloatPoint& COpenGLStaticMesh::GetMeshPosition() const
{
	//Return the current mesh psoition (reference origin).
	return(this->mModelReferenceOrigin);
}

void COpenGLStaticMesh::SetMeshPosition(const CFloatPoint& newMeshPosition)
{
	//Set the new absolute mesh position.
	this->mModelReferenceOrigin = newMeshPosition;
}


void COpenGLStaticMesh::TranslateMesh(const CVector& translationVector)
{
	//Translate the mesh by the specified amount.
	this->mModelReferenceOrigin = this->mModelReferenceOrigin + 
		CFloatPoint(translationVector.GetXComponent(), 
		translationVector.GetYComponent(), translationVector.GetZComponent());
}

bool COpenGLStaticMesh::GenerateDisplayListFromMesh(const CStaticMeshBase& staticMesh)
{
	bool							bListGeneratedSuccessfully = false;


	//Obtain a list of polygons from the mesh...
	PolygonList						meshPolyList = staticMesh.GetPolygonList();	

	if (!meshPolyList.empty()) {
		//Number of display lists to generate.
		const QuantityType			kNumDisplayLists = 1;

		//Obtain a display list reference...
		const displayListReference = ::glGenLists(kNumDisplayLists);

		//Create a display list (GL_COMPILE indicates that the list will
		//be created, but not rendered immediately).
		::glNewList(displayListReference, GL_COMPILE);

		// Create texture(s) and bind the texture(s), allowing the texture/
		// textures to be used.
		::glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		GLuint						textureID;
		::glGenTextures(1, &textureID);

		// Store the texture reference/identifier.
		this->AddTextureLayerReference(textureID);

		::glBindTexture(GL_TEXTURE_2D, textureID);

		::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


		const CTextureCollection&	textureCollection = staticMesh.GetTextureCollection();
		const CBaseTextureData*		pTextureInformation = textureCollection.GetTextureLayer(0);

		if (pTextureInformation) {
			const void*				pRawTextureData = pTextureInformation->GetTextureData();

			if (pRawTextureData) {
				::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pTextureInformation->GetTextureWidth(),
					pTextureInformation->GetTextureHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, pRawTextureData);
			}

			::glBindTexture(GL_TEXTURE_2D, textureID);
		}

		//Iterate through the list of mesh polygons, adding the polygons
		//to the created OpenGL display list.
		for(PolygonList::iterator polylistIterator = meshPolyList.begin();
			polylistIterator != meshPolyList.end(); polylistIterator++)
		{
			//Obtain the vertex list from the current polygon...
			CFloatPolygon::VertexList	
									vertexList  = polylistIterator->GetVertexList();

			//Create an OpenGl polygon.
			::glBegin(GL_POLYGON);
			for(CFloatPolygon::VertexList::iterator vertexListIterator = vertexList.begin();
				vertexListIterator != vertexList.end(); vertexListIterator++)
			{
				//Set the material properties (basic lighting model interactions -
				//ambient, diffuse, specular, emissive) for the polygon..

				const ScalarType	kMaxSpecularExponent = 128.0;

				//Ambient color.
				CFloatColor			ambientColor =
					polylistIterator->GetLightShading(eShadeAmbient);
				GLfloat				ambientMaterial[] = { ambientColor.GetRedValue(),
					ambientColor.GetGreenValue(), ambientColor.GetBlueValue(), 1.0 };
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientMaterial);

				//Diffuse color.
				CFloatColor			diffuseColor =
					polylistIterator->GetLightShading(eShadeDiffuse);
				GLfloat				diffuseMaterial[] = { diffuseColor.GetRedValue(),
					diffuseColor.GetGreenValue(), diffuseColor.GetBlueValue(), 1.0 };
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseMaterial);

				//Emissive color.
				CFloatColor			emissiveColor =
					polylistIterator->GetLightShading(eShadeEmissive);
				GLfloat				emissiveMaterial[] = { emissiveColor.GetRedValue(),
					emissiveColor.GetGreenValue(), emissiveColor.GetBlueValue(), 1.0 };
				glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emissiveMaterial);			

				//Specular color.
				CFloatColor			specularColor =
					polylistIterator->GetLightShading(eShadeSpecular);
				GLfloat				specularMaterial[] = { specularColor.GetRedValue(), 
					specularColor.GetGreenValue(), specularColor.GetBlueValue(), 1.0 };
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularMaterial);
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,
					kMaxSpecularExponent * polylistIterator->GetSpecularity());
				
				//Retrieve the vertex normal.
				CVector				vertexNormal = vertexListIterator->GetNormal();

				//Store the color/alpha information.
				CFloatColor			vertexColor = vertexListIterator->GetColor();
				::glColor4d(vertexColor.GetRedValue(), vertexColor.GetGreenValue(),
					vertexColor.GetBlueValue(), vertexColor.GetAlphaValue());
				//Store the normal.
				::glNormal3d(vertexNormal.GetXComponent(), vertexNormal.GetYComponent(),
					vertexNormal.GetZComponent());

				//Texture coordinates
				ScalarType			uTextureCoordinate = 0.0;
				ScalarType			vTextureCoordinate = 0.0;
				vertexListIterator->GetTextureCoordinates(uTextureCoordinate, vTextureCoordinate);
				::glTexCoord2f(uTextureCoordinate, vTextureCoordinate);

				//Store the vertex coordinates.
				::glVertex3d(vertexListIterator->GetXCoord(), 
					vertexListIterator->GetYCoord(),
					vertexListIterator->GetZCoord());
			}
			::glEnd();

		#if RENDER_VERTEX_NORMALS
			for(CFloatPolygon::VertexList::iterator vertexNormIterator = vertexList.begin();
				vertexNormIterator != vertexList.end(); vertexNormIterator++)
			{
				//Retrieve the vertex normal.
				CVector				vertexNormal = vertexNormIterator->GetNormal();
				::glBegin(GL_LINES);
				//Render the normal vector (debugging purposes only).
				::glVertex3d(vertexNormIterator->GetXCoord(), 
					vertexNormIterator->GetYCoord(),
					vertexNormIterator->GetZCoord());
				::glVertex3d(vertexNormIterator->GetXCoord() + vertexNormal.GetXComponent(), 
					vertexNormIterator->GetYCoord() + vertexNormal.GetYComponent(),
					vertexNormIterator->GetZCoord() + vertexNormal.GetZComponent());
				::glEnd();
			}
		#endif	//#if RENDER_VERTEX_NORMALS
		}

		// Unbind any texture objects.
		::glBindTexture(GL_TEXTURE_2D, 0);

		//Complete the display list.
		::glEndList();

		//Store the display list reference number.
		this->SetDisplayListReference(displayListReference);

		//Indicate a success condition. 
		bListGeneratedSuccessfully = true;
	}


	//Set the validity flag of the display list reference to
	//correspond to the display list creation result flag.
	this->mbDisplayListReferenceSet = bListGeneratedSuccessfully;

	return(bListGeneratedSuccessfully);
}

IndexType COpenGLStaticMesh::GetDisplayListReference() const
{
	//Return the stored display list number.
	return(this->mDisplayListReference);
}

void COpenGLStaticMesh::SetDisplayListReference(const IndexType displayListReference)
{
	//Store the diplay list number.
	this->mDisplayListReference = displayListReference;
}

bool COpenGLStaticMesh::IsDisplayListReferenceValid() const
{
	//Return the result of the display list creation routine.
	return(this->mbDisplayListReferenceSet);
}

void COpenGLStaticMesh::AddTextureLayerReference(const IndexType textureLayerReference)
{
	// Add the texture reference to the list of texture references.
	this->mTextureLayerReferenceList.push_back(textureLayerReference);
}


bool COpenGLStaticMesh::GetTextureLayerReference(const IndexType textureLayerIndex,
		IndexType& textureLayerReference) const
{
	bool											bReferenceRetrievedSuccessfully = false;

	if (textureLayerIndex < this->mTextureLayerReferenceList.size()) {
		// Retrieve the texture reference at the specified texture layer, if the
		// reference does not exceed the current number of texture layers.
		textureLayerReference = *(this->mTextureLayerReferenceList.begin() +
			textureLayerIndex);

		bReferenceRetrievedSuccessfully = true;
	}

	return(bReferenceRetrievedSuccessfully);
}


QuantityType COpenGLStaticMesh::GetNumAllocatedTextures() const
{
	// Return the size of the allocated texture list.
	return(this->mTextureLayerReferenceList.size());
}

bool COpenGLStaticMesh::ReleaseTextureLayers()
{
	bool							bTextureLayersReleased = false;

	GLuint							currentTextureLayerReference;

	while (!this->mTextureLayerReferenceList.empty()) {
		// Retrieve the reference of the last texture in the list...
		currentTextureLayerReference = this->mTextureLayerReferenceList.back();

		// Remove the reference from the list.
		this->mTextureLayerReferenceList.pop_back();

		// Delete the texture, freeing allocated resources..
		::glDeleteTextures(1, &currentTextureLayerReference);

		bTextureLayersReleased = true;
	}

	return(bTextureLayersReleased);
}