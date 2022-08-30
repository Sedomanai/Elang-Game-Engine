#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h> // in qt, glew must be compiled before anything else

#include "elang_library_builder.h"
#include "tools/controls.h"
#include "apparatus/asset_loader.h"

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