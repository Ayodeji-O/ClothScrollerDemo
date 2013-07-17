// SceneScrollerTeatheredCloth.h - Cloth teathered on four corners, being blown by wind from
//                                 directly behind, with a scrolling message imprinted on
//                                 the cloth - declarations.

#ifndef _SCENESCROLLERTEATHEREDCLOTH_H
#define _SCENESCROLLERTEATHEREDCLOTH_H	1

#include "ClothScrollerScenesCommon.h"


class CSceneScrollerTeatheredCloth : public CBaseClothScrollerScene
{
public:
	CSceneScrollerTeatheredCloth();
	~CSceneScrollerTeatheredCloth();

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
									mkTeatheredClothSceneConstants;
};

#endif	// #ifndef _SCENESCROLLERTEATHEREDCLOTH_H