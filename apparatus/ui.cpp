#include "ui.h"

#include "../tools/controls.h"
#include "../elements/button.h"
#include "../elements/sprite.h"

namespace el
{
	void syncButtonsToSprite() {
		for (auto e : gStage->view<Sprite, Button>()) {
			auto rect = ent<Box>(e);
			auto circ = ent<Radius>(e);
			if (rect.has()) {
				ent<Sprite>(e)->sync(*rect);
			}
			else if (circ.has()) {
				ent<Sprite>(e)->sync(*circ);
			}
		}
	}

	void updateAllButtons(asset<Camera> camera) {
		auto pos = *camera * gMouse.currentPosition();
		for (auto e : gStage->view<Button>()) {
			ent<Button> button = e;
			auto rect = ent<Box>(e);
			auto circ = ent<Radius>(e);

			bool hit = ((rect.has() && rect->contains(pos)) ||
				(circ.has() && circ->contains(pos)));
			button->update(button, hit);
		}
	}

	void moveCameraWASD(asset<Camera> camera, float speed) {
		if (camera) {
			if (gKey.state(eKeyCode::A) == eInput::HOLD) {
				camera->move(vec3(-speed, 0, 0));
			}
			else if (gKey.state(eKeyCode::D) == eInput::HOLD) {
				camera->move(vec3(speed, 0, 0));
			}
			if (gKey.state(eKeyCode::W) == eInput::HOLD) {
				camera->move(vec3(0, speed, 0));
			}
			else if (gKey.state(eKeyCode::S) == eInput::HOLD) {
				camera->move(vec3(0, -speed, 0));
			}
		}
	}
};