#include <elpch.h>
#include "ui.h"
#include "../common/algorithm.h"
#include "../tools/camera.h"
#include "../tools/controls.h"
#include "../tools/stage.h"
#include "../elements/basic.h"
#include "../elements/button.h"
#include "../elements/sprite.h"

namespace el
{
	void syncButtonsToSprite() {
		for (auto e : gStage->view<Sprite, Button>()) {
			auto rect = obj<Box>(e);
			auto circ = obj<Radius>(e);
			if (rect.has()) {
				obj<Sprite>(e)->sync(*rect);
			} else if (circ.has()) {
				obj<Sprite>(e)->sync(*circ);
			}
		}
	}

	void updateAllButtons(asset<Camera> camera) {
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

	void moveCameraWASD(asset<Camera> camera, float speed) {
		if (camera) {
			float scale = camera->scale().x;
			speed *= scale;

			if (gKey.state(eKeyCode::A) == eInput::Hold) {
				camera->move(vec3(-speed, 0, 0));
			} else if (gKey.state(eKeyCode::D) == eInput::Hold) {
				camera->move(vec3(speed, 0, 0));
			}
			if (gKey.state(eKeyCode::W) == eInput::Hold) {
				camera->move(vec3(0, speed, 0));
			} else if (gKey.state(eKeyCode::S) == eInput::Hold) {
				camera->move(vec3(0, -speed, 0));
			}

			if (gKey.state(eKeyCode::Q) == eInput::Hold) {
				camera->scale(1.1f);
				camera->resizeZ(1.0f);
			} else if (gKey.state(eKeyCode::E) == eInput::Hold) {
				camera->scale(10.0f/11.0f);
				camera->resizeZ(1.0f);
			}
		}
	}

	bool zoomCamera(Camera& camera, bool mousePivot, double scrollMultiplier, float lowlimit, float highlimit) {
		// if scrollMultiplier is 0, don't do anything
		if (scrollMultiplier != 0) {
			// this is the actual scroll value based on mousewheel direction clamped to -1 to 1
			// it's negative (inverted) because the multiplier param is easier to read this way 
			// (x1.1, x1.2, etc.. instead of something ugly like x5/7)
			auto wheel = clamp((int32)gMouse.wheel(), -1, 1);
			auto val = pow(scrollMultiplier, -wheel);

			auto prevscale = camera.scale();
			camera.scale(vec3((float)val, (float)val, 1));
			if (camera.scale().x < lowlimit || camera.scale().x > highlimit) {
				camera.resizeX(prevscale.x);
				camera.resizeY(prevscale.y);
				return false;
			}

			if (mousePivot) {
				auto mpos = camera * gMouse.currentPosition();
				auto cpos = camera.position();
				auto delta = mpos - cpos;

				// i'll be honest.. it took a while for me to get to "pval" but I still don't know the logic
				// it was found thorugh trial and error... i may try to understand someday but for now
				auto pval = (wheel > 0) ? scrollMultiplier : 1.0 / scrollMultiplier;
				delta *= (float)((1.0 - val) * pval);
				camera.move(vec3(delta, 0));
			} 

			return true;
		} return false;
	}

	void setupCameraTween(tweeny::tween<vec3>& tween) {
		tween = tweeny::from(vec3()).to(vec3()).during(0.0f);
	}

	void tweenCameraInput(tweeny::tween<vec3>& tween, Camera& camera, Camera& target, int duration) {
		tween = tweeny::from(vec3(vec2(camera.position()), camera.scale().x))
			.to(vec3(vec2(target.position()), target.scale().x))
			.during(duration).via(tweeny::easing::quadraticOut).onStep(
				[&](vec3 v)->bool {
					camera.toX(v.x);
					camera.toY(v.y);
					camera.resizeX(v.z);
					camera.resizeY(v.z);
					return false;
				}
		);
	}

	void tweenCamera(tweeny::tween<vec3>& tween, Camera& camera, Camera& target) {
		tweenCameraInput(tween, camera, target);
		if (tween.progress() != 1.0f)
			tween.step(1);
	}
};