// ClothNodeNetwork.cpp - Contains implementations pertinent to the network of nodes required for
//                        simulating cloth movement

#include "ClothNodeNetwork.h"





/////
// CSpringNodeLink class
/////
CSpringNodeLink::CSpringNodeLink(CClothNode& firstClothNode,
	CClothNode& secondClothNode, const ScalarType springConstant) :
	mSpringConstant(springConstant),
	mRestingLength(CSpringNodeLink::DistanceBetweenNodes(firstClothNode, secondClothNode)),
	mpHeadLink(NULL), mpTailLink(NULL)
{
	// Link the cloth nodes with the spring.
	this->LinkClothNodes(firstClothNode, secondClothNode);

}


CSpringNodeLink::CSpringNodeLink(const ScalarType springConstant,
								 const ScalarType springLength) :
	mSpringConstant(springConstant),
	mRestingLength(springLength),
	mpHeadLink(NULL), mpTailLink(NULL)
{

}

CSpringNodeLink::~CSpringNodeLink()
{
	// Remove all association between cloth nodes (no further
	// action is required for any previously-linked cloth
	// nodes after this routine has been exectued).
	this->UnlinkClothNodes();
}


bool CSpringNodeLink::IsValid() const
{
	// A spring should not have a physical length less than zero;
	// also valid, linked cloth nodes must exist.
	return(this->mRestingLength >= 0 && (this->mpHeadLink != NULL) &&
		(this->mpTailLink != NULL) && this->mpHeadLink->IsValid() &&
		this->mpTailLink->IsValid());
}


bool CSpringNodeLink::LinkClothNodes(CClothNode& firstClothNode,
		CClothNode& secondClothNode)
{
	bool							bNodesLinkedSuccessfully = false;

	// Store pointers to the cloth nodes.
	this->mpHeadLink = &firstClothNode;
	this->mpTailLink = &secondClothNode;

	// Ensure that the cloth nodes are aware of the spring link.
	bNodesLinkedSuccessfully = firstClothNode.AddAttachedSpring(*this) &&
		secondClothNode.AddAttachedSpring(*this);

	return(bNodesLinkedSuccessfully);
}

bool CSpringNodeLink::GetForceOnNode(const CClothNode& targetClothNode,
									 CVector& forceVector) const
{
	bool							bForceObtainedSuccessfully = false;

	if (this->IsNodeLinked(targetClothNode) && this->IsValid() &&
		this->mpHeadLink &&	this->mpTailLink)
	{

		const ScalarType			springDisplacement = 
			this->DistanceBetweenNodes(*(this->mpHeadLink), *(this->mpTailLink)) -
			this->mRestingLength;

		// Spring force = - (Spring Constant x Spring displacement)
		const ScalarType			springForce = this->mSpringConstant *
			springDisplacement;

		// Determine the direction of the force...
		forceVector.SetAsDisplacementVector((this->mpHeadLink)->GetNodeLocation(),
			(this->mpTailLink)->GetNodeLocation());
		forceVector.Normalize();

		// Compute the final force vector (reverse the force direction, depending upon
		// which end of the spring the node resides upon).
		forceVector = (this->mpHeadLink == &targetClothNode) ?
			(forceVector * -springForce) : (forceVector * springForce);

		bForceObtainedSuccessfully = true;
	}
	else {
		// Do not attempt to compute a force vector, as the spring is not linked
		// to the target node, or a class-defining parameter is invalid.
		forceVector = CVector(0.0, 0.0, 0.0);
	}

	return(bForceObtainedSuccessfully);
}

// Removes any existing links between the represented spring and the
// cloth nodes.
bool CSpringNodeLink::UnlinkClothNodes()
{
	bool							bRemovedSuccessfully = false;

	// Notify attached springs to remove the spring reference
	// from their internal list, effectively breaking the
	// link between the spring and the cloth node.
	if (this->mpHeadLink) {
		(*(this->mpHeadLink)).RemoveAttachedSpring(*this);
		mpHeadLink = NULL;

		bRemovedSuccessfully = true;
	}

	if (this->mpTailLink) {
		mpTailLink->RemoveAttachedSpring(*this);
		mpTailLink = NULL;
	}
	else {
		bRemovedSuccessfully = false;
	}

	return(bRemovedSuccessfully);
}

bool CSpringNodeLink::SetHeadLink(CClothNode* pHeadLink)
{
	bool							bSetSuccessfully = false;

	// NULL pointers are not allowed for head/tail links - a spring
	// should always be attached on both terminal points.
	if (AssertExpression(pHeadLink != NULL)) {
		this->mpHeadLink = pHeadLink;
		bSetSuccessfully = true;
	}

	return(bSetSuccessfully);
}


bool CSpringNodeLink::SetTailLink(CClothNode* pTailLink)
{
	bool							bSetSuccessfully = false;

	// NULL pointers are not allowed for head/tail links - a spring
	// should always be attached on both terminal points.
	if (AssertExpression(pTailLink != NULL)) {
		this->mpTailLink = pTailLink;
		bSetSuccessfully = true;
	}

	return(bSetSuccessfully);
}


bool CSpringNodeLink::IsNodeLinked(const CClothNode& queryNode) const
{
	// Determine if the cloth node is linked to either the
	// start or the end of the spring.
	return((&queryNode == this->mpHeadLink) ||
		(&queryNode == this->mpTailLink));
}

ScalarType	CSpringNodeLink::DistanceBetweenNodes(const CClothNode& firstClothNode,
												  const CClothNode& secondClothNode) const
{
	// Determine the distance between nodes, using their contained coordinates.
	const CFloatPoint				firstClothNodeCoord = firstClothNode.GetNodeLocation();
	const CFloatPoint				secondClothNodeCoord = secondClothNode.GetNodeLocation();

	return(firstClothNodeCoord.DistanceFrom(secondClothNodeCoord));
}

/////
// CClothNode class
/////
CClothNode::CClothNode() :
	CPhysicalNodeBase()
{
	// Copy constructor
}


CClothNode::CClothNode(const CFloatPoint& initialLocation, 
					   const ScalarType nodeMass, const bool bIsAnchorNode) :
	CPhysicalNodeBase(initialLocation, nodeMass, bIsAnchorNode)
{
	this->mAttachedSprings.clear();
}

CClothNode::~CClothNode()
{

}


bool CClothNode::IsValid() const
{
	// The node must have springs attached, and have
	// a non-zero mass in order to be valid.
	const bool						bSpringsAttached =
		(this->mAttachedSprings.size() > 0);

	const bool						bMassValid = 
		CPhysicalNodeBase::IsValid();


	return(bSpringsAttached && bMassValid);
}

// Attaches a spring to the current node.
bool CClothNode::AddAttachedSpring(CSpringNodeLink& targetSpring)
{
	bool							bAddedSuccessfully = false;

	// Determine if the spring exists in the list already...
	SpringNodeLinkList::iterator	springListIterator;
		
	springListIterator = std::find(this->mAttachedSprings.begin(), this->mAttachedSprings.end(),
		&targetSpring);

	// ...If the spring does not exist in the list, add it to the list.
	if (springListIterator == mAttachedSprings.end())
	{
		this->mAttachedSprings.push_back(&targetSpring);
		bAddedSuccessfully = true;
	}

	return(bAddedSuccessfully);
}

// Removes a spring from the current node.
bool CClothNode::RemoveAttachedSpring(CSpringNodeLink& attachedSpring)
{
	bool							bRemovedSuccessfully = false;

	// Attempt to find the spring in the spring list...
	SpringNodeLinkList::iterator	springListIterator;
		
	springListIterator = std::find(this->mAttachedSprings.begin(), this->mAttachedSprings.end(),
		&attachedSpring);

	// ...Remove the spring from the list of springs.
	if (springListIterator != mAttachedSprings.end())
	{
		this->mAttachedSprings.erase(springListIterator);
	}

	return(bRemovedSuccessfully);
}


bool CClothNode::SumAttachedSpringForces()
{
	bool							bForcesSummedSuccessfully =
		!this->mAttachedSprings.empty();

	CVector							springForceSum(0.0, 0.0, 0.0);
	CVector							currentSpringForce(0.0, 0.0, 0.0);

	// Iterate through the list of attached springs...
	for (SpringNodeLinkList::iterator springListIterator = (this->mAttachedSprings).begin();
		springListIterator != (this->mAttachedSprings).end(); springListIterator++)
	{
		// Sum the forces imposed by all attached springs.
		bForcesSummedSuccessfully = (*springListIterator)->GetForceOnNode(*this,
			currentSpringForce);
		if (*springListIterator && bForcesSummedSuccessfully)
		{
			springForceSum = springForceSum + currentSpringForce;
		}
	}

	// Add the imposed spring force sum to the force vector that is to be evaluated.
	this->mPendingForceVector = this->mPendingForceVector + springForceSum;

	return(bForcesSummedSuccessfully);
}

CClothNode CClothNode::operator=(const CClothNode& clothNode)
{
	// Copy the data resident in the source cloth node class.
	::memcpy(this, &clothNode, sizeof(*this) - sizeof(this->mAttachedSprings));

	//this->mNodeLocation = clothNode.mNodeLocation;
	//this->mNodeMass = clothNode.mNodeMass;
	//this->mPendingForceVector = clothNode.mPendingForceVector;
	//this->mVelocityVector = clothNode.mVelocityVector;
	//this->mbIsAnchorNode = clothNode.mbIsAnchorNode;

	// ...Explicitly copy the node list data - (this structure
	// will be invalid if the source class is destroyed).
	//this->mAttachedSprings.clear();
	//this->mAttachedSprings = clothNode.mAttachedSprings;

	return(clothNode);
}


/////
// CClothNodeNetwork class
/////
CClothNodeNetwork::CClothNodeNetwork(const QuantityType elementsPerRow,
		const QuantityType elementsPerColumn,
		const FloatCoord mNodeNetworkWidth, const FloatCoord mNodeNetworkHeight,
		const CFloatPoint networkCoordOffset, const ScalarType nodeMass,
		const ScalarType structSpringConstant, const ScalarType shearSpringConstant,
		const ScalarType flexionSpringConstant) :

	mClothNodeGrid(elementsPerRow, elementsPerColumn), mNodeNetworkWidth(mNodeNetworkWidth),
	mNodeNetworkHeight(mNodeNetworkHeight), mNetworkCoordOffset(networkCoordOffset), 
	mNodeMass(nodeMass), mStructSpringConstant(structSpringConstant),
	mShearSpringConstant(shearSpringConstant), mFlexionSpringConstant(flexionSpringConstant),
	mForceTimeQuantum(0.0), mClothBaseColor(kMaxColorComponentValue, kMaxColorComponentValue,
	kMaxColorComponentValue), mClothSpecularity(kMaxColorComponentValue)
{
	// Construct the cloth network.
	AssertExpression(BuildClothNodeNetwork());
}

CClothNodeNetwork::~CClothNodeNetwork()
{
	// Destroy the created cloth network.s
	DestroyClothNodeNetwork();
}

	
bool CClothNodeNetwork::IsValid() const
{
	// The basic node network definition is valid if the grid 
	// definition is valid, and the node mass is non-negative.
	const bool						bNodeMassIsValid = (this->mNodeMass >= 0);

	return(bNodeMassIsValid && this->mClothNodeGrid.IsValid() &&
		(this->mForceTimeQuantum > 0.0));
}


bool CClothNodeNetwork::SetAnchorNode (const IndexType xIndex,
		const IndexType yIndex, const bool bIsAnchorNode)
{
	bool							bAnchorNodeSet = false;

	if ((xIndex < this->mClothNodeGrid.GetRowSize()) &&
		(yIndex < this->mClothNodeGrid.GetColumnSize()))
	{
		CClothNode*					pTargetClothNode = 
			this->mClothNodeGrid.GetElementAt(xIndex, yIndex);

		// Attempt to refrence the node using the provided grid
		// indices...
		if(pTargetClothNode) {
			// ...Set the anchor node status.
			pTargetClothNode->SetAnchorNodeStatus(bIsAnchorNode);
		}

		bAnchorNodeSet = true;

	}

	return(bAnchorNodeSet);
}


bool CClothNodeNetwork::GenerateNodeNetworkMesh(CStaticMeshBase& nodeNetworkMesh) const
{
	bool							bGeneratedSuccessfully = false;


	if (this->IsValid())
	{
		// Retrieve node network dimensions (stored in grid during object
		// construction).
		const QuantityType			elementsPerRow =
			this->mClothNodeGrid.GetRowSize();
		const QuantityType			elementsPerColumn = 
			this->mClothNodeGrid.GetColumnSize();

		const CVector				kNormalVector(0.0, 0.0, 1.0);

		if ((elementsPerRow > 0) && (elementsPerColumn > 0)) {

			PolygonList				nodeMeshPolygons;

			for (QuantityType xLoop = 0; xLoop < (elementsPerRow - 1); xLoop++) {
				for (QuantityType yLoop = 0; yLoop < (elementsPerColumn - 1); yLoop++) {

					CFloatPolygon	firstHalfPolygon;
					CFloatPolygon	secondHalfPolygon;
					
					const CClothNode*
									pUpperLeftNode = 
									this->mClothNodeGrid.GetElementAt(xLoop, yLoop);
					const CClothNode*
									pUpperRightNode = 
									this->mClothNodeGrid.GetElementAt(xLoop + 1, yLoop);
					const CClothNode*
									pLowerLeftNode =
									this->mClothNodeGrid.GetElementAt(xLoop, yLoop + 1);
					const CClothNode*
									pLowerRightNode = 
									this->mClothNodeGrid.GetElementAt(xLoop + 1, yLoop + 1);

					const ScalarType		uCoordLeft = ((ScalarType)xLoop /
						(ScalarType)(elementsPerRow - 1));
					const ScalarType		uCoordRight = ((ScalarType)(xLoop + 1) /
						(ScalarType)(elementsPerRow - 1));
					const ScalarType		vCoordTop = ((ScalarType)yLoop /
						(ScalarType)(elementsPerColumn - 1));
					const ScalarType		vCoordBottom = ((ScalarType)(yLoop + 1) /
						(ScalarType)(elementsPerColumn - 1));
					/*const ScalarType		vCoordTop =
						(ScalarType)(elementsPerColumn - 2 - yLoop) /
						(ScalarType)(elementsPerColumn - 1);
					const ScalarType		vCoordBottom =
						(ScalarType)(elementsPerColumn - 1 - yLoop) /
						(ScalarType)(elementsPerColumn - 1);*/

					if (pUpperLeftNode && pUpperRightNode && pLowerLeftNode && pLowerRightNode) {
						firstHalfPolygon.AddVertex(
							CPolygonVertex(pUpperLeftNode->GetNodeLocation(), kNormalVector,
							mClothBaseColor, uCoordLeft, vCoordTop));
						firstHalfPolygon.AddVertex(
							CPolygonVertex(pLowerRightNode->GetNodeLocation(), kNormalVector,
							mClothBaseColor, uCoordRight, vCoordBottom));
						firstHalfPolygon.AddVertex(
							CPolygonVertex(pLowerLeftNode->GetNodeLocation(), kNormalVector,
							mClothBaseColor, uCoordLeft, vCoordBottom));

						secondHalfPolygon.AddVertex(
							CPolygonVertex(pUpperLeftNode->GetNodeLocation(), kNormalVector,
							mClothBaseColor, uCoordLeft, vCoordTop));

						secondHalfPolygon.AddVertex(
							CPolygonVertex(pUpperRightNode->GetNodeLocation(), kNormalVector,
							mClothBaseColor, uCoordRight, vCoordTop));

						secondHalfPolygon.AddVertex(
							CPolygonVertex(pLowerRightNode->GetNodeLocation(), kNormalVector,
							mClothBaseColor, uCoordRight, vCoordBottom));


						// Set the flag color for the current polygon set.
						firstHalfPolygon.SetLightShading(eShadeDiffuse, this->mClothBaseColor);
						secondHalfPolygon.SetLightShading(eShadeDiffuse, this->mClothBaseColor);
						firstHalfPolygon.SetLightShading(eShadeSpecular, this->mClothBaseColor);
						secondHalfPolygon.SetLightShading(eShadeSpecular, this->mClothBaseColor);

						// Set the flag specularity for the current polygon set.
						firstHalfPolygon.SetSpecularity(this->mClothSpecularity);
						secondHalfPolygon.SetSpecularity(this->mClothSpecularity);


						nodeMeshPolygons.push_back(firstHalfPolygon);
						nodeMeshPolygons.push_back(secondHalfPolygon);
					}
				}
			}

			if (nodeMeshPolygons.size() > 0)
			{
				bGeneratedSuccessfully = nodeNetworkMesh.SetModelGeometry(nodeMeshPolygons);
			}
		}
	}


	return(bGeneratedSuccessfully);
}


bool CClothNodeNetwork::EvaluateClothNodeNetworkForces(const CExternalNodeForceCollection&
													   externalNodeForceCollection)
{
	bool							bEvaluatedSuccessfully = false;

	if (this->IsValid()) {
		// Retrieve node network dimensions (stored in grid during object
		// construction).
		const QuantityType			elementsPerRow =
			this->mClothNodeGrid.GetRowSize();
		const QuantityType			elementsPerColumn = 
			this->mClothNodeGrid.GetColumnSize();

		

		// Iterate through the nodes in the cloth node network...
		for (QuantityType yLoop = 0; yLoop < (elementsPerColumn); yLoop++) {
			for (QuantityType xLoop = 0; xLoop < (elementsPerRow); xLoop++) {
				CClothNode*			pCurrentClothNode =
					this->mClothNodeGrid.GetElementAt(xLoop, yLoop);
				
				if (pCurrentClothNode) {
					CVector			resultantForceVector;
					// First, evaluate external forces for each node.
					externalNodeForceCollection.EvaluateExternalForces(*pCurrentClothNode,
						resultantForceVector, this->mForceTimeQuantum);
					pCurrentClothNode->SetForceVector(resultantForceVector);

					// Now, evaluate internal network (spring) forces for each node.
					pCurrentClothNode->SumAttachedSpringForces();


					bEvaluatedSuccessfully = true;
				}
			}
		}

		

		// Apply the forces to each node in the network.
		for (QuantityType yLoopApply = 0; yLoopApply < (elementsPerColumn); yLoopApply++) {
			for (QuantityType xLoopApply = 0; xLoopApply < (elementsPerRow); xLoopApply++) {
				CClothNode*			pCurrentClothNode =
					this->mClothNodeGrid.GetElementAt(xLoopApply, yLoopApply);
				
				if (pCurrentClothNode) {
					pCurrentClothNode->ApplyForceVector(this->mForceTimeQuantum);
	
					bEvaluatedSuccessfully = true;
				}
			}
		}
						
	}


	return(bEvaluatedSuccessfully);
}

bool CClothNodeNetwork::SetForceTimeQuantum(const ScalarType timeQuantum)
{
	bool							bQuantumSetSuccessfully = false;

	if (timeQuantum > 0.0) {
		// A time quantum must be greater than zero in order for a simulation
		// step to be evaluated with valid results.
		this->mForceTimeQuantum = timeQuantum;
	}

	return(bQuantumSetSuccessfully);
}

// Sets the base color of the cloth (for rendering).
void CClothNodeNetwork::SetClothBaseColor(const CFloatColor& clothColor)
{
	// Store the cloth color.
	this->mClothBaseColor = clothColor;
}

// Sets the specularity exponent ("shininess") of the cloth (for rendering).
bool CClothNodeNetwork::SetClothSpecularity(const ScalarType specularity)
{
	bool							bSpecularitySetSuccessfully = false;

	if((specularity >= kMinSpecularityValue) && (specularity <= kMaxSpecularityValue)) {
		// Specularity value is within bounds - set the flag specularity.
		this->mClothSpecularity = specularity;
		bSpecularitySetSuccessfully = true;
	}

	return(bSpecularitySetSuccessfully);
}


bool CClothNodeNetwork::BuildClothNodeNetwork()
{
	bool							bNetworkBuiltSuccessfully = false;

	if (this->mClothNodeGrid.IsValid())
	{
		// Retrieve node network dimensions (stored in grid during object
		// construction).
		const QuantityType			elementsPerRow =
			this->mClothNodeGrid.GetRowSize();
		const QuantityType			elementsPerColumn = 
			this->mClothNodeGrid.GetColumnSize();

		if ((elementsPerRow > 0) && (elementsPerColumn > 0) &&
			(this->mNodeNetworkWidth > 0.0) && (this->mNodeNetworkHeight > 0.0))
		{
			// Determine the spacing (coordinate stride) between successive
			// cloth network nodes.
			const FloatCoord		xSpacing = (this->mNodeNetworkWidth /
				elementsPerRow);
			const FloatCoord		ySpacing = (this->mNodeNetworkHeight /
				elementsPerColumn);

			bNetworkBuiltSuccessfully = true;

			for (QuantityType yLoop = 0; yLoop < elementsPerColumn; yLoop++) {
				for (QuantityType xLoop = 0; xLoop < elementsPerRow; xLoop++) {

					const CFloatPoint
									currentNodeCoords(((FloatCoord)xLoop * xSpacing),
									((FloatCoord)yLoop * ySpacing),	0.0);

					// Create a cloth network node, and store it within the current
					// grid location..
					CClothNode		currentNode((currentNodeCoords + mNetworkCoordOffset),
						this->mNodeMass, false);
						
					bNetworkBuiltSuccessfully =
						this->mClothNodeGrid.SetElementAt(xLoop, yLoop, currentNode) &&
						bNetworkBuiltSuccessfully;
				}
			}

			// Link the nodes with springs that are essential to performing the
			// cloth motion simulation.
			bNetworkBuiltSuccessfully = this->LinkClothNodeNetworkSprings(true,
				true, true) && bNetworkBuiltSuccessfully;
		}
	}

	return(bNetworkBuiltSuccessfully);
}


bool CClothNodeNetwork::LinkClothNodeNetworkSprings(const bool bUseStruturalSprings,
													const bool bUseShearSprings,
													const bool bUseFlexionSprings)
{
	bool							bLinkedSuccessfully = false;

	// Interval between nodes for flexion spring connections (horizontal and vertical,
	// respectively).
	const QuantityType				kFlexionSpringXStride = 3;
	const QuantityType				kFlexionSpringYStride = kFlexionSpringXStride;
	
	if (this->mClothNodeGrid.IsValid())
	{
		// Retrieve node network dimensions (stored in grid during object
		// construction).
		const QuantityType			elementsPerRow =
			this->mClothNodeGrid.GetRowSize();
		const QuantityType			elementsPerColumn = 
			this->mClothNodeGrid.GetColumnSize();

		CClothNode*					pFirstClothNode = NULL;
		CClothNode*					pSecondClothNode = NULL;

		if ((elementsPerRow > 0) && (elementsPerColumn > 0)) {

			bLinkedSuccessfully = true;

			for (QuantityType xLoop = 0; xLoop < elementsPerRow; xLoop++) {
				for (QuantityType yLoop = 0; yLoop < elementsPerColumn; yLoop++) {

					pFirstClothNode = this->mClothNodeGrid.GetElementAt(xLoop, yLoop);

					// Create structural springs.

					if (pFirstClothNode) {
						// Create inter-column structural spring.
						if ((xLoop + 1) < elementsPerRow) {
							pSecondClothNode = this->mClothNodeGrid.GetElementAt(xLoop + 1, yLoop);
							
							if (pSecondClothNode) {
								CSpringNodeLink*
										pSpringNodeLink = new /*(std::nothrow)*/ CSpringNodeLink(*pFirstClothNode,
											*pSecondClothNode, this->mStructSpringConstant);

								bLinkedSuccessfully = pSpringNodeLink && bLinkedSuccessfully;
							}
						}

						// Create inter-row structural spring.
						if ((yLoop + 1) < elementsPerColumn) {
							pSecondClothNode = this->mClothNodeGrid.GetElementAt(xLoop,
								yLoop + 1);

							CSpringNodeLink*
								pSpringNodeLink = new /*(std::nothrow)*/ CSpringNodeLink(*pFirstClothNode,
								*pSecondClothNode, this->mStructSpringConstant);

							bLinkedSuccessfully = pSpringNodeLink && bLinkedSuccessfully;
						}
					

						// Create shear springs.
						if (((xLoop + 1) < elementsPerRow) && ((yLoop + 1) < elementsPerColumn)) {
							// Create inter-column shear spring.
							pSecondClothNode = this->mClothNodeGrid.GetElementAt(xLoop + 1, yLoop + 1);

							if (pSecondClothNode) {
								CSpringNodeLink*
										pFirstNodeLink = new /*(std::nothrow)*/ CSpringNodeLink(*pFirstClothNode,
										*pSecondClothNode, this->mShearSpringConstant);
							}
						
							// Create inter-row shear spring.
							pFirstClothNode = this->mClothNodeGrid.GetElementAt(xLoop,
								yLoop + 1);

							pSecondClothNode =
								this->mClothNodeGrid.GetElementAt(xLoop + 1, yLoop);

							if (pFirstClothNode && pSecondClothNode) {
								CSpringNodeLink*
										pSecondNodeLink = new /*(std::nothrow)*/ CSpringNodeLink(*pFirstClothNode,
										*pSecondClothNode, this->mShearSpringConstant);
							}
						}
					}

					// Create flexion springs.

					// Create inter-column flexion spring.
					if ((xLoop + 2) < elementsPerRow) {
						pFirstClothNode = this->mClothNodeGrid.GetElementAt(xLoop, yLoop);

						pSecondClothNode = this->mClothNodeGrid.GetElementAt(xLoop + 2,
							yLoop);
						
						if (pFirstClothNode && pSecondClothNode) {
							CSpringNodeLink*
								pSpringNodeLink = new /*(std::nothrow)*/ CSpringNodeLink(*pFirstClothNode,
								*pSecondClothNode, this->mStructSpringConstant);

							bLinkedSuccessfully = pSpringNodeLink && bLinkedSuccessfully;
						}
					}

					// Create inter-row flexion spring.
					if ((yLoop + 2) < elementsPerColumn) {
						pSecondClothNode = this->mClothNodeGrid.GetElementAt(xLoop,
							yLoop + 2);

						if (pFirstClothNode && pSecondClothNode) {
							CSpringNodeLink*
								pSpringNodeLink = new /*(std::nothrow)*/ CSpringNodeLink(*pFirstClothNode,
								*pSecondClothNode, this->mStructSpringConstant);

							bLinkedSuccessfully = pSpringNodeLink && bLinkedSuccessfully;
						}
					}



				}
			}
		}
	}


	return(bLinkedSuccessfully);
}


void CClothNodeNetwork::DestroyClothNodeNetwork()
{
	// (Free allocated spring memory).

}



