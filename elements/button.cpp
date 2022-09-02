#include <elpch.h>
#include "button.h"


namespace el 
{
	void Button::update(Entity e, bool hit) {
		bool curr = hit;

		if (curr) {
			if (mPrev) {
				ev->onHover(e, context);
			} else {
				ev->onHover(e, context);
				ev->onEnter(e, context);
			}
		} else if (mPrev) {
			ev->onExit(e, context);
		}
		ev->postUpdate(e, context);
		mPrev = curr;
	}

}