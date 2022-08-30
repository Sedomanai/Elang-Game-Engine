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

	struct ELANG_DLL Button
	{
		Button() : ev(0), mPrev(false), mCurr(false) {};
		Button(IButtonEvent* buttonEvent, obj<Entity> context_ = NullEntity)
			: mPrev(false), mCurr(false), ev(buttonEvent) {}

		void update(Entity self, bool hit);

		template<typename T> void serialize(T& archive) { archive(context); };

		obj<Entity> context;
		void setEvent(IButtonEvent* event_) { ev = event_; }
	private:
		IButtonEvent* ev;
		bool mPrev, mCurr;
	};

	struct KeyButtonEvent
	{
		Entity left, right, up, down;
	};
}


