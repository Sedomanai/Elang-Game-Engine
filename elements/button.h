/*****************************************************************//**
 * @file   button.h
 * @brief  For a button element. May be revised in the future, for now this shall suffice
 * 
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
#include "../tools/project.h"
#include "../tools/stage.h"

namespace el
{
	/**
	 * @brief In order to make Buttons lightweight as possible, Buttons do not use signals.
	 *		  Instead, buttons receive an IButtonEvent interface to fire when the button conditions are met.
	 *		  Absolutely anything can inherit an IButtonEvent, from root level to surface level, even hub classes.
	 *		  Just make sure they do not go out of scope before the buttons
	 */
	__interface ELANG_DLL IButtonEvent
	{
		void onEnter(Entity self, Entity context);
		void onHover(Entity self, Entity context);
		void onExit(Entity self, Entity context);
		void postUpdate(Entity self, Entity context);
	};

	/**
	 * @brief Most basic and simple type of Button. 
	 */
	struct ELANG_DLL Button
	{
		Button() : ev(0), mPrev(false) {};
		Button(IButtonEvent* buttonEvent, obj<Entity> context_ = NullEntity)
			: mPrev(false), ev(buttonEvent) {}

		/**
		 * @brief Update function. Hit detection algorithm must be calculated outside this function
		 * 
		 * @param self : While intended for the entity with this Button element attached,
		 *				 this can also recieve and hold an index/identifier that can somehow refer to the button.
		 * @param hit : Check whether it is hit or not. Must be done manually, probably in another apparatus
		 */
		void update(Entity self, bool hit);

		/**
		 * @brief Used by Cereal
		 */
		template<typename T> void serialize(T& archive) { archive(context); };

		/**
		 * @brief Often, a button needs more than itself to determine its event logic.
		 *		  It's possible to attach a context to the button (other than itself) for additional data to work with.
		 * @brief As with all Entity types, it can also hold a raw custom int32 to manually parse into logic
		 * @brief * There's really no need for more than one context
		 */
		obj<Entity> context;

		/**
		 * @param event_ : IButtonEvent. In most cases you need to set the event via constructor and not change them
		 */
		void setEvent(IButtonEvent* event_) { ev = event_; }
	private:
		IButtonEvent* ev;
		bool mPrev;
	};

	struct KeyButtonEvent
	{
		Entity left, right, up, down;
	};
}


