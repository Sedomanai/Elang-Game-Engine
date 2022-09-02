/*****************************************************************//**
 * @file   keysym.h
 * @brief  Basic operations for all kinds of key simulations
 *
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
#include "define.h"
#include "enums.h"
#include "../elang_builder.h"

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
	 * @brief Refined button input, useful for event based loops such as game loops
	 * @brief Can also be used in GUI app for more refined keyboard input
	 * @brief eKeyCode is based on modified version of SDLK(ey)
	 * @brief * update() must register at the END of a game loop along with MouseSym update
	 */
	struct ELANG_DLL KeySym
	{
		/**
		 * Translate from SDL Keycode to Elang Keycode.
		 *
		 * @param key : SDL keycode
		 * @return Elang keycode
		 */
		sizet translateFromSDLKeyCode(sizet key);

		/**
		 * Current eInput state of key in Elang Keycode. Refer to <common/enum.h> for more on eInput state
		 *
		 * @param key : Elang keycode
		 * @return Current state of key in eInput by keycode
		 */
		eInput state(eKeyCode key);

		/**
		 * Current eInput state of key in Elang Keycode. Refer to <common/enum.h> for more on eInput state
		 *
		 * @param key : Elang keycode
		 * @return Current state of key, by keycode
		 */
		eInput state(sizet key);

		/**
		 * Fire this in any event that can transfer key press signal
		 * @brief Updates all None state to Once state
		 *
		 * @param key : Must be translated to eKeyCode
		 */
		void onPress(sizet key);

		/**
		 * Fire this in any event that can transfer key release signal.
		 * Updates all Hold state to Lift
		 *
		 * @param key : Must be translated to eKeyCode
		 */
		void onRelease(sizet key);

		/**
		 * @brief Fire this every frame or any interval that should detect key signals. Updates all key states
		 * @brief Detects presses and releases from previous frame. If pressed (Once) update to Hold state.
		 * If released (Lift) update to None state
		 *
		 * @brief * For more info on when exactly to invoke this method, refer to the KeySym comment intellisense.
		 */
		void updateKeys();

		/**
		 * @brief Completely reset the key simulator. This means clearing all 256 key states
		 * @brief ****
		 * @brief *** WARNING: This may be absolutely necessary when transitioning between mutliple mouse event loops, for example between two windows.
		 * It really depends on the platform, but generally call this between every window change. Otherwise may result in exceptions and/or weird behavior.
		 * @brief Alternatively consider creating multiple MouseSym for every window instead of using a single global controller.
		 */
		void reset();

	private:
		eInput mKeys[256];
		sizet mUpdates[ELANG_TOTAL_KEY_PRESS_COUNT];
		sizet mUpdateNum;

		void registerUpdate(sizet key);
	};

	extern ELANG_DLL KeySym gKey;
}