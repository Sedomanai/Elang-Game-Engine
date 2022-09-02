#include <elpch.h>
#include "mousesym.h"
#include "vecstream.h"

namespace el
{
	MouseSym::MouseSym() : mWheel(0.0f), mState{} {};

	void MouseSym::onPress(const vec2& pos, sizet buttonIndex) {
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

	void MouseSym::onRelease(const vec2& pos, sizet buttonIndex) {
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

	void MouseSym::updateKeys(const vec2& pos) {
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

	void MouseSym::print() const {
		cout << "=========== Mouse State ===============" << endl;
		cout << "Current State: " << sizet(mState[0]) << endl;
		cout << "Delta Position: " << deltaPosition() << " Previous Position: " << endl;
		//cout << "Current Position: " << mCurr.x << " " << mCurr.y << " Previous Position: " << mPrev.x << " " << mPrev.y << endl;
		cout << "Last Clicked: " << mLastC << " Last Released: " << mLastR << endl;
		cout << "=======================================" << endl;
	}

	vec2 MouseSym::currentPosition() const { return mCurr; }
	vec2 MouseSym::previousPosition() const { return mPrev; }
	vec2 MouseSym::deltaPosition() const { return mCurr - mPrev; }
	vec2 MouseSym::lastClickedPosition(sizet buttonIndex) const { return mLastC[buttonIndex]; }
	vec2 MouseSym::lastReleasedPosition(sizet buttonIndex) const { return mLastR[buttonIndex]; }
	eInput MouseSym::state(sizet buttonIndex) const { return (mState[buttonIndex] == eInput::Flap) ? eInput::Snap : mState[buttonIndex]; }

	void MouseSym::updateWheel(float value) { mWheel = value; }
	float MouseSym::wheel() const { return mWheel; }
	void MouseSym::reset() {
		mWheel = 0;
		mCurr = mPrev;
		for (int i = 0; i < 3; i++) {
			mLastR[i] = mLastC[i] = mCurr;
			mState[i] = eInput::None;
		}
	}
}
 