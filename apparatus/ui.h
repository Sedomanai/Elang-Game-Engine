#pragma once

#include "../tools/camera.h"

namespace el
{
	extern ELANG_DLL void syncButtonsToSprite();
	extern ELANG_DLL void updateAllButtons(asset<Camera>);
	extern ELANG_DLL void moveCameraWASD(asset<Camera> camera, float speed);
};