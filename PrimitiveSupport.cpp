//PrimitiveSupport.cpp - Contains coordinate/geometry management-related definitions

#include "PrimaryHeaders.h"
#include "PrimitiveSupport.h"



/////
// CFloatThreeSpaceBase class
/////
CFloatThreeSpaceBase::CFloatThreeSpaceBase(const FloatCoord xValue, const FloatCoord yValue,
										   const FloatCoord zValue) :
	mXValue(xValue), mYValue(yValue), mZValue(zValue)
{

}

CFloatThreeSpaceBase::~CFloatThreeSpaceBase()
{
}

#if 0
//Multiplies all contained three-space values by a scalar.
CFloatThreeSpaceBase CFloatThreeSpaceBase::operator*(const ScalarType scalarValue) const
{
	//Multiply the individual coordinate values by a scalar...
	const FloatCoord				newXCoord = this->GetX() * scalarValue;
	const FloatCoord				newYCoord = this->GetY() * scalarValue;
	const FloatCoord				newZCoord = this->GetZ() * scalarValue;

	//Return a new point representation created from the altered coordinates.
	return(CFloatThreeSpaceBase(newXCoord, newYCoord, newZCoord));
}

//Divides all contained three-space values by a scalar.
CFloatThreeSpaceBase CFloatThreeSpaceBase::operator/(const ScalarType scalarValue) const
{
	const ScalarType				kUnity = scalarValue;
	ScalarType reciprocalScalar = 0;

	if (::AssertExpression(scalarValue != 0.0)) {
		reciprocalScalar = kUnity / scalarValue;
	}

	//Invoke the multiplication operator, using the reciprocal of the divisor as an operand.
	return((*this) * reciprocalScalar);
}
#endif	//#if 0

//Displaces a three-space triple by a specified coordinate offset (addition).
CFloatThreeSpaceBase CFloatThreeSpaceBase::operator+(const CFloatThreeSpaceBase& scalarValue) const
{
	//Add the individual coordinate values, using the input point representation as an offset.
	const FloatCoord				newXCoord = this->GetX() + scalarValue.GetX();
	const FloatCoord				newYCoord = this->GetY() + scalarValue.GetY();
	const FloatCoord				newZCoord = this->GetZ() + scalarValue.GetZ();

	//Return a new point representation created from the altered coordinates.
	return(CFloatThreeSpaceBase(newXCoord, newYCoord, newZCoord));
}

//Displaces a three-space triple by a specified coordinate offset (subtraction).
CFloatThreeSpaceBase CFloatThreeSpaceBase::operator-(const CFloatThreeSpaceBase& scalarValue) const
{
	//Negate the constituent values of the coordinate offset, and 
	return((*this) + CFloatThreeSpaceBase(-scalarValue.GetX(), -scalarValue.GetY(),
		-scalarValue.GetZ()));
}


//Obtains the contained x-constituent of the three-space object.
FloatCoord CFloatThreeSpaceBase::GetX() const
{
	return(this->mXValue);
}

//Obtains the contained y-constituent of the three-space object.
FloatCoord CFloatThreeSpaceBase::GetY() const
{
	return(this->mYValue);
}

//Obtains the contained z-constituent of the three-space object.
FloatCoord CFloatThreeSpaceBase::GetZ() const
{
	return(this->mZValue);
}


//Sets the x-constituent of a three-space triple.
void CFloatThreeSpaceBase::SetX(const FloatCoord xValue)
{
	this->mXValue = xValue;
}

//Sets the y-constituent of a three-space triple.
void CFloatThreeSpaceBase::SetY(const FloatCoord yValue)
{
	this->mYValue = yValue;
}

//Sets the z-constituent of a three-space triple.
void CFloatThreeSpaceBase::SetZ(const FloatCoord zValue)
{
	this->mZValue = zValue;
}

/////
// CFloatPoint class
/////

CFloatPoint::CFloatPoint(const FloatCoord xCoord, const FloatCoord yCoord,
						 const FloatCoord zCoord) :
	CFloatThreeSpaceBase(xCoord, yCoord, zCoord)
{
	//Initialize coordinate values (assignment list).
}


CFloatPoint::~CFloatPoint()
{
}

//Retrieves the X coordinate of the point in three-dimensional space.
FloatCoord CFloatPoint::GetXCoord() const
{
	return(this->GetX());
}

//Retrieves the Y coordinate of the point in three-dimensional space.
FloatCoord CFloatPoint::GetYCoord() const
{
	return(this->GetY());
}

//Retrieves the Z coordinate of the point in three-dimensional space.
FloatCoord CFloatPoint::GetZCoord() const
{
	return(this->GetZ());
}


ScalarType	CFloatPoint::DistanceFrom(const CFloatPoint& referencePoint) const
{
	// Obtain per-coordinate deltas for each point, and evaluate the
	// distance between the points (distance =
	// (dx^2 + dy^x + dz^2) ^ 1/2).
	const FloatCoord	deltaX = this->GetXCoord() -
		referencePoint.GetXCoord();
	const FloatCoord	deltaY = this->GetYCoord() -
		referencePoint.GetYCoord();
	const FloatCoord	deltaZ = this->GetZCoord() -
		referencePoint.GetZCoord();

	return(::sqrt((deltaX * deltaX) + (deltaY * deltaY) + (deltaZ * deltaZ)));
}


CFloatPoint CFloatPoint::operator*(const ScalarType scalarValue) const
{
	//Multiply the individual coordinate values by a scalar...
	const FloatCoord				newXCoord = this->GetXCoord() * scalarValue;
	const FloatCoord				newYCoord = this->GetYCoord() * scalarValue;
	const FloatCoord				newZCoord = this->GetZCoord() * scalarValue;

	//Return a new point representation created from the altered coordinates.
	return(CFloatPoint(newXCoord, newYCoord, newZCoord));
}

//Displaces a three-space triple by a specified coordinate offset (addition).
CFloatPoint CFloatPoint::operator+(const CFloatPoint& scalarValue) const
{
	//Add the individual coordinate values, using the input point representation as an offset.
	const FloatCoord				newXCoord = this->GetXCoord() +
		scalarValue.GetXCoord();
	const FloatCoord				newYCoord = this->GetYCoord() +
		scalarValue.GetYCoord();
	const FloatCoord				newZCoord = this->GetZCoord() +
		scalarValue.GetZCoord();

	//Return a new point representation created from the altered coordinates.
	return(CFloatPoint(newXCoord, newYCoord, newZCoord));
}

//Displaces a three-space triple by a specified coordinate offset (subtraction).
CFloatPoint CFloatPoint::operator-(const CFloatPoint& scalarValue) const
{
	//Negate the constituent values of the coordinate offset, and sum the
	//coordinates.
	return((*this) + CFloatPoint(-scalarValue.GetX(), -scalarValue.GetY(),
		-scalarValue.GetZ()));
}


/////
// CFloatColor class
/////

CFloatColor::CFloatColor(const ScalarType red, const ScalarType green,
						 const ScalarType blue)
{
	//Set the color component values (alpha will default to 1.0).
	this->mRed = this->ClampComponent(red);
	this->mGreen = this->ClampComponent(green);
	this->mBlue = this->ClampComponent(blue);
	this->mAlpha = this->ClampComponent(kMaxColorComponentValue);
}

CFloatColor::CFloatColor(const ScalarType red, const ScalarType green,
						 const ScalarType blue, const ScalarType alpha)
{
	//Set the color component values, as well as the alpha value.
	this->mRed = this->ClampComponent(red);
	this->mGreen = this->ClampComponent(green);
	this->mBlue = this->ClampComponent(blue);
	this->mAlpha = this->ClampComponent(alpha);
}

CFloatColor::~CFloatColor()
{
}

ScalarType CFloatColor::GetRedValue() const
{
	//Obtain the red component.
	return(this->mRed);
}


ScalarType CFloatColor::GetGreenValue() const
{
	//Obtain the blue component.
	return(this->mGreen);
}


ScalarType CFloatColor::GetBlueValue() const
{
	//Obtain the green component.
	return(this->mBlue);
}


ScalarType CFloatColor::GetAlphaValue() const
{
	//Obtain the alpha value.
	return(this->mAlpha);
}

ScalarType CFloatColor::ClampComponent(const ScalarType componentValue) const
{
	ScalarType						clampedValue = 	componentValue;
	

	//If the color component is less than 0.0 or greater than 1.0, 
	//clamp the value to the nearest boundary (0.0 and 1.0, respectively).
	if (componentValue > kMaxColorComponentValue) {
		clampedValue = 	kMaxColorComponentValue;
	}
	else if (componentValue < kMinColorComponentValue) {
		clampedValue = 	kMinColorComponentValue;
	}

	return(clampedValue);
}

/////
// CBaseTextureData class
////

CBaseTextureData::CBaseTextureData() : mbOwnedByObject(true)
{

}

CBaseTextureData::~CBaseTextureData()
{
	if (this->mbOwnedByObject) {
		// Release the memory allocated for the texture data.
		delete(this->mpTextureData);
	}
}

CBaseTextureData::CBaseTextureData(const QuantityType textureWidth,
								   const QuantityType textureHeight,
								   const QuantityType colorDepth,
								   const QuantityType dataSize,
								   void* pTextureData) :
	mTextureWidth(textureWidth), mTextureHeight(textureHeight),
	mColorBitsPerPixel(colorDepth), mTextureDataSize(dataSize),
	mpTextureData(pTextureData), mbOwnedByObject(false)
{

}

QuantityType CBaseTextureData::GetTextureWidth() const
{
	// Return the width of the texture.
	return(this->mTextureWidth);
}


QuantityType CBaseTextureData::GetTextureHeight() const
{
	// Return the height of the texture.
	return(this->mTextureHeight);
}


QuantityType CBaseTextureData::GetTextureColorDepth() const
{
	// Return the color depth of the texture.
	return(this->mColorBitsPerPixel);
}

QuantityType CBaseTextureData::GetTextureDataSize() const
{
	// Returns the size of the texture data.
	return(this->mTextureDataSize);
}

bool CBaseTextureData::CreateTexture(const QuantityType width,
		const QuantityType height, const QuantityType bitsPerPixel)
{
	bool							bTextureCreatedSuccessfully = false;


	return(bTextureCreatedSuccessfully);
}

const void* CBaseTextureData::GetTextureData() const
{
	void*							pTextureData = NULL;

	if (this->IsValid()) {
		// Retrieve a pointer to the texture data.
		pTextureData = this->mpTextureData;
	}

	return(pTextureData);
}

bool CBaseTextureData::IsValid() const
{
	bool							bObjectIsValid = false;

	// The texture size must be non-zero in both dimensions; in addition,
	// the texture pointer must be non-null.
	if ((this->GetTextureWidth() > 0) && (this->GetTextureHeight() > 0) &&
		(this->mpTextureData != NULL))
	{
		bObjectIsValid = true;
	}

	return(bObjectIsValid);

}

CBaseTextureData CBaseTextureData::operator=(const CBaseTextureData& textureData)
{
	// Copy the information from the source object to the class.
	::memcpy(this, &textureData, sizeof(this));

	// If the texture data memory was allocated by the object, then this data
	// must be explicitly copied (along with a new memory allocation), as the
	// data will be no longer exist upon destruction of the original object.
	if (textureData.mbOwnedByObject) {
		this->mpTextureData = NULL;
		if (this->CreateTexture(textureData.GetTextureWidth(),
			textureData.GetTextureHeight(), textureData.GetTextureColorDepth()) &&
			this->IsValid())
		{
			const void*				pSourceTextureData = textureData.GetTextureData();

			// Copy the texture image data.
			if (pSourceTextureData) {
				::memcpy(this->mpTextureData, pSourceTextureData,
					textureData.GetTextureDataSize());
			}
		}
	}

	return(*this);
}

/////
// CTextureCollection class
////

CTextureCollection::CTextureCollection()
{

}

CTextureCollection::~CTextureCollection()
{

}

bool CTextureCollection::AddTextureLayer(const CBaseTextureData& textureData)
{
	bool							bTextureAddedSuccessfully = false;

	if (textureData.IsValid()) {
		// Add the texture to the list of textures.
		this->mTextureList.push_back(textureData);

		bTextureAddedSuccessfully = true;
	}

	return(bTextureAddedSuccessfully);
}

bool CTextureCollection::RemoveTextureLayer(const IndexType textureLayerIndex)
{
	bool							bTextureRemovedSuccessfully = false;

	if (textureLayerIndex < this->GetNumTextureLayers()) {
		
		this->mTextureList.erase(this->mTextureList.begin() + textureLayerIndex - 1);

		bTextureRemovedSuccessfully = true;
	}
	
	return(bTextureRemovedSuccessfully);
}


IndexType CTextureCollection::GetNumTextureLayers() const
{
	// Return the number of layers present in the collection.
	return(this->mTextureList.size());
}

const CBaseTextureData*	CTextureCollection::GetTextureLayer(const IndexType textureLayer) const
{
	const CBaseTextureData*			pBaseTextureData = NULL;

	if (textureLayer < this->GetNumTextureLayers())
	{
		// Retrieve a pointer to particular texture layer.
		pBaseTextureData = &(this->mTextureList[textureLayer]);
	}

	return(pBaseTextureData);
}


/////
// CVector class
/////
CVector::CVector() :
	CFloatThreeSpaceBase(0, 0, 0)
{

}

CVector::CVector(const FloatCoord xComponent, const FloatCoord yComponent,
				 const FloatCoord zComponent) :
	CFloatThreeSpaceBase(xComponent, yComponent, zComponent)
{

}

CVector::~CVector()
{
}

FloatCoord CVector::GetXComponent() const
{
	//Retrieve the x-component of the vector.
	return(this->GetX());
}

FloatCoord CVector::GetYComponent() const
{
	//Retrieve the y-component of the vector.
	return(this->GetY());
}

FloatCoord CVector::GetZComponent() const
{
	//Retrieve the z-component of the vector.
	return(this->GetZ());
}

ScalarType CVector::GetMagnitude() const
{
	//Determine the magnitude of the three-component vector.
	//(Magnitude = square root of (x^2 + y^2 + z^2))
	const FloatCoord				xComponent = this->GetXComponent();
	const FloatCoord				yComponent = this->GetYComponent();
	const FloatCoord				zComponent = this->GetZComponent();

	return((FloatCoord)::sqrt((xComponent * xComponent) +
		(yComponent * yComponent) + (zComponent * zComponent)));
}

void CVector::Normalize()
{
	//Store the magnitude value...
	const ScalarType				magnitude = this->GetMagnitude();

	//Normalize the represented vector.
	if (magnitude != 0.0) {
		this->SetX(this->GetXComponent() / magnitude);
		this->SetY(this->GetYComponent() / magnitude);
		this->SetZ(this->GetZComponent() / magnitude);
	}
}

void CVector::SetAsDisplacementVector(const CFloatPoint& vectorHeadPoint,
									  const CFloatPoint& vectorTailPoint)
{
	// Determine the point displacement...
	const CFloatPoint				pointDisplacement =
			(vectorHeadPoint - vectorTailPoint);

	// Define the vector object using the computed point displacement.
	this->SetX(pointDisplacement.GetXCoord());
	this->SetY(pointDisplacement.GetYCoord());
	this->SetZ(pointDisplacement.GetZCoord());
}


CVector CVector::operator*(const ScalarType& scalarValue) const
{
	// Multiply all vector components by a scalar, and return
	// the resultant vector.
	const FloatCoord				newXComponent =
		this->GetXComponent() * scalarValue;
	const FloatCoord				newYComponent =
		this->GetYComponent() * scalarValue;
	const FloatCoord				newZComponent =
		this->GetZComponent() * scalarValue;

	return(CVector(newXComponent, newYComponent, newZComponent));
}

CVector CVector::operator/(const ScalarType& scalarValue) const
{
	// Multiply the vector by a reciprocal value, effectively
	// dividing the vector by a value.
	const CVector					newVector = 
		(scalarValue != 0.0) ? ((*this) * (1 / scalarValue)) :
		*this;

	return(newVector);
}


CVector CVector::operator+(const CVector& secondVector) const
{
	// Sum the consituent vector components in order to create the
	// total vector sum.
	const FloatCoord				newXComponent =
		this->GetXComponent() + secondVector.GetXComponent();
	const FloatCoord				newYComponent =
		this->GetYComponent() + secondVector.GetYComponent();
	const FloatCoord				newZComponent = 
		this->GetZComponent() + secondVector.GetZComponent();

	return(CVector(newXComponent, newYComponent, newZComponent));
}


CVector CVector::operator-(const CVector& secondVector) const
{

	// Negate the vector components of one vector, and sum the
	// vectors in order to produce a vector difference.
	return(*this + CVector(-secondVector.GetXComponent(),
		-secondVector.GetYComponent(), -secondVector.GetZComponent()));
}


/////
// CPolygonVertex class
/////
CPolygonVertex::CPolygonVertex() :
	CFloatPoint(0.0, 0.0, 0.0), mVertexColor(kMaxColorComponentValue, kMaxColorComponentValue,
		kMaxColorComponentValue)
{
}

CPolygonVertex::CPolygonVertex(const FloatCoord xCoord,
		const FloatCoord yCoord,
		const FloatCoord zCoord) :
	CFloatPoint(xCoord, yCoord, zCoord), mVertexColor(kMaxColorComponentValue,
		kMaxColorComponentValue, kMaxColorComponentValue)
{
}

CPolygonVertex::CPolygonVertex(const FloatCoord xCoord,
		const FloatCoord yCoord,
		const FloatCoord zCoord,
		const CVector& vertexNormal,
		const CFloatColor& floatColor,
		const ScalarType uTextureCoord, const ScalarType vTextureCoord) :
	CFloatPoint(xCoord, yCoord, zCoord), mVertexColor(floatColor),
	muTextureCoord(uTextureCoord), mvTextureCoord(vTextureCoord)

{
	CPolygonVertex::CPolygonVertex(xCoord, yCoord, zCoord);
	//Assign the vertex normal.
	this->SetNormal(vertexNormal);
}

CPolygonVertex::CPolygonVertex(const CFloatPoint& floatCoords) :
		CFloatPoint(floatCoords.GetXCoord(),
			floatCoords.GetYCoord(),
			floatCoords.GetZCoord()), 
			mVertexColor(kMaxColorComponentValue,
		kMaxColorComponentValue, kMaxColorComponentValue)
{
	CPolygonVertex::CPolygonVertex(floatCoords.GetXCoord(),
		floatCoords.GetYCoord(), floatCoords.GetZCoord());
}


CPolygonVertex::CPolygonVertex(const CFloatPoint& floatCoords,
		const CVector& vertexNormal, const CFloatColor& floatColor,
		const ScalarType uTextureCoord, const ScalarType vTextureCoord) :
		CFloatPoint(floatCoords.GetXCoord(),
			floatCoords.GetYCoord(),
			floatCoords.GetZCoord()),
			mVertexColor(kMaxColorComponentValue,
		kMaxColorComponentValue, kMaxColorComponentValue),
		mVertexNormal(vertexNormal),
		muTextureCoord(uTextureCoord), mvTextureCoord(vTextureCoord)
{
	CPolygonVertex::CPolygonVertex(floatCoords.GetXCoord(),
		floatCoords.GetYCoord(), floatCoords.GetZCoord(),
		vertexNormal, floatColor);

}


CPolygonVertex::~CPolygonVertex()
{
}

CVector CPolygonVertex::GetNormal() const
{
	return(this->mVertexNormal);
}

void CPolygonVertex::SetNormal(const CVector& vertexNormal)
{
	//Set the normal to the specified vector, and normalize
	//the vector.
	this->mVertexNormal = vertexNormal;
	this->mVertexNormal.Normalize();
}

CFloatColor CPolygonVertex::GetColor() const
{
	//Return the vertex color.
	return(this->mVertexColor);
}

void CPolygonVertex::GetTextureCoordinates(ScalarType& uCoord,
		ScalarType& vCoord) const
{
	// Retrieve the texture coordinates.
	uCoord = this->muTextureCoord;
	vCoord = this->mvTextureCoord;
}


void CPolygonVertex::SetTextureCoordinates(const ScalarType uCoord,
										  const ScalarType vCoord)
{
	// Store the texture coordinates, clamping the coordinates
	// between 0.0 and 1.0;
	this->muTextureCoord = this->ClampTextureCoord(uCoord);
	this->mvTextureCoord = this->ClampTextureCoord(vCoord);
}


ScalarType CPolygonVertex::ClampTextureCoord(const ScalarType textureCoordinate)
{
	ScalarType						finalTextureCoordinate = textureCoordinate;

	// Clamp the texture coordinate values between 0.0 and 1.0 (inclusive).
	if (textureCoordinate > kMaxTextureCoordinateValue) {
		finalTextureCoordinate = kMaxTextureCoordinateValue;
	}
	else if (textureCoordinate < kMinTextureCoordinateValue) {
		finalTextureCoordinate = kMinTextureCoordinateValue;
	}

	return(textureCoordinate);
}

/////
// CFloatPolygon class
/////
CFloatPolygon::CFloatPolygon() : 
	mColorShadeAmbient(0.0, 0.0, 0.0), mColorShadeDiffuse(0.0, 0.0, 0.0),
	mColorShadeSpecular(0.0, 0.0, 0.0), mColorShadeEmissive(0.0, 0.0, 0.0),
	mSpecularity(0.0)
{
}

CFloatPolygon::~CFloatPolygon()
{
}

CFloatPolygon::CFloatPolygon(const VertexList& vertexList)  : 
	mColorShadeAmbient(0.0, 0.0, 0.0), mColorShadeDiffuse(0.0, 0.0, 0.0),
	mColorShadeSpecular(0.0, 0.0, 0.0), mColorShadeEmissive(0.0, 0.0, 0.0),
	mSpecularity(0.0)
{
	//Copy the vertex list into the local vertex list.
	mVertexList = vertexList;
}

IndexType CFloatPolygon::GetVertexCount() const
{
	//Return the number of elements contained in the vertex list.
	return(this->mVertexList.size());
}


void CFloatPolygon::AddVertex(const CPolygonVertex& polygonVertex)
{
	//Add the vertex to the internal vertex list.
	this->mVertexList.push_back(polygonVertex);
}

bool CFloatPolygon::DeleteVertex(const IndexType vertexIndex)
{
	bool							bVertexDeletedSuccessfully = false;

	//If the vertex index is in the valid range of vertex indices,
	//remove the vertex from the vertex list.
	if (::AssertExpression(vertexIndex < this->GetVertexCount())) {
		(this->mVertexList).erase((this->mVertexList).begin() + 
			vertexIndex);

		bVertexDeletedSuccessfully = true;

	}

	return(bVertexDeletedSuccessfully);
}


CFloatColor	CFloatPolygon::GetLightShading(const EBasicLightShading lightShadingType)
{
	CFloatColor						shadingColor(0.0, 0.0, 0.0);

	switch(lightShadingType) {
	case eShadeAmbient:				//Ambient light contribution color
		shadingColor = this->mColorShadeAmbient;
		break;
	case eShadeDiffuse:				//Diffuse light contribution color
		shadingColor = this->mColorShadeDiffuse;
		break;
	case eShadeSpecular:			//Specular light contribution color
		shadingColor = this->mColorShadeSpecular;
		break;
	case eShadeEmissive:			//Emissive light contribution color
		shadingColor = this->mColorShadeEmissive;
		break;
	default:
		//Code should never arrive here.
		::AssertExpression(false);
		break;
	}

	return(shadingColor);
}

void CFloatPolygon::SetLightShading(const EBasicLightShading lightShadingType,
									const CFloatColor& lightShadingColor)
{
	switch(lightShadingType) {
	case eShadeAmbient:				//Ambient light contribution color
		this->mColorShadeAmbient = lightShadingColor;
		break;
	case eShadeDiffuse:				//Diffuse light contribution color
		this->mColorShadeDiffuse = lightShadingColor;
		break;
	case eShadeSpecular:			//Specular light contribution color
		this->mColorShadeSpecular = lightShadingColor;
		break;
	case eShadeEmissive:			//Emissive light contribution color
		this->mColorShadeEmissive = lightShadingColor;
		break;
	default:
		//Code should never arrive here.
		::AssertExpression(false);
		break;
	}
}

ScalarType CFloatPolygon::GetSpecularity() const
{
	return(this->mSpecularity);
}

void CFloatPolygon::SetSpecularity(const ScalarType specularity)
{
	const ScalarType				kMaxSpecularValue = kMaxSpecularityValue;
	const ScalarType				kMinSpecularValue = kMinSpecularityValue;

	ScalarType						finalSpecularity =
		specularity;

	//Clamp the specularity value between 0.0 and 1.0 (inclusive)
	//as necessary.
	if(specularity > kMaxSpecularValue) {
		finalSpecularity = kMaxSpecularValue;
	}
	else if(specularity < kMinSpecularValue) {
		finalSpecularity = kMinSpecularValue;
	}

	//Store the specularity value.
	this->mSpecularity = specularity;
}

/////
// CVertexGrid class
/////
CVertexGrid::CVertexGrid(const IndexType width, const IndexType height) :
	mVertexGrid(width, height), mGridWidth(width), mGridHeight(height)

{

}

CVertexGrid::~CVertexGrid()
{
}


IndexType CVertexGrid::GetGridWidth() const
{
	IndexType						gridWidth = 0;

	//If the grid object is valid, obtain the stored width.
	if(this->IsGridValid()) {
		gridWidth = this->mGridWidth;
	}

	return(gridWidth);
}

IndexType CVertexGrid::GetGridHeight() const
{
	IndexType						gridHeight = 0;

	//If the grid object is valid, obtain the stored height.
	if(this->IsGridValid()) {
		gridHeight = this->mGridHeight;
	}

	return(gridHeight);
}


//Adds vertex to the grid at the specified index.
bool CVertexGrid::AddVertexToGrid(const IndexType xIndex,
		const IndexType yIndex, const CPolygonVertex& polygonVertex)
{
	bool							bVertexSuccessfullyAdded = false;

	CPolygonVertex					vertexToAdd = polygonVertex;

	bVertexSuccessfullyAdded = this->mVertexGrid.SetElementAt(xIndex,
		yIndex, vertexToAdd);

	return(bVertexSuccessfullyAdded);
}

//Generates a list of polygons from a the internal grid.
//The tesselation factor begins with a value of 1, with
//higher values providing increased tesselation degrees.
bool CVertexGrid::GeneratePolygonsFromGrid(PolygonList& polygonList,
		const IndexType tesselationFactor) const
{
	bool							bGridGeneratedSuccessfully = false;

	polygonList.clear();
	//!!!Tessalation ignored for now... [AAO, 2-19-06]

	//Get the grid width and height. In order to create polygons
	//from the grid, it is required that both the number of rows
	//and columns are greater than one.
	const IndexType					gridWidth =
		this->GetGridWidth();
	const IndexType					gridHeight =
		this->GetGridHeight();

	if(this->IsGridValid() && (gridWidth > 1) &&
		(gridHeight > 1))
	{
		for(IndexType heightLoop = 0; heightLoop < (gridHeight - 1); heightLoop++) {
			for(IndexType widthLoop = 0; widthLoop < (gridWidth - 1); widthLoop++) {
				//Obtain a group of vertices that constitute a rectangular region
				//(quadratic patch in three-dimensional space)...
				const CPolygonVertex
					upperLeft = *(this->mVertexGrid.GetElementAt(widthLoop, heightLoop));
				const CPolygonVertex
					upperRight = *(this->mVertexGrid.GetElementAt(widthLoop + 1, heightLoop));
				const CPolygonVertex
					lowerLeft = *(this->mVertexGrid.GetElementAt(widthLoop, heightLoop + 1));
				const CPolygonVertex
					lowerRight = *(this->mVertexGrid.GetElementAt(widthLoop + 1, heightLoop + 1));

				//Create two triangles from the rectangle.
				CFloatPolygon		firstPolygon;
				CFloatPolygon		secondPolygon;

				firstPolygon.AddVertex(upperLeft);
				firstPolygon.AddVertex(lowerLeft);
				firstPolygon.AddVertex(lowerRight);

				//!!!?
				secondPolygon.AddVertex(upperLeft);
				secondPolygon.AddVertex(lowerRight);
				secondPolygon.AddVertex(upperRight);

				polygonList.push_back(firstPolygon);
				polygonList.push_back(secondPolygon);
			}
		}
	}

	return(bGridGeneratedSuccessfully);
}

//Verifies the validity of the vertex grid object.
bool CVertexGrid::IsGridValid() const
{
	return((this->mVertexGrid).IsValid());
}


CVector VectorCrossProduct(const CVector& vectorA, const CVector& vectorB)
{
	//http://hemsidor.torget.se/users/m/mauritz/math/vect/xprod.htm
	//ab=((a2b3 - a3b2), (a1b3 - a3b1), (a1b2 - a2b1))

	const FloatCoord				crossX =
		(vectorA.GetYComponent() * vectorB.GetZComponent()) -
		(vectorA.GetZComponent() * vectorB.GetYComponent());
	const FloatCoord				crossY =
		(vectorA.GetXComponent() * vectorB.GetZComponent()) -
		(vectorA.GetZComponent() * vectorB.GetXComponent());
	const FloatCoord				crossZ =
		(vectorA.GetXComponent() * vectorB.GetYComponent()) -
		(vectorA.GetYComponent() * vectorB.GetXComponent());


	//Return the new vector (cross product of the two specified vectors -
	//VectorA x VectorB.
	return(CVector(crossX, crossY, crossZ));
}