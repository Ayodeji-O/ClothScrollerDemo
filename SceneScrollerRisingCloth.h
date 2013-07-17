// SceneScrollerRisingCloth.h - Cloth teathered to the ground on one end, being suspended
//                              by upward-blowing wind, with a scrolling message displayed
//                              on the cloth - declarations.

#ifndef _SCENESCROLLERRISINGCLOTH_H
#define _SCENESCROLLERRISINGCLOTH_H	1

#include "ClothScrollerScenesCommon.h"


class CSceneScrollerRisingCloth : public CBaseClothScrollerScene
{
public:
	CSceneScrollerRisingCloth();
	~CSceneScrollerRisingCloth();

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
	// Rising cloth scene constants.
	static const CBaseClothScrollerSceneParamBlock
									mkRisingClothSceneConstants;
};


#endif	// #ifndef _SCENESCROLLERRISINGCLOTH_H