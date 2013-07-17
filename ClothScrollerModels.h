// ClothScrollerModels.h - (Portions of code adapted from "Balloons" project, BalloonsModels.h
//                         file) Contains declarations specific to balloon model storage and
//                         generation.

#ifndef _CLOTHSCROLLERMODELS_H
#define _CLOTHSCROLLERMODELS_H	1

#include "ModelGeometry.h"



class CGeneratedCylinder : public CGeneratedModel
{
public:
	CGeneratedCylinder(const QuantityType xGridPoints, const QuantityType yGridPoints,
		const CFloatColor& cylinderColor, const ScalarType cylinderRadius = 1.0,
		const ScalarType cylinderHeight = 1.0);
	~CGeneratedCylinder();

protected:

	bool							GenerateModel();

	//Retrieves the width of the intermediate vertex grid.
	IndexType						GetVertexGridWidth() const;

	//Retrieves the height of the intermediate vertex grid.
	IndexType						GetVertexGridHeight() const;
	
	//Obtains the height of the cylinder.
	ScalarType						GetCylinderHeight() const;

	//Obtains the radius of the cylinder.
	ScalarType						GetCylinderRadius() const;
		
	bool							ApplyCylinderSurfaceProperties(PolygonList& polygonList) const;

	const ScalarType				mCylinderHeight;

	const ScalarType				mCylinderRadius;

	CVertexGrid						mVertexGrid;


	CFloatColor						mCylinderColor;
};





#endif	// #ifndef _CLOTHSCROLLERMODELS_H