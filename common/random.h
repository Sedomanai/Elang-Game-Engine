/*****************************************************************//**
 * @file   random.h
 * @brief  Random related 
 * 
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/

#pragma once

#include <random>

namespace  el
{
	/** HOW TO USE RNG (because I never remember this)
	* rng r
	* r.seed(SEED()());
	* (i/r/ui)rand rand(min, max)
	* lv = (i/r/ui)rand(r)
	*/
	using rng = std::mt19937;
	using irand = std::uniform_int_distribution<int>;
	using uirand = std::uniform_int_distribution<unsigned int>;
	using rrand = std::uniform_real_distribution<float>;
	using SEED = std::random_device;
}