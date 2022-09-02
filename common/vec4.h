/*****************************************************************//**
 * @file   vec4.h
 * @brief  vector4
 * 
 * @author Sedomanai
 * @date   September 2022
 *********************************************************************/
#pragma once
#include "../elang_builder.h"

namespace el
{
    struct vec2;
    struct vec3;
    struct ELANG_DLL vec4
    {
        float x, y, z, w;

        constexpr vec4();
        constexpr vec4(float common);
        constexpr vec4(float x_, float y_, float z_, float w_);
        constexpr vec4(const vec4& v);
        constexpr vec4(const vec2& v, float z_, float w_);
        constexpr vec4(const vec3& v, float w_);

        constexpr vec4& operator=(const vec4& v);
        constexpr bool operator==(const vec4& v);
        constexpr bool operator!=(const vec4& v);
        constexpr vec4& operator+=(const vec4& v);
        constexpr vec4& operator-=(const vec4& v);
        constexpr vec4& operator*=(const float v);
        constexpr vec4& operator/=(const float v);
        constexpr vec4 operator+(const vec4& v) const;
        constexpr vec4 operator-(const vec4& v) const;
        constexpr vec4 operator*(const float v) const;
        constexpr vec4 operator/(const float v) const;
        constexpr vec3 operator-() const;
        constexpr bool isZero() const;
        constexpr bool isOne() const;

        /**
         * For Cereal.
         *
         * @param Cereal InArchive
         */
        template<typename T>
        void serialize(T& archive) {
            archive(x, y, z, w);
        }
    };
}