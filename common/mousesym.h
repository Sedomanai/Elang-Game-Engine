/*****************************************************************//**
 * @file   mousesym.h
 * @brief  For mouse simulation 
 * 
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/

#pragma once

#include "../common/math.h"
#include "../common/string.h"
#include "../common/enums.h"

namespace el
{
	/**
	 * @brief In Qt (and in most frameworks) mouse update should happen during mouse_move event
	 * and once before mouse press and release events in order to update the delta value properly.
	 * In game loops (in SDL etc.), update() must register at the END of the loop along with ButtonSym update. 
	 * @brief *
	 * @brief Position varies per system depending on where it receives the coordinate values. 
	 * MouseSym does not care about the absolute coordinates, only mouse events and its relative change.
	 * You may need to input and translate this value according to your own needs.
	 */
	struct MouseSym
	{
		enum
		{
			Left = 0x00,
			Right = 0x01,
			Middle = 0x02,
		};

		/**
		 * Fire this in any event that can transfer mouse press signal.
		 * Updates all None state to Once state
		 *
		 * @param pos- Current mouse position
		 * @param buttonIndex- Must be translated to MousSym.enum state
		 */
		void onPress(const vec2 pos, sizet buttonIndex) {
			mLastC[buttonIndex] = pos;
			switch (mState[buttonIndex]) {
			case eInput::None:
				mState[buttonIndex] = eInput::Once;
				break;
			case eInput::Lift:
			case eInput::Snap:
				mState[buttonIndex] = eInput::Flap;
				break;
			}
		}

		/**
		 * Fire this in any event that can transfer mouse release signal.
		 * Updates all Hold state to Lift
		 *
		 * @param pos- Current mouse position
		 * @param buttonIndex- Must be translated to MousSym.enum state
		 */
		void onRelease(const vec2 pos, sizet buttonIndex) {
			mLastR[buttonIndex] = pos;
			switch (mState[buttonIndex]) {
			case eInput::Hold:
				mState[buttonIndex] = eInput::Lift;
				break;
			case eInput::Once:
			case eInput::Flap:
				mState[buttonIndex] = eInput::Snap;
				break;
			}
		}

		/**
		 * Fire this every frame or any interval that should detect mouse signals. Updates all mouse states.
		 * Detects presses and releases from previous frame. If pressed (Once state) update to Hold state.
		 * If released (Lift state) update to Lift state.
		 *
		 * For more info on when exactly to invoke this method, refer to the MouseSym comment intellisense.
		 */
		void updateKeys(const vec2 pos) {
			mPrev = mCurr;
			mCurr = pos;
			for (sizet i = 0; i < 3; i++) {
				switch (mState[i]) {
				case eInput::Once:
				case eInput::Flap:
					mState[i] = eInput::Hold;
					break;
				case eInput::Lift:
				case eInput::Snap:
					mState[i] = eInput::None;
					break;
				}
			}
		}

		/**
		 * Log current data, including mosue state and delta position as well as last clicked and last released functions. 
		 * Uses cout.
		 */
		void print() const {
			cout << "=========== Mouse State ===============" << endl;
			cout << "Current State: " << sizet(mState[0]) << endl;
			cout << "Delta Position: " << deltaPosition().x << " " << deltaPosition().y << " Previous Position: " << endl;
			//cout << "Current Position: " << mCurr.x << " " << mCurr.y << " Previous Position: " << mPrev.x << " " << mPrev.y << endl;
			cout << "Last Clicked: " << mLastC[0].x << " " << mLastC[0].y << " Last Released: " << mLastR[0].x << " " << mLastR[0].y << endl;
			cout << "=======================================" << endl;
		}

		vec2 currentPosition() const { return mCurr; }
		vec2 previousPosition() const { return mPrev; }
		vec2 deltaPosition() const { return mCurr - mPrev; }
		vec2 lastClickedPosition(sizet buttonIndex) const { return mLastC[buttonIndex]; }
		vec2 lastReleasedPosition(sizet buttonIndex) const { return mLastR[buttonIndex]; }
		eInput state(sizet buttonIndex) const { return (mState[buttonIndex] == eInput::Flap) ? eInput::Snap : mState[buttonIndex]; }


		/**
		 * Fire this every frame or any interval that should detect mouse wheel signals. 
		 * 
		 * @param value - Mouse wheel delta. Mousesym does not care about magnitude or absolute direction.
		 */
		void updateWheel(float value) { mWheel = value; }
		float wheel() const { return mWheel; }

		void reset() {
			mWheel = 0;
			mCurr = mPrev;
			for (int i = 0; i < 3; i++) {
				mLastR[i] = mLastC[i] = mCurr;
				mState[i] = eInput::None;
			}
		}
	private:
		float mWheel;
		vec2 mPrev, mCurr;
		vec2 mLastC[3], mLastR[3];
		eInput mState[3];
	};
}
 