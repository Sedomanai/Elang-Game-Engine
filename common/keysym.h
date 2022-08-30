/*****************************************************************//**
 * @file   keysym.h
 * @brief  Basic operations for all kinds of key simulations. 
 * 
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/

#pragma once

#include "../elang_library_builder.h"
#include "../common/enums.h"

namespace el
{
	// This keycode was created centered around SDL keycode
	enum class eKeyCode
	{
		Backspace = 8,
		Tab,

		Return = 13,
		Escape = 27,

		Space = 32,

		Apostrophe = 39,

		Comma = 44,
		Minus,
		Period,
		Slash,

		N0 = 48,
		N1,
		N2,
		N3,
		N4,
		N5,
		N6,
		N7,
		N8,
		N9,

		Semicolon = 59,
		Equals = 61,

		LBracket = 91,
		BackSlahs,
		RBracket,

		Acute = 96,
		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,

		Delete = 127,

		LCtrl = 167,
		LShift,
		LAlt,
		LWin,
		RCtrl,
		RShift,
		RAlt,
		RWin,

		Caps = 200,
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		Print_Screen,
		Lock,
		Break,
		Insert,
		Home,
		Pgup,
		Void,
		End,
		Pgdn,

		Right,
		Left,
		Down,
		Up,

		NumLock,
		NumSlash,
		NumAsterisk,
		NumMinus,
		NumPlus,
		NumEnter,
		Num1,
		Num2,
		Num3,
		Num4,
		Num5,
		Num6,
		Num7,
		Num8,
		Num9,
		Num0,
		NumPeriod,

		Context = 244
	};

	#define ELANG_TOTAL_KEY_PRESS_COUNT 32
	
	/**
	 * @brief Refined button input, useful for event based loops such as game loops. Can also be used in GUI app for more refined keyboard input.
	 * eKeyCode is based on modified version of SDLK(ey). update() must register at the END of a game loop along with MouseSym update.
	 */
	struct KeySym
	{
		/**
		 * Translate from SDL Keycode to Elang Keycode.
		 * 
		 * @param SDL key
		 * @return Elang key
		 */
		sizet translateFromSDLKeyCode(sizet key) {
			if (key > 1000000000) {
				key -= 1073741681;
				if (key > 300)
					key -= 200;
			} return key;
		}

		//sizet translateFromQtKeyCode(sizet key) {
		//	if (key > 1000000000) {
		//		key -= 1073741681;
		//		if (key > 300)
		//			key -= 200;
		//	} return key;
		//}

		/**
		 * Current eInput state of key in Elang Keycode. Refer to <common/enum.h> for more on eInput state.
		 * 
		 * @param key Elang keycode
		 * @return Current state of key
		 */
		eInput state(eKeyCode key) {
			return state(sizet(key));
		}


		/**
		 * Current eInput state of key in Elang Keycode. Refer to <common/enum.h> for more on eInput state.
		 *
		 * @param key- Elang keycode
		 * @return Current state of key
		 */
		eInput state(sizet key) {
			if (0 < key && key < 256) {
				return (mKeys[key] == eInput::Flap) ? eInput::Snap : mKeys[key];
			} else return eInput::None;
		}

		/**
		 * Fire this in any event that can transfer key press signal.
		 * Updates all None state to Once state
		 * 
		 * @param key- Must be translated to eKeyCode
		 */
		void onPress(sizet key) {
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
		 * @param key- Must be translated to eKeyCode
		 */
		void onRelease(sizet key) {
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
		 * Fire this every frame or any interval that should detect key signals. Updates all key states.
		 * Detects presses and releases from previous frame. If pressed (Once state) update to Hold state.
		 * If released (Lift state) update to Lift state.
		 * 
		 * For more info on when exactly to invoke this method, refer to the KeySym comment intellisense. 
		 */
		void updateKeys() {
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
		 * Resets all 256 key states.
		 */
		void reset() {
			for (int i = 0; i < 256; i++) {
				mKeys[i] = eInput::None;
			} mUpdateNum = 0;
		}

	private:
		eInput mKeys[256];
		sizet mUpdates[ELANG_TOTAL_KEY_PRESS_COUNT];
		sizet mUpdateNum;
		
		void registerUpdate(sizet key) {
			if (mUpdateNum < ELANG_TOTAL_KEY_PRESS_COUNT) {
				mUpdates[mUpdateNum] = key;
				mUpdateNum++;
			}
		}
	};

	extern ELANG_DLL KeySym gKey;
}