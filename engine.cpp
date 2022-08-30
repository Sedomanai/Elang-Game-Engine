#include "engine.h"
#include "common/time.h"
#include "common/math.h"
//#include "elements/sprite.h"
//#include "elements/button.h"

namespace el 
{
	const uint Engine::sFPS = 60;

	void Engine::run() {
		SDL_Init(SDL_INIT_EVERYTHING);
		SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");

		mWindow = SDL_CreateWindow("Ilang.exe", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			800, 600, SDL_WINDOW_OPENGL);
		if (!mWindow)
			cout << "windowError" << endl;

		mContext = SDL_GL_CreateContext(mWindow);
		if (!mContext)
			std::cout << "contextError" << std::endl;

		if (glewInit() != GLEW_OK) {
			std::cout << "glewError" << std::endl;
			return;
		}

		debugging = false;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
		SDL_GL_SwapWindow(mWindow); 

		Painter::sCreateNullTexture();

		resize();
		load();
		start();
		loop();
	}

	void Engine::load() {}
	void Engine::update() {}
	void Engine::paint() {}
	void Engine::loop() {
		bool running = true;
		SDL_Event e;
		const long long nns_per_update = long long(1000000000.0f / sFPS);
		auto previous = chrono_now;
		long long lag = 0;
		while (running) {
			while (SDL_PollEvent(&e)) {
				// all window events here
				switch (e.window.event) {
				case SDL_WINDOWEVENT_CLOSE:
					running = false;
					break;
				case SDL_WINDOWEVENT_SIZE_CHANGED:
					resize();
					break;
				case SDL_WINDOWEVENT_MOVED:
					resize();
					break;
				}

				vec2 position;
				// all input events here
				switch (e.type) {
				case SDL_KEYUP:
					gKey.onRelease(gKey.translateFromSDLKeyCode(e.key.keysym.sym));
					break;
				case SDL_KEYDOWN:
					gKey.onPress(gKey.translateFromSDLKeyCode(e.key.keysym.sym));
					break;
				case SDL_MOUSEBUTTONDOWN:
					position = vec2(e.button.x - mW / 2.0f, -e.button.y + mH / 2.0f);
					switch (e.button.button) {
					case SDL_BUTTON_LEFT:
						gMouse.onPress(position, MouseSym::Left);
						break;
					case SDL_BUTTON_MIDDLE:
						gMouse.onPress(position, MouseSym::Middle);
						break;
					case SDL_BUTTON_RIGHT:
						gMouse.onPress(position, MouseSym::Right);
						break;
					}
					break;
				case SDL_MOUSEBUTTONUP:
					position = vec2(e.button.x - mW / 2.0f, -e.button.y + mH / 2.0f);
					switch (e.button.button) {
					case SDL_BUTTON_LEFT:
						gMouse.onRelease(position, MouseSym::Left);
						break;
					case SDL_BUTTON_MIDDLE:
						gMouse.onRelease(position, MouseSym::Middle);
						break;
					case SDL_BUTTON_RIGHT:
						gMouse.onRelease(position, MouseSym::Right);
						break;
					}
					break;
				case SDL_MOUSEWHEEL:
					gMouse.updateWheel((float)e.wheel.y);
					break;
				}
			}

			auto current = chrono_now;
			auto elapsed = current - previous;
			previous = current;
			lag += chrono_nnsec(elapsed).count();

			bool first = true;
			while (lag >= nns_per_update) {
				lag -= nns_per_update;
				input();
				if ((gKey.state(eKeyCode::LCtrl) == eInput::Hold) && 
					(gKey.state(eKeyCode::LShift) == eInput::Hold) &&
					(gKey.state(eKeyCode::D) == eInput::Once)) 
				{
					debugging = !debugging;
				}
				update();

				int mx, my;
				SDL_GetMouseState(&mx, &my);
				vec2 position = vec2(mx - mW / 2.0f, -my + mH / 2.0f);
				gKey.updateKeys();
				gMouse.updateKeys(position);
				gMouse.updateWheel(0.0f);
			}
			render();
		};
	}


	void Engine::render() {
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		paint();
		SDL_GL_SwapWindow(mWindow);
	}

	void Engine::resize() {
		SDL_GetWindowPosition(mWindow, &mX, &mY);
		SDL_GetWindowSize(mWindow, &mW, &mH);
		glViewport(0, 0, mW, mH);
		updateViewport(-float(mW) / 2.0f, float(mW) / 2.0f, -float(mH) / 2.0f, float(mH) / 2.0f);
	}

}