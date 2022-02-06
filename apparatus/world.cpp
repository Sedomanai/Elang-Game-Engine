#include "world.h"

#include "../elements/sprite.h"

namespace el
{
	void notifyPositionalChange(Observer& observer) {
		for (auto e : observer) {
			if (gStage->any_of<Sprite, Canvas>(e)) {
				auto spr = obj<Sprite>(e);
				if (spr.has()) {
					spr.update();
				}
			}
		} observer.clear();
	}
}

