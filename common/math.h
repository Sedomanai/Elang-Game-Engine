/*****************************************************************//**
 * @file   math.h
 * @brief  Math collection
 *
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
#include "define.h"
#include "../elang_builder.h"

namespace el
{
	namespace el_math
	{
		inline constexpr double d_pi = 3.14159265358979;
		inline constexpr float pi = 3.14159265359f;
		inline constexpr float half_pi = 1.57079632679f;
		inline constexpr float div_180 = 0.00555555555f;
		inline constexpr float div_360 = 0.00277777777f;
		inline constexpr float div_pi = 0.31830988618f;
		inline constexpr float to_radians = pi * div_180;
		inline constexpr float to_degrees = 180.0f * div_pi;

		/**
		 * @brief Normalizes angle in degrees so that it is >= 0 and <= 360.
		 */
		extern ELANG_DLL float normalizeAngle(float degrees);
		/**
		 * @brief For more loose float comparions.
		 */
		extern ELANG_DLL bool nearly_equal(float a, float b, float epsilon = 128 * FLT_EPSILON, float relth = FLT_MIN);

		template<typename T>
		T constexpr sqrtNewtonRaphson(T x, T curr, T prev) {
			return curr == prev
				? curr
				: sqrtNewtonRaphson<T>(x, 0.5f * (curr + x / curr), curr);
		}

		/**
		 * @brief Constexpr version of the square root.
		 *
		 * @return For a finite and non-negative value of x, returns an approximation of the square root of x, otherwise, returns NaN
		 */
		template<typename T>
		T constexpr sqrt(T x) {
			return x >= 0 && x < std::numeric_limits<T>::infinity()
				? sqrtNewtonRaphson<T>(x, x, 0)
				: std::numeric_limits<T>::quiet_NaN();
		}
	}
}