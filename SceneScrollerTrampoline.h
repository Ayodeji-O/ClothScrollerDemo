// SceneScrollerTrampoline.h - Cloth restrained in a "trampoline" fashion (all edges are immobile),
//                             being blown from the rear by wind, with a scrolling message
//                             imprinted on the cloth - declarations.


#ifndef _SCENESCROLLERTRAMPOLINE_H
#define _SCENESCROLLERTRAMPOLINE_H	1


#include "ClothScrollerScenesCommon.h"


class CSceneScrollerTrampoline : public CBaseClothScrollerScene
{
public:
	CSceneScrollerTrampoline();
	~CSceneScrollerTrampoline();

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
									mkTrampolineSceneConstants;
};






#endif	// #ifndef _SCENESCROLLERTRAMPOLINE_H