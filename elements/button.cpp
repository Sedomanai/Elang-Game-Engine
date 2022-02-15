#include "button.h"

namespace el 
{
	void Button::update(Entity e, bool hit) {
		mCurr = hit;

		if (mCurr) {
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
		mPrev = mCurr;
	}

}