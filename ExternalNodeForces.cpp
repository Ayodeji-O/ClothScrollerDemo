// ExternalNodeForces.cpp - Contains declarations for external node force generators.


#include "ExternalNodeForces.h"


/////
// CGravityNodeForce class
/////

CGravityNodeForce::CGravityNodeForce(const ScalarType gravitationalConstant,
									 const CVector gravityVector) :
	mGravitationalConstant(gravitationalConstant),
	mGravityVector((gravityVector.GetMagnitude() > 0.0) ?
		(gravityVector / gravityVector.GetMagnitude()) : gravityVector)
{

}


CGravityNodeForce::~CGravityNodeForce()
{

}

bool CGravityNodeForce::GenerateExternalForceVector(const CPhysicalNodeBase& targetNode,
											   CVector& forceVector) const
{
	// Gravitational force (N) = Mass (kg) x Gravitational acceleration constant (m / s^2)
	const ScalarType				forceMagnitude = targetNode.GetNodeMass() *
		this->mGravitationalConstant;

	// Provide a direction for the gravitational force.
	forceVector = mGravityVector * forceMagnitude;

	return(true);
}


bool CGravityNodeForce::IsValid() const
{
	// The class is always "valid" - any gravitational constant is accepted (including
	// negative).
	return(true);
}


CWindNodeForce::CWindNodeForce(const CVector directionVector, const ScalarType magnitude) :
	mDirectionVector(directionVector),  mMaxMagnitude(magnitude)
{

}

CWindNodeForce::~CWindNodeForce()
{

}


bool CWindNodeForce::GenerateExternalForceVector(const CPhysicalNodeBase& targetNode,
												 CVector& forceVector) const
{
	bool							bForceGeneratedSuccessfully = false;

	if (this->IsValid())
	{
		forceVector = this->mDirectionVector;


		CFloatPoint					nodeLocation = targetNode.GetNodeLocation();

		const double				noise = this->NoiseFunction(nodeLocation.GetXCoord(),
			nodeLocation.GetYCoord(), nodeLocation.GetZCoord());
		CVector						perturbation(noise, noise, noise);
		forceVector = forceVector + perturbation;
		forceVector.Normalize();

		forceVector = forceVector * ((::cos(noise * this->mAlterationConstant) + 1.0) / 2.0); 
		forceVector = forceVector * this->mMaxMagnitude;

		bForceGeneratedSuccessfully = true;
	}


	return(bForceGeneratedSuccessfully);
}

bool CWindNodeForce::IsValid() const
{
	// Magnitude must be greater than or equal to zero (otherwise, final wind direction
	// is the opposite of the intended direction).
	return(this->mMaxMagnitude >= 0.0);
}


void CWindNodeForce::SetAlterationConstant(const QuantityType constant)
{
	this->mAlterationConstant = constant;
}

double CWindNodeForce::NoiseFunction(const double noiseInputX,
									 const double noiseInputY,
									 const double noiseInputZ) const
{
	const							kDecimalMultiplier = 1000.0;
	double							noiseResult = 0.0;

	int								integerInput = (int)(kDecimalMultiplier * noiseInputX);
	integerInput += (int)((kDecimalMultiplier * noiseInputY)) << 1;
	integerInput += (int)((kDecimalMultiplier * noiseInputZ)) << 2;


	const int						kShiftConstant = 13;
	const int						kConstant1 = 15731;
	const int						kConstant2 = 789221;
	const int						kConstant3 = 1376312589;
	const double					kConstant4 = 1073741824.0;


	// http://freespace.virgin.net/hugo.elias/models/m_perlin.htm

    integerInput = (integerInput << kShiftConstant) ^ integerInput;
    noiseResult = ( 1.0 - ( (integerInput * (integerInput * integerInput * kConstant1 + kConstant2) +
		kConstant3) & 0x7fffffff) / kConstant4);

	return(noiseResult);
}

CViscousNodeForce::CViscousNodeForce(const ScalarType viscousCoefficient) :
	mViscousCoefficient(viscousCoefficient)
{

}

CViscousNodeForce::~CViscousNodeForce()
{

}

bool CViscousNodeForce::GenerateExternalForceVector(const CPhysicalNodeBase& targetNode,
													CVector& forceVector) const
{
	bool							bForceGeneratedSuccessfully = false;

	if (this->IsValid()) {
		// Visous Damping Force (N) =  Velocity (m / s) * Damping Coefficient (kg / s)
		forceVector = targetNode.GetNodeVelocityVector() * (-this->mViscousCoefficient);
		bForceGeneratedSuccessfully = true;
	}

	return(bForceGeneratedSuccessfully);
}


bool CViscousNodeForce::IsValid() const
{
	// The viscous coefficient must be greater (or equivalent to) zero; otherwise,
	// acceleration occurs on the node instead of damping.
	return(this->mViscousCoefficient >= 0.0);
}
