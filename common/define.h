/*******************************************************************
 * @file   define.h
 * @brief  Elang common definitiions, macros, and using operators
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
#include <type_traits>

using sizet = size_t;
using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;
using uint = unsigned int;
using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

/**
 * Explicit class copy constructor and assignment operator macro for specific cases
 * Does not include destructor
 */
#define EL_USING_BASE_CLASS(t, base) \
using base::base; \
t(const base& src) : base::base(src) {} \
t(base&& src) : base::base(src) {} \
t& operator=(const base& src) { base::operator=(src); return *this; } \
t& operator=(base&& src) { base::operator=(src); return *this; }

#include <iostream>
namespace el
{
	using logger = std::ostream;
}
#ifdef _IOSTREAM_
	using std::cout;
	using std::cin;
	using std::endl;
#endif

#define NOMINMAX
