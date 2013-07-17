// ClothScrollerModels.h - (Portions of code adapted from "Balloons" project, BalloonsModels.h
//                         file) Contains definitions specific to balloon model storage and
//                         generation.


#include "ClothScrollerModels.h"


CGeneratedCylinder::CGeneratedCylinder(const QuantityType xGridPoints,
									   const QuantityType yGridPoints,
									   const CFloatColor& cylinderColor,
									   const ScalarType cylinderRadius,
									   const ScalarType cylinderHeight) :
	mVertexGrid(xGridPoints, yGridPoints), mCylinderHeight(cylinderHeight),
	mCylinderRadius(cylinderRadius), mCylinderColor(cylinderColor)
{
	//Create the cylinder model.
	this->GenerateModel();
}

CGeneratedCylinder::~CGeneratedCylinder()
{
}

bool CGeneratedCylinder::GenerateModel()
{
	bool							bGeneratedSuccessfully = false;

	//Determine the dimensions of the intermediate vertex grid...
	const IndexType					gridWidth = this->GetVertexGridWidth();
	const IndexType					gridHeight = this->GetVertexGridHeight();

	const ScalarType				cylinderRadius = this->GetCylinderRadius();
	const ScalarType				cylinderHeight = this->GetCylinderHeight();

	if((gridWidth > 1) && (gridHeight > 1)) {
		for(IndexType zLoop = 0; zLoop < gridHeight; zLoop++) {
			//Rotate the profile point
			for(IndexType xLoop = 0; xLoop < gridWidth; xLoop++) {
				FloatCoord			currentAngle = kTwoPi * xLoop / (gridWidth - 1);

				const FloatCoord	currentXCoord = cylinderRadius * 
					::cos(currentAngle);
				const FloatCoord	currentYCoord = cylinderRadius *
					::sin(currentAngle);
				const FloatCoord	currentZCoord = 
					((FloatCoord)zLoop / (FloatCoord)(gridHeight - 1) * cylinderHeight) -
					(cylinderHeight / 2.0);


				//...Use the derivative to determine the local surface
				//normal (at the vertex).
				const CVector		vertexNormal(1.0 * ::cos(currentAngle), 1.0 * ::sin(currentAngle),
					0.0);

				//Add the vertex to the vertex grid.
				this->mVertexGrid.AddVertexToGrid(xLoop, zLoop, CPolygonVertex(currentXCoord,
					currentYCoord, currentZCoord, vertexNormal));
			}
		}

		//Generate the balloon polygons.
		PolygonList					generatedPolygons;
		this->mVertexGrid.GeneratePolygonsFromGrid(generatedPolygons);

		//Set the surface properties appropriately (primarily specularity).
		this->ApplyCylinderSurfaceProperties(generatedPolygons);

		//Set the created mesh to be the final balloon model.
		this->mGeneratedMesh.SetModelGeometry(generatedPolygons);
	}

	return(bGeneratedSuccessfully);
}


//Retrieves the width of the intermediate vertex grid.
IndexType CGeneratedCylinder::GetVertexGridWidth() const
{
	//Obtains the width of the vertex grid.
	return(this->mVertexGrid.GetGridWidth());
}

//Retrieves the height of the intermediate vertex grid.
IndexType CGeneratedCylinder::GetVertexGridHeight() const
{
	//Obtains the height of the vertex grid.
	return(this->mVertexGrid.GetGridHeight());
}
	
//Obtains the height of the cylinder.
ScalarType CGeneratedCylinder::GetCylinderHeight() const
{
	//Obtains the cylinder height.
	return(this->mCylinderHeight);
}

//Obtains the width of the cylinder.
ScalarType CGeneratedCylinder::GetCylinderRadius() const
{
	//Obtains the cylinder radius.
	return(this->mCylinderRadius);
}

bool CGeneratedCylinder::ApplyCylinderSurfaceProperties(PolygonList& polygonList) const
{
	bool							bPropertiesAppliedSuccessfully = false;

	//Cylinder specularity.
	const FloatCoord				kSpecularity = (FloatCoord)0.010;
	//Specular highlight color.
	const CFloatColor				kSpecularColor(1.0, 1.0, 1.0);

	//Emissive color.
	const ScalarType				kEmissiveScaling = 0.3;
	const CFloatColor				kEmissiveColor(
		this->mCylinderColor.GetRedValue() * kEmissiveScaling,
		this->mCylinderColor.GetGreenValue() * kEmissiveScaling,
		this->mCylinderColor.GetBlueValue() * kEmissiveScaling);

	for(PolygonList::iterator polygonListIterator = polygonList.begin();
		polygonListIterator != polygonList.end(); polygonListIterator++)
	{
		//Set the diffuse and ambient colors to be equivalent to the cylinder color.
		polygonListIterator->SetLightShading(eShadeAmbient,
			this->mCylinderColor);
		polygonListIterator->SetLightShading(eShadeDiffuse,
			this->mCylinderColor);

		polygonListIterator->SetLightShading(eShadeEmissive,
			kEmissiveColor);

		//Set the specular color.
		polygonListIterator->SetLightShading(eShadeSpecular,
			kSpecularColor);

		//Set the specular amount.
		polygonListIterator->SetSpecularity(kSpecularity);
	}

	bPropertiesAppliedSuccessfully = !polygonList.empty();

	return(bPropertiesAppliedSuccessfully);
}