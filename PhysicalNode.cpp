// PhysicalNode.cpp - Contains implementations pertaining to nodes/node physics.

#include "PhysicalNode.h"

/////
// CPhysicalNodeBase class
/////
CPhysicalNodeBase::CPhysicalNodeBase() :
	mNodeLocation(0.0, 0.0, 0.0), mNodeMass(0.0), mVelocityVector(0.0, 0.0, 0.0),
	mbIsAnchorNode(false)
{
	// Copy constructor
}


CPhysicalNodeBase::CPhysicalNodeBase(const CFloatPoint& initialLocation, 
									 const ScalarType nodeMass, const bool bIsAnchorNode) : 
	mNodeLocation(initialLocation), mNodeMass(nodeMass), mVelocityVector(0.0, 0.0, 0.0),
	mbIsAnchorNode(bIsAnchorNode)
{
	
}


CPhysicalNodeBase::~CPhysicalNodeBase()
{

}

bool CPhysicalNodeBase::IsValid() const
{
	// The mass of a node must be greater than zero in order for the node to be
	// valid.
	return(this->mNodeMass > 0);
}


CFloatPoint CPhysicalNodeBase::GetNodeLocation() const
{
	// Return the current node location in three-dimensional
	// space.
	return(this->mNodeLocation);
}


ScalarType CPhysicalNodeBase::GetNodeMass() const
{
	// Return the mass of the node.
	return(this->mNodeMass);
}

CVector	CPhysicalNodeBase::GetNodeVelocityVector() const
{
	// Return the instantaneous node velocity.
	return(this->mVelocityVector);
}


void CPhysicalNodeBase::SetForceVector(const CVector& forceVector)
{
	// Set the current force that is being applied to the node.
	this->mPendingForceVector = forceVector;
}

bool CPhysicalNodeBase::ApplyForceVector(const ScalarType timeQuantum)
{
	bool							bAppliedSuccessfully = false;

	if ((timeQuantum > 0.0) && (this->mPendingForceVector.GetMagnitude() > 0.0)) {


		// Acceleration = Force (N) / Mass (kg)
		const CVector				nodeAccelerationVector = this->mPendingForceVector /
			this->GetNodeMass();

		// (Delta - from external force) Velocity (m / s) = Acceleration (m / s^2) x Time (s)
		const CVector				deltaVelocityVector = nodeAccelerationVector *
			timeQuantum;


		// (Delta - from external force) Distance = 1 / 2 x Acceleration (m / s^2) x Time^2 (s^2)
		const CVector				deltaDisplacementVector = nodeAccelerationVector * timeQuantum *
			timeQuantum;


		const CVector				totalDisplacementVector = 
			(this->GetNodeVelocityVector() * timeQuantum) + deltaDisplacementVector;


		// Apply the displacement, and set the new node location.
		const CFloatPoint			nodeDisplacement(totalDisplacementVector.GetXComponent(),
			totalDisplacementVector.GetYComponent(), totalDisplacementVector.GetZComponent());

		this->SetNodeLocation(this->GetNodeLocation() + nodeDisplacement);


		// Compute the final velocity by summing the velocity change vector and the current
		// velocity vector.
		this->SetNodeVelocityVector(this->GetNodeVelocityVector() + deltaVelocityVector);


		// Clear the force vector after the force has been applied to the node (no forces are
		// currently pending).
		this->mPendingForceVector = CVector(0.0, 0.0, 0.0);
	}


	return (bAppliedSuccessfully);
}

void CPhysicalNodeBase::SetAnchorNodeStatus(const bool bIsAnchorNode)
{
	// Set the anchor node flag (indicates whether or not the node
	// is movable by external forces).
	this->mbIsAnchorNode = bIsAnchorNode;
}


bool CPhysicalNodeBase::IsAnchorNode() const
{
	// Return the current anchor node status.
	return(this->mbIsAnchorNode);
}

void  CPhysicalNodeBase::SetNodeLocation(const CFloatPoint& nodeLocation)
{
	if (this->IsValid() && !this->IsAnchorNode()) {
		this->mNodeLocation = nodeLocation;
	}

}

void CPhysicalNodeBase::SetNodeVelocityVector(const CVector& newVelocityVector)
{
	if (this->IsValid() && !this->IsAnchorNode()) {
		// Set the velocity vector if the node is not an anchor node.
		this->mVelocityVector = newVelocityVector;
	}
}

/////
// CExternalNodeForce class
/////
CExternalNodeForce::CExternalNodeForce()
{

}

CExternalNodeForce::~CExternalNodeForce()
{

}

bool CExternalNodeForce::IsValid() const
{
	// Derived classes should override this method, and perform the
	// appropriate parameter verifications as necessary.
	return(false);
}


/////
// CExternalNodeForceCollection class
/////
CExternalNodeForceCollection::CExternalNodeForceCollection()
{

}

CExternalNodeForceCollection::~CExternalNodeForceCollection()
{
	this->mExternalForceList.clear();
}


bool CExternalNodeForceCollection::AddExternalNodeForce(CExternalNodeForce& externalNodeForce)
{
	bool							bAddedSuccessfully = false;

	// Add the external force generator to the list of generators.
	if (externalNodeForce.IsValid()) {
		this->mExternalForceList.push_back(&externalNodeForce);
		bAddedSuccessfully = true;
	}

	return(bAddedSuccessfully);
}


bool CExternalNodeForceCollection::EvaluateExternalForces(const CPhysicalNodeBase& targetNode,
														  CVector& forceVector,
														  const ScalarType timeQuantum) const
{
	bool							bEvaluatedSuccessfully = false;

	// Clear the force vector...
	forceVector = CVector(0.0, 0.0, 0.0);

	ExternalForceListType::const_iterator
									forceListIterator;

	// Iterate through the list of forces...
	for (forceListIterator = this->mExternalForceList.begin(); forceListIterator !=
		this->mExternalForceList.end(); forceListIterator++)
	{
		if (*forceListIterator) {
			CVector					currentForceVector(0.0, 0.0, 0.0);

			// Evaluate the current force, generating a force vector, and produce a vector
			// sum for the forces.
			if((*forceListIterator)->GenerateExternalForceVector(targetNode, currentForceVector)) {
				forceVector = forceVector + currentForceVector;

				bEvaluatedSuccessfully = true;
			}
		}
	}

	return(bEvaluatedSuccessfully);
}

