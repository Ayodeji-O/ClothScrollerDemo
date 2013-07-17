// SceneScrollerFlag.h - Scene of flag waving in the wind, with a scrolling message -
//                       declarations.


#ifndef _SCENESCROLLERFLAG_H
#define _SCENESCROLLERFLAG_H	1

#include "ClothScrollerScenesCommon.h"


class CSceneScrollerFlag : public CBaseClothScrollerScene
{
public:
	CSceneScrollerFlag();
	~CSceneScrollerFlag();

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
	// Flag scene constants.
	static const CBaseClothScrollerSceneParamBlock
									mkFlagSceneConstants;
};



#endif	//#ifndef _SCENESCROLLERFLAG_H