// ClothNodeNetwork.h - Defines classes pertinent to the network of nodes required for simulating
//                      cloth movement

#ifndef _CLOTHNODENETWORK_H_
#define _CLOTHNODENETWORK_H_		1


#include "PrimitiveSupport.h"
#include "ModelGeometry.h"
#include "PhysicalNode.h"
#include <list>
#include <algorithm>			// Required for std::find(...)


// Cloth simulation implementation concept is based on the following
// references:
//
// 1.) Provot, Xavier.
//     Deformation Constraints in a Mass-Spring Model to Describe Rigid Cloth Behavior
//     Graphics Interface, 1995, pp. 147-155.
//     (http://www-rocq.inria.fr/syntim/research/provot)
//
// 2.) Jeff Lander
//     Devil in the Blue-Faceted Dress: Real-Time Cloth Animation
//     (http://www.gamasutra.com/features/20000327/lander_pfv.htm)
//
// 3.) R. Bridson, S. Marino and R. Fedkiw. 
//     Deformation Constraints in a Mass-Spring Model to Describe Rigid Cloth Behavior
//     Eurographics/SIGGRAPH Symposium on Computer Animation


// Forward reference for CSpringNodeLink...
class CClothNode;


class CSpringNodeLink
{
public:

	CSpringNodeLink(CClothNode& firstClothNode,
		CClothNode& secondClothNode, const ScalarType springConstant);
	CSpringNodeLink(const ScalarType springConstant, const
		ScalarType springLength);
	~CSpringNodeLink();

	// Determines if the spring defintion (including the presence
	// of link nodes) is valid.
	bool							IsValid() const;

	// Creates a spring link between two cloth nodes.
	bool							LinkClothNodes(CClothNode& firstClothNode,
		CClothNode& secondClothNode);

	// Returns the force that is applied on the target node by the connected spring.
	bool							GetForceOnNode(const CClothNode& targetClothNode,
		CVector& forceVector) const;

protected:
	// Removes any existing links between the represented spring and the
	// cloth nodes.
	bool							UnlinkClothNodes();

	// Sets a reference to the cloth node that is attached to
	// the "head" of the spring.
	bool							SetHeadLink(CClothNode* pHeadLink);

	// Sets a reference to the cloth node that is attached to
	// the "tail" of the spring.
	bool							SetTailLink(CClothNode* pTailLink);


	// Determines if the specified node is linked to the spring object.
	bool							IsNodeLinked(const CClothNode& queryNode) const;


	// Determines the distance between two nodes, using their contained coordinates.
	ScalarType						DistanceBetweenNodes(const CClothNode& firstClothNode,
		const CClothNode& secondClothNode) const;

	// Spring constant ("k")
	const ScalarType				mSpringConstant;

	// Length of spring when no net force is being applied
	const ScalarType				mRestingLength;
	
	// References to cloth nodes (head and tail designations
	// do not contain any implicit prioritizations or
	// classifications).

	// First linked node.
	CClothNode*						mpHeadLink;

	// Second linked node.
	CClothNode*						mpTailLink;
};

// Type definition - list of springs attached to a single cloth
// node.
typedef	std::list<CSpringNodeLink*>	SpringNodeLinkList;


class CClothNode : public CPhysicalNodeBase
{
public:
	CClothNode();
	CClothNode(const CFloatPoint& initialLocation, 
		const ScalarType nodeMass, const bool bIsAnchorNode = false);
	~CClothNode();

	// Determines if the cloth node if valid in accordance
	// with its defining parameters.
	bool							IsValid() const;

	// Attaches a spring to the current node.
	bool							AddAttachedSpring(CSpringNodeLink&
		targetSpring);

	// Removes a spring from the current node.
	bool							RemoveAttachedSpring(CSpringNodeLink&
		attachedSpring);

	// Sums the forces imposed by all attached springs, and adds the forces
	// to the pending force vector;
	bool							SumAttachedSpringForces();

	CClothNode operator=(const CClothNode& clothNode);

protected:

	// List of attached springs (references).
	SpringNodeLinkList				mAttachedSprings;
};




class CClothNodeNetwork
{
public:

	CClothNodeNetwork(const QuantityType elementsPerRow,
		const QuantityType elementsPerColumn,
		const FloatCoord mNodeNetworkWidth, const FloatCoord mNodeNetworkHeight,
		const CFloatPoint networkCoordOffset, const ScalarType nodeMass,
		const ScalarType structSpringConstant, const ScalarType shearSpringConstant,
		const ScalarType flexionSpringConstant);
	~CClothNodeNetwork();

	// Determines if the cloth network class is valid.
	bool							IsValid() const;

	// Sets the anchor node designation flag as specified (when set, forces
	// applied to the node will not result in node motion).
	bool							SetAnchorNode (const IndexType xIndex,
		const IndexType yIndex, const bool bIsAnchorNode);

	// Generates a polygonal mesh from the cloth node network.
	bool							GenerateNodeNetworkMesh(CStaticMeshBase&
		nodeNetworkMesh) const;

	// Evaluates all forces being applied to the cloth network (internal and external).
	bool							EvaluateClothNodeNetworkForces(
		const CExternalNodeForceCollection&	externalNodeForceCollection);

	// Sets the unit of time (seconds) used when performing stepwise evaluation of applied
	// forces in the cloth simulation.
	bool							SetForceTimeQuantum(const ScalarType timeQuantum);

	// Sets the base color of the cloth (for rendering).
	void							SetClothBaseColor(const CFloatColor& clothColor);

	// Sets the specularity exponent ("shininess") of the cloth (for rendering).
	bool							SetClothSpecularity(const ScalarType specularity);

protected:

	// Constructs the cloth network, using the parameters specified during
	// object construction.
	bool							BuildClothNodeNetwork();

	// Intended for use during network construction - creates spring links
	// between the cloth network nodes.
	bool							LinkClothNodeNetworkSprings(const bool
		bUseStruturalSprings, const bool bUseShearSprings, const bool
		bUseFlexionSprings);

	// Deconstructs the cloth network, releasing any allocated memory.
	void							DestroyClothNodeNetwork();


	typedef	CStatic2DArray<CClothNode>
									ClothNodeGridType;
			
	// Grid that contains the cloth nodes.
	ClothNodeGridType				mClothNodeGrid;

	// Unit of time (seconds) used for each evaluation step in the cloth simulation.
	ScalarType						mForceTimeQuantum;


	// Base color for the cloth.
	CFloatColor						mClothBaseColor;

	// Specular highlight exponent.
	ScalarType						mClothSpecularity;

	//Initialization constants

	// Grid width (for node network construction)
	const FloatCoord				mNodeNetworkWidth;

	// Grid height (for node network construction)
	const FloatCoord				mNodeNetworkHeight;

	// Starting offset of the network grid (used to displace all
	// points by a particular amount, effectively positioning the
	// entire grid).
	const CFloatPoint				mNetworkCoordOffset;

	// Node Mass
	const ScalarType				mNodeMass;


	// Springs constants ("k")

	// Main "structural" spring constants.
	const ScalarType				mStructSpringConstant;

	// Diagonal "shear" spring constants.
	const ScalarType				mShearSpringConstant;

	// "Flexion/Bend" spring constants (for enhanced rigidity).
	const ScalarType				mFlexionSpringConstant;
};



#endif	// #ifndef _CLOTHNODENETWORK_H_