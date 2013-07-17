// SceneScrollerTapestry.h - Tapestry suspended on top end, being blown by wind from the rear
//                           in a diagonal-upward direction, with a scrolling message on the
//                           tapestry - declarations.


#ifndef _SCENESCROLLERTAPESTRY_H
#define _SCENESCROLLERTAPESTRY_H 1

#include "ClothScrollerScenesCommon.h"


class CSceneScrollerTapestry : public CBaseClothScrollerScene
{
public:
	CSceneScrollerTapestry();
	~CSceneScrollerTapestry();

	//Initializes the scene - this function must be called before generating
	//a scene.
	bool							InitializeScene();

	//Positions the scene camera (this is the point of view from which the scene will
	//be rendered).
	void							PositionSceneCamera(const QuantityType currentClockTick);

	//Performs a stepwise scene update.
	bool							UpdateScene(const QuantityType currentClockTick);

	//Renders the scene (does not perform a flush or buffer swap).
	bool							RenderScene() const;

protected:
	static const CBaseClothScrollerSceneParamBlock
									mkTapestrySceneConstants;
};


#endif	// #ifndef _SCENESCROLLERTAPESTRY_H