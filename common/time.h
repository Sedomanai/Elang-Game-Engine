#pragma once

#include <chrono>

#include "define.h"

#define chrono_now std::chrono::high_resolution_clock::now()
#define chrono_nnsec std::chrono::duration_cast<std::chrono::nanoseconds>
#define chrono_msec std::chrono::duration_cast<std::chrono::milliseconds>
#define chrono_sec std::chrono::duration_cast<std::chrono::seconds>
#define chrono_timepoint std::chrono::time_point<std::chrono::steady_clock>

namespace  el
{
	//프로파일 (함수 객체, 나노초 단위로 경과시간 리턴)
	//receive functor as parameter, return elapsed time by nano-seconds
	template <typename F>
	inline long long profile(sizet times, const F& func) {
		auto t1 = chrono_now;
		for (sizet i = 0; i < times; i++)
			func();
		auto t2 = chrono_now;
		auto ret = chrono_nnsec(t2 - t1).count();
		return ret;
	}

	//시간차 루프 speed = tick/sec
	template<typename F>
	void loop(F functor, float speed = 1.0f, sizet loopCount = 10000000) {
		sizet tick = 0;
		auto begin = chrono_now;
		long long it = 0;
		while (true) {
			auto duration = chrono_msec(chrono_now - begin).count();
			if (it < duration - (long long)(1000 / speed)) {
				it = duration;
				functor();
				tick++;
				if (tick > loopCount - 1)
					break;
			}
		}
	}
}