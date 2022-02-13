#include "ui.h"

namespace el
{
	void syncButtonsToSprite() {
		syncButtonsToSprite<0, 0, 0>();
	}

	void updateAllButtons(asset<Camera> camera) {
		updateAllButtons<0>(camera);
	}

	void moveCameraWASD(asset<Camera> camera, float speed) {
		moveCameraWASD<0>(camera, speed);
	}
};