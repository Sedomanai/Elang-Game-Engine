#pragma once

#include "sprite.h"
#include "basic.h"

namespace el
{
	__interface IButtonEvent
	{
		void onEnter(Entity e) {};
		void onHover(Entity e) {};
		void onExit(Entity e) {};
		void postUpdate(Entity e) {};
	};

	struct Button
	{
		Button() : ev(0), mPrev(false), mCurr(false) {};
		Button(IButtonEvent* buttonEvent)
			: mPrev(false), mCurr(false), ev(buttonEvent) {}

		void update(Entity e, bool hit) {
			mPrev = mCurr;
			mCurr = hit;

			if (mCurr) {
				if (mPrev) {
					ev->onHover(e);
				}
				else {
					ev->onHover(e);
					ev->onEnter(e);
				}
			}
			else if (mPrev) {
				ev->onExit(e);
			}
			ev->postUpdate(e);
		}

		template<typename T> void serialize(T& archive) {};

	private:
		IButtonEvent* ev;
		bool mPrev, mCurr;
	};

	struct KeyButtonEvent
	{
		Entity left, right, up, down;
	};
}


