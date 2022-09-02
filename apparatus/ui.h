/*****************************************************************//**
 * @file   ui.h
 * @brief  UI related appartus
 *		   Check the function descriptions
 * 
 * @author Sedomanai
 * @date   September 2022
 *********************************************************************/

#pragma once
#include <tweeny/tween.h>
#include "../tools/tool_declarer.h"
#include "../elang_builder.h"

namespace el
{
	struct Camera;
	struct vec3;

	// sync all entities with both buttons & sprites and sync them
	extern ELANG_DLL void syncButtonsToSprite();

	// update all buttons
	extern ELANG_DLL void updateAllButtons(asset<Camera> camera);

	// Attach this to game loop (input area) to move camera with WASD
	extern ELANG_DLL void moveCameraWASD(asset<Camera> camera, float speed);

	// Attach this to game loop (input area) to zoom camera with mousewheel
	// If the camera is tweened, pass the camera target instead
	extern ELANG_DLL bool zoomCamera(Camera& camera, bool mousePivot = false, double scrollMultiplier = 1.1, float lowlimit = 0.02f, float highlimit = 50.0f);

	// To tween camera, make an empty tween object and create it via this function 
	extern ELANG_DLL void setupCameraTween(tweeny::tween<vec3>&);

	// To tween camera, make a tween and pass the created tween object, camera reference, and the target camera object
	// Durtaion refers to the time it takes to sync to target 
	// This does not belong in the loop and only needs to be set up once (though apparently it doesn't really matter)
	extern ELANG_DLL void tweenCameraInput(tweeny::tween<vec3>&, Camera& camera, Camera& target, int duration = 15);

	// Invoke this in the loop to tween camera. If it's programmed to be tweened (via tweenCameraInput), 
	extern ELANG_DLL void tweenCamera(tweeny::tween<vec3>& tween, Camera& camera, Camera& target);
};