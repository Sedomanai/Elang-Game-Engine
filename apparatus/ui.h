#pragma once

#include "../tools/camera.h"

#include "../tools/controls.h"
#include "../elements/button.h"
#include "../elements/sprite.h"

#include "../elang_library_builder.h"

#include <tweeny/tweeny.h>

namespace el
{
	extern ELANG_DLL void syncButtonsToSprite();
	extern ELANG_DLL void updateAllButtons(asset<Camera> camera);
	extern ELANG_DLL void moveCameraWASD(asset<Camera> camera, float speed);
	extern ELANG_DLL bool zoomCamera(Camera& camera, bool mousePivot = false, double scrollMultiplier = 1.1, float lowlimit = 0.02f, float highlimit = 50.0f);
	extern ELANG_DLL void setupCameraTween(tweeny::tween<vec3>&);
	extern ELANG_DLL void tweenCameraInput(tweeny::tween<vec3>&, Camera& camera, Camera& target, int duration = 15);
	inline void tweenCamera(tweeny::tween<vec3>& tween, Camera& camera, Camera& target) {
		tweenCameraInput(tween, camera, target);
		if (tween.progress() != 1.0f)
			tween.step(1);
	}
};