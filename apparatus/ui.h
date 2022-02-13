#pragma once

#include "../tools/camera.h"

#include "../tools/controls.h"
#include "../elements/button.h"
#include "../elements/sprite.h"

namespace el
{
	template<int M, int T, int C>
	inline void syncButtonsToSprite() {
		for (auto e : gStage->view<SpriteImpl<SpriteVertex, M, T, C>, Button>()) {
			auto rect = obj<Box>(e);
			auto circ = obj<Radius>(e);
			if (rect.has()) {
				obj<SpriteImpl<SpriteVertex, M, T, C>>(e)->sync(*rect);
			}
			else if (circ.has()) {
				obj<SpriteImpl<SpriteVertex, M, T, C>>(e)->sync(*circ);
			}
		}
	}

	template<int C>
	inline void updateAllButtons(asset<CameraImpl<C>> camera) {
		auto pos = *camera * gMouse.currentPosition();
		for (auto e : gStage->view<Button>()) {
			obj<Button> button = e;
			auto rect = obj<Box>(e);
			auto circ = obj<Radius>(e);

			bool hit = ((rect.has() && rect->contains(pos)) ||
				(circ.has() && circ->contains(pos)));
			button->update(button, hit);
		}
	}

	template<int N>
	void moveCameraWASD(asset<CameraImpl<N>> camera, float speed) {
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

	extern ELANG_DLL void syncButtonsToSprite();
	extern ELANG_DLL void updateAllButtons(asset<Camera>);
	extern ELANG_DLL void moveCameraWASD(asset<Camera> camera, float speed);
};