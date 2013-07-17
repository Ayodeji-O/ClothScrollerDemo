//PrimitiveSupport.h - Contains coordinate/geometry management-related declarations

#ifndef	_PRIMITIVESUPPORT_H
#define _PRIMITIVESUPPORT_H			1

#include "PrimaryHeaders.h"
#include "ClothScrollerDefines.h"


//Define internal floating point coordinate type and scalar type...
#if DOUBLE_PRECISION_INTERNALCOORDS
#define FloatCoord double
#define ScalarType double
#else	//#if DOUBLE_PRECISION_INTERNALCOORDS
#define	FloatCoord float
#define ScalarType float
#endif	//#if DOUBLE_PRECISION_INTERNALCOORDS...#else

//Define integral indexing / quantity type
#define IndexType unsigned long
#define RelativeIndexType long
#define QuantityType IndexType
#define RelativeQuantityType RelativeIndexType

//Define rendering (output) floating point coordinate type.
#if DOUBLE_PRECISION_RENDERCOORDS
#define RenderFloatCoord double
#else	//#if DOUBLE_PRECISION_RENDERCOORDS
#define RenderFloatCoord float
#endif	//#if DOUBLE_PRECISION_RENDERCOORDS...#else


//Constants
//PI
const FloatCoord					kPI = (FloatCoord)3.1415926535897932;
//2 x PI
const FloatCoord					kTwoPi = 2.0 * kPI;



//Representation of a point in three-dimensional space.
class CFloatThreeSpaceBase
{
public:
	CFloatThreeSpaceBase(const FloatCoord xValue,
		const FloatCoord yValue,
		const FloatCoord zValue);
	~CFloatThreeSpaceBase();

	//Coordinate operators (convenience functions)

#if 0
	//Multiplies all contained point coordinate values by a scalar.
	virtual CFloatThreeSpaceBase operator*(const ScalarType scalarValue) const;
	//Divides all contained point coordinate values by a scalar.
	virtual CFloatThreeSpaceBase operator/(const ScalarType scalarValue) const;
#endif	//#if 0 - Subclass conflict in resolution.
	//Displaces a point coordinate by a specified coordinate offset (addition).
	virtual CFloatThreeSpaceBase operator+(const CFloatThreeSpaceBase& scalarValue) const;
	//Displaces a point coordinate by a specified coordinate offset (subtraction).
	virtual CFloatThreeSpaceBase operator-(const CFloatThreeSpaceBase& scalarValue) const;

protected:
	//Obtains the contained x-constituent of the three-space object.
	FloatCoord						GetX() const;
	//Obtains the contained y-constituent of the three-space object.
	FloatCoord						GetY() const;
	//Obtains the contained z-constituent of the three-space object.
	FloatCoord						GetZ() const;

	//Sets the the contained x-constituent of the three-space object.
	void							SetX(const FloatCoord xValue); 
	//Sets the the contained y-constituent of the three-space object.
	void							SetY(const FloatCoord yValue);
	//Sets the the contained z-constituent of the three-space object.
	void							SetZ(const FloatCoord zValue);

private:
	FloatCoord						mXValue;					//X Constituent
	FloatCoord						mYValue;					//Y Constituent
	FloatCoord						mZValue;					//Z Constituent
};


//Representation of a point in three-dimensional space.
class CFloatPoint : public CFloatThreeSpaceBase
{
public:
	CFloatPoint(const FloatCoord xCoord,
		const FloatCoord yCoord,
		const FloatCoord zCoord);
	~CFloatPoint();

	//Obtains the x-coordinate of the represented point in three-dimensional space.
	FloatCoord						GetXCoord() const;
	//Obtains the y-coordinate of the represented point in three-dimensional space. 
	FloatCoord						GetYCoord() const;
	//Obtains the z-coordinate of the represented point in three-dimensional space.
	FloatCoord						GetZCoord() const;

	//Determines the distance of the represented point from another point in three-
	//dimensional space.
	ScalarType						DistanceFrom(const CFloatPoint& referencePoint) const;

	//Coordinate operators (convenience functions)

	//Multiplies all contained point coordinate values by a scalar.
	CFloatPoint operator*(const ScalarType scalarValue) const;
	//Displaces a point coordinate by a specified coordinate offset (addition).
	CFloatPoint operator+(const CFloatPoint& scalarValue) const;
	//Displaces a point coordinate by a specified coordinate offset (subtraction).
	CFloatPoint operator-(const CFloatPoint& scalarValue) const;

protected:

};

const ScalarType					kMaxColorComponentValue = 1.0;
const ScalarType					kMinColorComponentValue = 0.0;
const ScalarType					kMaxSpecularityValue = 1.0;
const ScalarType					kMinSpecularityValue = 0.0;
const ScalarType					kMaxTextureCoordinateValue = 1.0;
const ScalarType					kMinTextureCoordinateValue = 0.0;


//Representation of a RGBA color value.
class CFloatColor
{
public:
	explicit CFloatColor(const ScalarType red, const ScalarType green,
		const ScalarType blue);
	explicit CFloatColor(const ScalarType red, const ScalarType green,
		const ScalarType blue, const ScalarType alpha);
	~CFloatColor();

	//Retrieves the red component of the color.
	ScalarType						GetRedValue() const;
	//Retrieves the green component of the color.
	ScalarType						GetGreenValue() const;
	//Retrieves the blue component of the color.
	ScalarType						GetBlueValue() const;
	//Retrieves the alpha component of the color.
	ScalarType						GetAlphaValue() const;

protected:
	//Clamps a component value between 0.0 and 1.0.
	ScalarType						ClampComponent(const ScalarType componentValue) const;

	//Red component
	ScalarType						mRed;
	//Green component
	ScalarType						mGreen;
	//Blue component
	ScalarType						mBlue;
	//Alpha component
	ScalarType						mAlpha;
};

// Representation of a texture.
class CBaseTextureData
{
public:
	CBaseTextureData();
	CBaseTextureData(const QuantityType textureWidth, const QuantityType textureHeight,
		const QuantityType colorDepth, const QuantityType dataSize, void* pTextureData);
	~CBaseTextureData();

	// Obtains the width of the texture.
	QuantityType					GetTextureWidth() const;

	// Obtains the height of the texture.
	QuantityType					GetTextureHeight() const;

	// Retrieves the color depth of the texture (bits per pixel).
	QuantityType					GetTextureColorDepth() const;

	// Retrieves the size of the actual texture image data.
	QuantityType					GetTextureDataSize() const;

	// Creates a texture of the specified size.
	bool							CreateTexture(const QuantityType width,
		const QuantityType height, const QuantityType bitsPerPixel);

	// Obtains a pointer to the texture data.
	const void*						GetTextureData() const;

	// Determines if the object is valid in accordance with its defining
	// parameters.
	bool							IsValid() const;

	// (Copy operator)
	CBaseTextureData				operator=(const CBaseTextureData& textureData);

protected:

	// If true, the texture memory was allocated by the texture
	// representation object.
	const bool						mbOwnedByObject;

	// Pointer to the texture data.
	void*							mpTextureData;

	// Width of the texture.
	QuantityType					mTextureWidth;

	// Height of the texture.
	QuantityType					mTextureHeight;

	// Color resolution of texture.
	QuantityType					mColorBitsPerPixel;

	// Size of the texture data.
	QuantityType					mTextureDataSize;
};

// Representation of a layered texture.
class CTextureCollection
{
public:
	CTextureCollection();
	~CTextureCollection();

	// Adds a layer to the texture collection.
	bool							AddTextureLayer(const CBaseTextureData& textureData);

	// Removes a texture layer from the texture list.
	bool							RemoveTextureLayer(const IndexType textureLayerIndex);

	// Retrieves the number of texture layers present in the collection.
	IndexType						GetNumTextureLayers() const;

	// Retrieves texture information for a specified texture layer.
	const CBaseTextureData*			GetTextureLayer(const IndexType textureLayer) const;

protected:

	typedef std::vector<CBaseTextureData>
									TextureList;


	// Sequentially-ordered list of texture layers.
	TextureList						mTextureList;
};

//Representation of a three-component vector.
class CVector : public CFloatThreeSpaceBase
{
public:
	CVector();
	CVector(const FloatCoord xComponent,
		const FloatCoord yComponent,
		const FloatCoord zComponent);
	~CVector();

	//Obtains the x-component of the vector;
	FloatCoord						GetXComponent() const;
	//Obtains the y-component of the vector;
	FloatCoord						GetYComponent() const;
	//Obtains the z-component of the vector;
	FloatCoord						GetZComponent() const;

	//Determines the magnitude of the vector.
	ScalarType						GetMagnitude() const;

	//Converts the vector to a unit vector.
	void							Normalize();

	// Creates a vector from the displacement of two points.
	void							SetAsDisplacementVector(const CFloatPoint&
		vectorHeadPoint, const CFloatPoint& vectorTailPoint);

	// Vector operators

	// Multiplies all vector components by a scalar, effectively multiplying the
	// vector by a scalar.
	CVector operator*(const ScalarType& scalarValue) const;

	// Divides all vector components by a scalar, effectively dividing the vector
	// by a scalar.
	CVector operator/(const ScalarType& scalarValue) const;

	// Produces the sum of two vectors (addition).
	CVector operator+(const CVector& secondVector) const;

	// Produces the difference of two vectors (subtraction).
	CVector operator-(const CVector& secondVector) const;

private:

};

//Representation of a point on a polygon in three-dimensional space.
class CPolygonVertex : public CFloatPoint
{
public:

	CPolygonVertex();
	CPolygonVertex(const FloatCoord xCoord,
		const FloatCoord yCoord,
		const FloatCoord zCoord);
	CPolygonVertex(const FloatCoord xCoord,
		const FloatCoord yCoord,
		const FloatCoord zCoord,
		const CVector& vertexNormal,
		const CFloatColor& floatColor = CFloatColor(kMaxColorComponentValue,
		kMaxColorComponentValue, kMaxColorComponentValue),
		const ScalarType uTextureCoord = 0.0, const ScalarType vTextureCoord = 0.0);
	CPolygonVertex(const CFloatPoint& floatCoords);
	CPolygonVertex(const CFloatPoint& floatCoords, const CVector& vertexNormal,
		const CFloatColor& floatColor = CFloatColor(kMaxColorComponentValue,
		kMaxColorComponentValue, kMaxColorComponentValue),
		const ScalarType uTextureCoord = 0.0, const ScalarType vTextureCoord = 0.0);
	~CPolygonVertex();

	//Obtains the normal associated with the vertex.
	CVector							GetNormal() const;

	//Sets the normal associated with the vertex. The normal
	//will be conveted to a unit normal.
	void							SetNormal(const CVector& vertexNormal);

	//Obtains the vertex color.
	CFloatColor						GetColor() const;

	//Obtains the texture coordinate.
	void							GetTextureCoordinates(ScalarType& uCoord,
		ScalarType& vCoord) const;

	//Sets the texture coordinate.
	void							SetTextureCoordinates(const ScalarType uCoord,
		const ScalarType vCoord);

protected:

	// Clamps texture coordinates between 0.0 and 1.0;
	ScalarType						ClampTextureCoord(const ScalarType textureCoordinate);

	//Per-vertex normal
	CVector							mVertexNormal;

	//Vertex color
	CFloatColor						mVertexColor;

	//Texture coordinate.
	ScalarType						muTextureCoord;
	ScalarType						mvTextureCoord;
};


//Surface interaction specifications - determines surface
//shading for lighting interactions
typedef enum
{
	eShadeAmbient = 0,
	eShadeDiffuse,
	eShadeSpecular,
	eShadeEmissive
} EBasicLightShading;

//Representation of a polygon in three-dimensional space.
class CFloatPolygon
{
public:
	//Construct that represents an ordered list of vertices.
	typedef std::vector<CPolygonVertex>
								VertexList;

	CFloatPolygon();
	CFloatPolygon(const VertexList& vertexList);
	~CFloatPolygon();

	//Obtains the vertex count of the polygon.
	IndexType					GetVertexCount() const;

	//Adds a vertex to the end of the polygon vertex list.
	void						AddVertex(const CPolygonVertex& polygonVertex);

	//Deletes an indexed vertex from the internal list of
	//polygon vertices.
	bool						DeleteVertex(const IndexType vertexIndex);

	//!!!Temporary
	VertexList					GetVertexList() const {return(mVertexList);}


	//Retrieves the surface color contribution for the particular basic lighting model.
	CFloatColor					GetLightShading(const EBasicLightShading lightShadingType);

	//Sets the surface color for the particular basic lighting model.
	void						SetLightShading(const EBasicLightShading lightShadingType,
		const CFloatColor& lightShadingColor);

	//Retrieves the specularity of a surface.
	ScalarType					GetSpecularity() const;

	//Sets the specularity ("shininess") of a surface. Values outside the range
	//of 0...1 are clamped to the appropriate value.
	void						SetSpecularity(const ScalarType specularity);

protected:
	//Internal list of polygon vertices.
	VertexList					mVertexList;

	//Ambient lighting contribution color.
	CFloatColor					mColorShadeAmbient;
	//Diffuse lighting contribution color.
	CFloatColor					mColorShadeDiffuse;
	//Specular lighting contribution color
	CFloatColor					mColorShadeSpecular;
	//Emissive lighting contribution color
	CFloatColor					mColorShadeEmissive;

	//Specularity  (0.0 - 1.0)
	ScalarType					mSpecularity;
};

//Type definition - list of polygons (should ultimately be abstracted/wrapped by a
//class).
typedef std::vector<CFloatPolygon>
								PolygonList;

const int						kDefaultTesselationFactor = 1;
class CVertexGrid
{
public:
	CVertexGrid(const IndexType width, const IndexType height);
	virtual ~CVertexGrid();

	//Returns the height of the grid.
	IndexType					GetGridWidth() const;
	//Returns the width of the grid.
	IndexType					GetGridHeight() const;

	//Adds vertex to the grid at the specified index.
	bool						AddVertexToGrid(const IndexType xIndex,
		const IndexType yIndex, const CPolygonVertex& polygonVertex);

	//Generates a list of polygons from the internal grid.
	//The tesselation factor begins with a value of 1, with
	//higher values providing increased tesselation degrees.
	bool						GeneratePolygonsFromGrid(PolygonList& polygonList,
		const IndexType tesselationFactor = kDefaultTesselationFactor) const;

	//Determines if the grid is valid (i.e. dimensions, vertex
	//allocation).
	bool						IsGridValid() const;

protected:
	//Width of the grid (number of elements per row).
	IndexType					mGridWidth;
	//Height of the grid (number of elements per column).
	IndexType					mGridHeight;


	//Vertex grid structure.
	CStatic2DArray<CPolygonVertex>
								mVertexGrid;
};


//Support routines

//Computes the cross product of two vectors.
CVector VectorCrossProduct(const CVector& vectorA, const CVector& vectorB);


#endif	//#ifndef	_PRIMITIVESUPPORT_H