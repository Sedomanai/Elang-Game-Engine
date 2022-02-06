#pragma once

#include "../elang_library_builder.h"
#include "sprite.h"
#include "basic.h"

namespace el
{
	__interface ELANG_DLL IButtonEvent
	{
		void onEnter(Entity self, Entity context);
		void onHover(Entity self, Entity context);
		void onExit(Entity self, Entity context);
		void postUpdate(Entity self, Entity context);
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
					ev->onHover(e, context);
				}
				else {
					ev->onHover(e, context);
					ev->onEnter(e, context);
				}
			}
			else if (mPrev) {
				ev->onExit(e, context);
			}
			ev->postUpdate(e, context);
		}

		template<typename T> void serialize(T& archive) {};

		obj<Entity> context;
	private:
		IButtonEvent* ev;
		bool mPrev, mCurr;
	};

	struct KeyButtonEvent
	{
		Entity left, right, up, down;
	};
}


