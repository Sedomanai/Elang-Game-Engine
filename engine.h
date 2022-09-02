#pragma once
#include <SDL/SDL.h>
#include "common/define.h"
#include "elang_builder.h"

namespace el
{
	struct ELANG_DLL Engine
	{
		void run();

		static uint secondsToFPS(float seconds) {
			return (uint)round(sFPS * seconds);
		}

	protected:
		virtual void start() {};
		virtual void input() {};
		virtual void update();
		virtual void paint();

		virtual void load();
		virtual void loop();
		virtual void render();
		virtual void resize();

		SDL_Window* mWindow;
		SDL_GLContext mContext;
		int mX, mY, mW, mH;
		bool debugging;

		static const uint sFPS;
	};
}