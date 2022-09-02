/*****************************************************************//**
 * @file   math.h
 * @brief  Math collection
 * 
 * @author Sedomanai
 * @date   September 2022
 *********************************************************************/
#pragma once
#include "../elang_builder.h"

namespace el
{
    struct vec3;
    struct vec4;
    struct ELANG_DLL vec2
    {
        float x, y;

        constexpr vec2();
        constexpr vec2(float x_, float y_);
        constexpr vec2(const vec2& v);
        constexpr vec2(const vec3& v);
        constexpr vec2(const vec4& v);

        constexpr vec2& operator=(const vec2& v);
        constexpr vec2& operator+=(const vec2& v);
        constexpr vec2& operator-=(const vec2& v);
        constexpr bool operator==(const vec2& v);
        constexpr bool operator!=(const vec2& v);
        constexpr vec2& operator*=(const float v);
        constexpr vec2& operator/=(const float v);
        constexpr vec2 operator+(const vec2& v) const;
        constexpr vec2 operator-(const vec2& v) const;
        constexpr vec2 operator*(const float v) const;
        constexpr vec2 operator/(const float v) const;
        constexpr vec2 operator-() const;
        constexpr bool isZero() const;
        constexpr bool isOne() const;
        constexpr float magnitude() const;
        constexpr float	magnitudeSquared() const;
        constexpr float distanceFrom(const vec2& ent) const;

        /**
         * distanceFrom without square root. Great for distance comparisons without in need of actual distance value.
         *
         * @param ent- Distance from this vector point to this vector
         * @return Squared value of distance in float
         */
        constexpr float squaredDistanceFrom(const vec2& ent) const;

        /**
         * Creates new vector with normalized value.
         *
         * @return Normalized new vector
         */
        constexpr vec2 normalized() const;

        /**
         * Normalizes this vector. Value before normalization is lost.
         */
        void normalize();


        template<typename T>
        void serialize(T& archive) {
            archive(x, y);
        }
    };
}