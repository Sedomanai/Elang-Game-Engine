#pragma once

#include <random>

namespace  el
{
	/** 랜덤 사용방법
	* rng r
	* r.seed(SEED()());
	* irand/rrand rand(min, max)
	* lv = rand(r)
	*/
	using rng = std::mt19937;
	using irand = std::uniform_int_distribution<int>;
	using uirand = std::uniform_int_distribution<uint>;
	using rrand = std::uniform_real_distribution<float>;
	using SEED = std::random_device;
}