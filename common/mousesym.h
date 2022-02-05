#pragma once

#include "../common/math.h"
#include "../common/string.h"
#include "../common/enums.h"

namespace el
{
	//=======================================================================================
	// In Qt (and in most frameworks) mouse update should happen during mouse_move event 
	// and once before mouse press and release events in order to update the delta value properly.
	// In game loops, update() must register at the END of the loop along with ButtonSym update.
	//=======================================================================================
	struct MouseSym
	{
		enum
		{
			LEFT = 0x00,
			RIGHT = 0x01,
			MIDDLE = 0x02,
		};

		void updateKeys(const vec2 pos) {
			mPrev = mCurr;
			mCurr = pos; 
			for (sizet i = 0; i < 3; i++) {
				switch (mState[i]) {
				case eInput::ONCE:
				case eInput::FLAP:
					mState[i] = eInput::HOLD;
					break;
				case eInput::LIFT:
				case eInput::SNAP:
					mState[i] = eInput::NONE;
					break;
				}
			}
		}

		void onPress(const vec2 pos, sizet buttonIndex) {
			mLastC[buttonIndex] = pos;
			switch (mState[buttonIndex]) {
			case eInput::NONE:
				mState[buttonIndex] = eInput::ONCE;
				break;
			case eInput::LIFT:
			case eInput::SNAP:
				mState[buttonIndex] = eInput::FLAP;
				break;
			}
		}

		void onRelease(const vec2 pos, sizet buttonIndex) {
			mLastR[buttonIndex] = pos;
			switch (mState[buttonIndex]) {
			case eInput::HOLD:
				mState[buttonIndex] = eInput::LIFT;
				break;
			case eInput::ONCE:
			case eInput::FLAP:
				mState[buttonIndex] = eInput::SNAP;
				break;
			}
		}

		void printPositions() const {
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
		eInput state(sizet buttonIndex) const { return (mState[buttonIndex] == eInput::FLAP) ? eInput::SNAP : mState[buttonIndex]; }

		void updateWheel(float value) { mWheel = value; }
		float wheel() const { return mWheel; }

		void reset() {
			mWheel = 0;
			mCurr = mPrev;
			for (int i = 0; i < 3; i++) {
				mLastR[i] = mLastC[i] = mCurr;
				mState[i] = eInput::NONE;
			}
		}
	private:
		float mWheel;
		vec2 mPrev, mCurr;
		vec2 mLastC[3], mLastR[3];
		eInput mState[3];
	};
}
 