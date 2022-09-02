/*****************************************************************//**
 * @file   mousesym.h
 * @brief  For mouse simulation 
 * 
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
#include "define.h"
#include "enums.h"
#include "vec2.h"
#include "../elang_builder.h"

namespace el
{
	struct vec2;
	/**
	 * @brief * In Qt (and in most frameworks) mouse update should happen during mouse_move event
	 * and once before mouse press and release events in order to update the delta value properly.
	 * In game loops (in SDL etc.), update() must register at the END of the loop along with ButtonSym update. 
	 * @brief * Position varies per system depending on where it receives the coordinate values. 
	 * MouseSym does not care about the absolute coordinates, only mouse events and its relative change.
	 * You may need to input and translate this value according to your own needs.
	 */
	struct ELANG_DLL MouseSym
	{
		enum
		{
			Left = 0x00,
			Right = 0x01,
			Middle = 0x02,
		};

		MouseSym();

		/**
		 * @brief Fire this in any event that can transfer mouse press signal
		 * @brief Updates all None state to Once state
		 *
		 * @param pos : Current mouse position
		 * @param buttonIndex : Must be translated to MousSym.enum state
		 */
		void onPress(const vec2& pos, sizet buttonIndex);

		/**
		 * @brief Fire this in any event that can transfer mouse release signal
		 * @brief Updates all Hold state to Lift
		 *
		 * @param pos : Current mouse position
		 * @param buttonIndex : Must be translated to MousSym.enum state
		 */
		void onRelease(const vec2& pos, sizet buttonIndex);

		/**
		 * @brief Fire this every frame or any interval that should detect mouse signals. Updates all mouse states
		 * @brief Detects presses and releases from previous frame. If pressed (Once) update to Hold state. 
		 * If released (Lift), update to None state
		 *
		 * @brief * For more info on when exactly to invoke this method, refer to the MouseSym comment intellisense.
		 */
		void updateKeys(const vec2& pos);

		/**
		 * @brief Log current data, including mosue state and delta position as well as last clicked and last released functions 
		 * @brief Uses cout.
		 */
		void print() const;

		/**
		 * Current eInput state of mouse state
		 *
		 * @param buttonIndex : 0 for Left Mouse, 1 for Right, and 2 for Middle
		 * @return Current state of key in eInput by button index
		 */
		eInput state(sizet buttonIndex) const;
		vec2 currentPosition() const;
		vec2 previousPosition() const;
		vec2 deltaPosition() const;
		vec2 lastClickedPosition(sizet buttonIndex) const;
		vec2 lastReleasedPosition(sizet buttonIndex) const;

		/**
		 * @brief Fire this every frame or any interval that should detect mouse wheel signals
		 * 
		 * @param value : Mouse wheel delta. Mousesym does not care about magnitude or absolute direction.
		 */
		void updateWheel(float value);
		float wheel() const;

		/**
		 * @brief Completely reset the mouse simulator. This means clearing all states and previous mouse data, etc
		 * @brief **** 
		 * @brief *** WARNING: This may be absolutely necessary when transitioning between mutliple mouse event loops, for example between two windows.
		 * It really depends on the platform, but generally call this between every window change. Otherwise may result in exceptions and/or weird behavior.
		 * @brief Alternatively consider creating multiple MouseSym for every window instead of using a single global controller.
		 */
		void reset();
	private:
		float mWheel;
		vec2 mPrev, mCurr;
		vec2 mLastC[3], mLastR[3];
		eInput mState[3];
	};
}
 