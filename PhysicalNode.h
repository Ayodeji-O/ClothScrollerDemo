// PhysicalNode.h - Contains declarations pertaining to nodes/node physics.


#ifndef _PHYSICALNODE_H
#define _PHYSICALNODE_H	1

#include "PrimaryHeaders.h"
#include "PrimitiveSupport.h"


class CPhysicalNodeBase
{
public:
	CPhysicalNodeBase();
	CPhysicalNodeBase(const CFloatPoint& initialLocation, 
		const ScalarType nodeMass, const bool bIsAnchorNode = false);
	~CPhysicalNodeBase();

	// Determines if the node if valid in accordance
	// with its defining parameters.
	bool							IsValid() const;

	// Retrieves the current node location.
	virtual CFloatPoint				GetNodeLocation() const;

	// Retrievs the mass of the node.
	virtual ScalarType				GetNodeMass() const;

	// Retrieves the current node velocity vector.
	virtual CVector					GetNodeVelocityVector() const;


	// Sets a constant force, specified by a magnitude and direction,
	// to be applied to the current node.
	void							SetForceVector(const CVector& forceVector);

	// Applies the pending force to the current node over a specified period of time.
	bool							ApplyForceVector(const ScalarType timeQuantum);

	// Sets the anchor node status (node will always report a velocity of zero
	// while this flag is set to TRUE).
	virtual void					SetAnchorNodeStatus(const bool bIsAnchorNode);

protected:

	// Determines if the current node is an anchor node.
	virtual bool					IsAnchorNode() const;

	// Sets the current node position.
	void							SetNodeLocation(const CFloatPoint& nodeLocation);

	// Sets the current velocity vector.
	void							SetNodeVelocityVector(const CVector& newVelocityVector);

	// Indicates whether or not the node is mobile.
	bool							mbIsAnchorNode;


	// Physical node attributes.

	// Node velocity vector
	CVector							mVelocityVector;
	
	// Node mass (kg)
	const ScalarType				mNodeMass;

	// Node location in three-dimensional space.
	CFloatPoint						mNodeLocation;

	// Externally-applied force (will have no direction/magnitude
	// after force has been applied).
	CVector							mPendingForceVector;
};


class CExternalNodeForce
{
public:
	CExternalNodeForce();
	virtual ~CExternalNodeForce();

	// Generates a force vector for the instantaneous force to be applied to
	// a particular node (the client must then use this force vector to apply
	// a force on the node).
	virtual bool					GenerateExternalForceVector(
		const CPhysicalNodeBase& targetNode, CVector& forceVector) const = 0;

	// Determines if the external node force generator class is valid
	// in accordance with its defining parameters.
	virtual bool					IsValid() const;


protected:


};

class CExternalNodeForceCollection
{
public:
	CExternalNodeForceCollection();
	~CExternalNodeForceCollection();

	// Adds an external force evaluator to the collection of forces
	// to be evaluated.
	bool AddExternalNodeForce(CExternalNodeForce& externalNodeForce);

	// Evaluates all contained external forces, using the physical
	// parameters specified within the provided node.
	bool EvaluateExternalForces(const CPhysicalNodeBase& targetNode,
		CVector& forceVector, const ScalarType timeQuantum) const;

protected:


	// Type definition - list of external forces that will be
	// applied to a node.
	typedef std::vector<CExternalNodeForce*>
									ExternalForceListType;


	// Collection of external forces.
	ExternalForceListType			mExternalForceList;
};


#endif	//#ifndef _PHYSICALNODE_H