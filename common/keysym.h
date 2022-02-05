#pragma once

#include "../elang_library_builder.h"
#include "../common/enums.h"

namespace el
{
	enum class eKeyCode
	{
		BACKSPACE = 8,
		TAB,

		RETURN = 13,
		ESCAPE = 27,

		SPACE = 32,

		APOSTROPHE = 39,

		COMMA = 44,
		MINUS,
		PERIOD,
		SLASH,

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

		SEMICOLON = 59,
		EQUALS = 61,

		LBRACKET = 91,
		BACKSLASH,
		RBRACKET,

		ACUTE = 96,
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

		DELETE_ = 127,

		LCTRL = 167,
		LSHIFT,
		LALT,
		LWIN,
		RCTRL,
		RSHIFT,
		RALT,
		RWIN,

		CAPS = 200,
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
		PRINT_SCREEN,
		LOCK,
		BREAK,
		INSERT,
		HOME,
		PGUP,
		VOID_,
		END,
		PGDN,

		RIGHT,
		LEFT,
		DOWN,
		UP,

		NUM_LOCK,
		NUM_SLASH,
		NUM_ASTERISK,
		NUM_MINUS,
		NUM_PLUS,
		NUM_ENTER,
		NUM1,
		NUM2,
		NUM3,
		NUM4,
		NUM5,
		NUM6,
		NUM7,
		NUM8,
		NUM9,
		NUM0,
		NUM_PERIOD,

		CONTEXT = 244
	};

	#define ELANG_TOTAL_KEY_PRESS_COUNT 32
	
	//=======================================================================================
	// Refined button input, useful for event based loop such as game loops.
	// Can also be used in GUI app for more refined keyboard input.
	// eKeyCode based on modified version of SDLK(ey)
	// update() must register at the END of a game loop along with MouseSym update.
	//=======================================================================================
	struct KeySym
	{
		sizet translateFromSDLKeyCode(sizet key) {
			if (key > 1000000000) {
				key -= 1073741681;
				if (key > 300)
					key -= 200;
			} return key;
		}

		sizet translateFromQtKeyCode(sizet key) {
			if (key > 1000000000) {
				key -= 1073741681;
				if (key > 300)
					key -= 200;
			} return key;
		}

		eInput state(eKeyCode key) {
			return state(sizet(key));
		}

		eInput state(sizet key) {
			if (0 < key && key < 256) {
				return (mKeys[key] == eInput::FLAP) ? eInput::SNAP : mKeys[key];
			} else return eInput::NONE;
		}

		void onPress(sizet key) {
			switch (mKeys[key]) {
			case eInput::NONE:
				mKeys[key] = eInput::ONCE;
				registerUpdate(key);
				break;
			case eInput::LIFT:
			case eInput::SNAP:
				mKeys[key] = eInput::FLAP;
				registerUpdate(key);
				break;
			}
		}

		void onRelease(sizet key) {
			switch (mKeys[key]) {
			case eInput::HOLD:
				mKeys[key] = eInput::LIFT;
				registerUpdate(key);
				break;
			case eInput::ONCE:
			case eInput::FLAP:
				mKeys[key] = eInput::SNAP;
				registerUpdate(key);
				break;
			}
		}

		void updateKeys() {
			for (unsigned int i = 0; i < mUpdateNum; i++) {
				auto j = mUpdates[i];
				switch (mKeys[j]) {
				case eInput::ONCE:
				case eInput::FLAP:
					mKeys[j] = eInput::HOLD;
					break;
				case eInput::LIFT:
				case eInput::SNAP:
					mKeys[j] = eInput::NONE;
					break;
				}
			} mUpdateNum = 0;
		}

		void reset() {
			for (int i = 0; i < 256; i++) {
				mKeys[i] = eInput::NONE;
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