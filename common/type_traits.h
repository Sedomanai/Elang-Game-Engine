/*****************************************************************//**
 * @file   type_traits.h
 * @brief  Commonly used type traits. May merge with algorithm? Not decided
 * 
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/

#pragma once
#include <type_traits>

template<typename T>
struct is_num { static constexpr bool value = (std::is_integral_v<T> || std::is_floating_point_v<T>); };
template<typename T>
struct is_char { static constexpr bool value = std::is_same_v<T, char>; };

template <class T>
constexpr bool is_num_v = is_char<T>::value;
template <class T>
constexpr bool is_char_v = is_char<T>::value;