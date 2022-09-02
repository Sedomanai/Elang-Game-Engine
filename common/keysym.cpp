#include <elpch.h>
#include "keysym.h"

namespace el
{
	sizet KeySym::translateFromSDLKeyCode(sizet key) {
		if (key > 1000000000) {
			key -= 1073741681;
			if (key > 300)
				key -= 200;
		} return key;
	}

	/**
		* Current eInput state of key in Elang Keycode. Refer to <common/enum.h> for more on eInput state
		* 
		* @param key : Elang keycode
		* @return Current state of key in eInput by keycode
		*/
	eInput KeySym::state(eKeyCode key) {
		return state(sizet(key));
	}


	/**
		* Current eInput state of key in Elang Keycode. Refer to <common/enum.h> for more on eInput state
		*
		* @param key : Elang keycode
		* @return Current state of key, by keycode
		*/
	eInput KeySym::state(sizet key) {
		if (0 < key && key < 256) {
			return (mKeys[key] == eInput::Flap) ? eInput::Snap : mKeys[key];
		} else return eInput::None;
	}

	/**
		* Fire this in any event that can transfer key press signal
		* @brief Updates all None state to Once state
		* 
		* @param key : Must be translated to eKeyCode
		*/
	void KeySym::onPress(sizet key) {
		switch (mKeys[key]) {
		case eInput::None:
			mKeys[key] = eInput::Once;
			registerUpdate(key);
			break;
		// Ignore Snap or Flap for now, that is a safety measure for key update loops slower than say 10fps.
		case eInput::Lift:
		case eInput::Snap:
			mKeys[key] = eInput::Flap;
			registerUpdate(key);
			break;
		}
	}

	/**
		* Fire this in any event that can transfer key release signal.
		* Updates all Hold state to Lift
		*
		* @param key : Must be translated to eKeyCode
		*/
	void KeySym::onRelease(sizet key) {
		switch (mKeys[key]) {
		case eInput::Hold:
			mKeys[key] = eInput::Lift;
			registerUpdate(key);
			break;
		// Ignore Snap or Flap for now, that is a safety measure for key update loops slower than say 10fps.
		case eInput::Once:
		case eInput::Flap:
			mKeys[key] = eInput::Snap;
			registerUpdate(key);
			break;
		}
	}

	/**
		* @brief Fire this every frame or any interval that should detect key signals. Updates all key states
		* @brief Detects presses and releases from previous frame. If pressed (Once) update to Hold state. 
		* If released (Lift) update to None state
		* 
		* @brief * For more info on when exactly to invoke this method, refer to the KeySym comment intellisense. 
		*/
	void KeySym::updateKeys() {
		for (unsigned int i = 0; i < mUpdateNum; i++) {
			auto j = mUpdates[i];
			// Ignore Snap or Flap for now, that is a safety measure for key update loops slower than say 10fps.
			switch (mKeys[j]) {
			case eInput::Once:
			case eInput::Flap:
				mKeys[j] = eInput::Hold;
				break;
			case eInput::Lift:
			case eInput::Snap:
				mKeys[j] = eInput::None;
				break;
			}
		} mUpdateNum = 0;
	}

	/**
		* @brief Completely reset the key simulator. This means clearing all 256 key states
		* @brief ****
		* @brief *** WARNING: This may be absolutely necessary when transitioning between mutliple mouse event loops, for example between two windows.
		* It really depends on the platform, but generally call this between every window change. Otherwise may result in exceptions and/or weird behavior.
		* @brief Alternatively consider creating multiple MouseSym for every window instead of using a single global controller.
		*/
	void KeySym::reset() {
		for (int i = 0; i < 256; i++) {
			mKeys[i] = eInput::None;
		} mUpdateNum = 0;
	}

	void KeySym::registerUpdate(sizet key) {
		if (mUpdateNum < ELANG_TOTAL_KEY_PRESS_COUNT) {
			mUpdates[mUpdateNum] = key;
			mUpdateNum++;
		}
	}
}