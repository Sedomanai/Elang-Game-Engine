#pragma once

#include "../tools/camera.h"

#include "../tools/controls.h"
#include "../elements/button.h"
#include "../elements/sprite.h"

namespace el
{
	extern ELANG_DLL void syncButtonsToSprite();
	extern ELANG_DLL void updateAllButtons(asset<Camera>);
	extern ELANG_DLL void moveCameraWASD(asset<Camera> camera, float speed);
};