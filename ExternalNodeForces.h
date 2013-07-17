// ExternalNodeForces.h - Contains declarations for external node force generators.


#ifndef _EXTERNALNODEFORCES_H
#define	_EXTERNALNODEFORCES_H	1


#include "PhysicalNode.h"

class CGravityNodeForce : public CExternalNodeForce
{
public:
	CGravityNodeForce(const ScalarType gravitationalConstant, const
		CVector gravityVector);
	~CGravityNodeForce();

	// Generates a force vector for the instantaneous gravitational
	// force to be applied to a particular node.
	bool							GenerateExternalForceVector(
		const CPhysicalNodeBase& targetNode, CVector& forceVector) const;

	// Determines if the external node gravitational force generator class
	// is valid in accordance with its defining parameters.
	bool							IsValid() const;

protected:
	// Vector that indicates the direction of gravity.
	const CVector					mGravityVector;

	// Gravitational acceleration constant.
	const ScalarType				mGravitationalConstant;
};




class CWindNodeForce : public CExternalNodeForce
{
public:
	CWindNodeForce(const CVector directionVector, const ScalarType maxMagnitude);
	~CWindNodeForce();

	// Generates a wind force vector for the instantaneous force to be applied to
	// a particular node.
	bool							GenerateExternalForceVector(
		const CPhysicalNodeBase& targetNode, CVector& forceVector) const;

	// Determines if the external node wind force generator class is valid
	// in accordance with its defining parameters.
	bool							IsValid() const;


	void							SetAlterationConstant(const QuantityType constant);

protected:

	// Noise function generates a number between -1.0 and 1.0, based upon the provided
	// input.
	double							NoiseFunction(const double noiseInputX,
		const double noiseInputY, const double noiseInputZ) const;

	QuantityType					mAlterationConstant;

	const CVector					mDirectionVector;
	const ScalarType				mMaxMagnitude;
};


class CViscousNodeForce : public CExternalNodeForce
{
public:
	CViscousNodeForce(const ScalarType viscousCoefficient);
	~CViscousNodeForce();


	// Generates a force vector for the instantaneous viscous/drag force to be
	// applied to a particular node.
	bool							GenerateExternalForceVector(
		const CPhysicalNodeBase& targetNode, CVector& forceVector) const;

	// Determines if the external node viscous force generator class is valid
	// in accordance with its defining parameters.
	bool							IsValid() const;


protected:

	// "Damping" coefficient
	const ScalarType				mViscousCoefficient;

};


#endif	// #ifndef _EXTERNALNODEFORCES_H