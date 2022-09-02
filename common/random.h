/*****************************************************************//**
 * @file   random.h
 * @brief  Random related 
 * 
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
#include <random>

namespace el
{
	/** 
	 * @brief ** HOW TO USE RNG (because I never remember this)
	 * @brief -------------------------------------------------------------
	 * @brief rng r
	 * @brief r.seed(SEED()());
	 * @brief (i/r/ui)rand rand(min, max)
	 * @brief lv = (i/r/ui)rand(r)
	*/
	using rng = std::mt19937;
	using irand = std::uniform_int_distribution<int>;
	using uirand = std::uniform_int_distribution<unsigned int>;
	using rrand = std::uniform_real_distribution<float>;
	using SEED = std::random_device;
}