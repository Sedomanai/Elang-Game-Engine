/*****************************************************************//**
 * @file   algorithm.h
 * @brief  A couple or more simple algorithms that is not so readily available in standard libraries
 *
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
#include <algorithm>
#include <type_traits>

 /**
 * Template struct for na. (No longer remember the mechanics, I only know that it's used to receive all number types)
  */
struct na
{
	typedef na type;
	enum { value = 0 };
};

/**
 * Template struct for identity. (No longer remember the mechanics, I only know that it's used to receive all number types)
 */
template<typename T = na>
struct identity
{
	typedef T type;
};

/**
 * Min function
 *
 * @param left : Value
 * @param right : Other value
 * @return Smaller value of the two
 */
template<typename T, typename U>
constexpr typename std::common_type_t<T, U> const& min(const T& left, const U& right) {
	return (left < right) ? left : right;
}

/**
 * Max function
 *
 * @param left : Value
 * @param right : Other value
 * @return Larger value of the two
 */
template<typename T, typename U>
constexpr typename std::common_type_t<T, U> const& max(const T& left, const U& right) {
	return (left > right) ? left : right;
}

/**
 * Clamp function
 *
 * @param val : Value in question
 * @param low : Low limit
 * @param high : High limit
 * @return Clamped value
 */
template<typename T>
constexpr T const& clamp(const T& val, const std::_Identity_t<T>& low, const std::_Identity_t<T>& high) {
	return (val < low) ? low : (val > high) ? high : val;
}