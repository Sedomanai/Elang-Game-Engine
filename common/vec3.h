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
    struct vec2;
    struct vec4;
    struct ELANG_DLL vec3
    {
        float x, y, z;

        constexpr vec3();
        constexpr vec3(float common);
        constexpr vec3(float x_, float y_, float z_);
        constexpr vec3(const vec3& v);
        constexpr vec3(const vec2& v, float z_);
        constexpr vec3(const vec4& v);

        constexpr vec3& operator=(const vec3& v);
        constexpr vec3& operator+=(const vec3& v);
        constexpr vec3& operator-=(const vec3& v);
        constexpr vec3& operator*=(const float v);
        constexpr bool operator==(const vec3& v);
        constexpr bool operator!=(const vec3& v);
        constexpr vec3& operator*=(const vec3& v);
        constexpr vec3& operator/=(const float v);
        constexpr vec3 operator+(const vec3& v) const;
        constexpr vec3 operator-(const vec3& v) const;
        constexpr vec3 operator*(const float v) const;
        constexpr vec3 operator*(const vec3& v) const;
        constexpr vec3 operator^(const vec3& v) const;
        constexpr vec3 operator/(const float v) const;
        constexpr vec3 operator-() const;
        constexpr bool isZero() const;
        constexpr bool isOne() const;
        constexpr float magnitude() const;
        constexpr float	magnitudeSquared() const;
        constexpr float distanceFrom(const vec3& ent) const;

        /**
         * distanceFrom method without square root. Great for distance comparisons without in need of actual distance value.
         *
         * @param ent- Distance from this vector point to this vector
         * @return Squared value of distance in float
         */
        constexpr float squaredDistanceFrom(const vec3& ent) const;

        /**
         * Creates new vector with normalized value.
         *
         * @return Normalized new vector
         */
        constexpr vec3 normalized() const;

        /**
         * @brief Normalizes this vector. Value before normalization is lost.
         */
        void normalize();

        /**
         * @brief If any of the axis is 0, increment by a very small amount to make it non-zero 
         */
        void preventZero();

        /**
         * @return Quaternion of this vector in vec4
         */
        vec4 quaternion() const;

        /**
         * For Cereal.
         *
         * @param Cereal InArchive
         */
        template<typename T>
        void serialize(T& archive) {
            archive(x, y, z);
        }
    };

    #define ELMATH_GLOBAL_SIDE vec3(1, 0, 0)
    #define ELMATH_GLOBAL_UP vec3(0, 1, 0)
    #define ELMATH_GLOBAL_FRONT vec3(0, 0, 1)
}