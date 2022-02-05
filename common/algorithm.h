#pragma once

#include <type_traits>

#include "define.h"

struct na
{
    typedef na type;
    enum { value = 0 };
};


template<typename T = na>
struct identity
{
    typedef T type;
};

// minmax
template<typename T, typename U>
constexpr typename std::common_type_t<T, U> const& min(const T& left, const U& right) {
    return (left < right) ? left : right;
}
template<typename T, typename U>
constexpr typename std::common_type_t<T, U> const& max(const T& left, const U& right) {
    return (left > right) ? left : right;
}

// clamp
template<typename T>
constexpr T const& clamp(const T& val, const std::_Identity_t<T>& low, const std::_Identity_t<T>& high) {
	return (val < low) ? low : (val > high) ? high : val;
}