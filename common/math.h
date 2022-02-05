#pragma once

#include <cmath>
#include <limits>   

#include "define.h"
#include "stream.h"

#define ELMATH_GLOBAL_SIDE vec3(1, 0, 0)
#define ELMATH_GLOBAL_UP vec3(0, 1, 0)
#define ELMATH_GLOBAL_FRONT vec3(0, 0, 1)

namespace el
{
    inline constexpr double d_pi = 3.14159265358979;
    inline constexpr float pi = 3.14159265359f;
    inline constexpr float half_pi = 1.57079632679f;
    inline constexpr float div_180 = 0.00555555555f;
    inline constexpr float div_360 = 0.00277777777f;
    inline constexpr float div_pi = 0.31830988618f;

    struct color8 { 
        unsigned char r, g, b, a; 
        color8(unsigned char r_, unsigned char g_, unsigned char b_, unsigned char a_) 
        : r(r_), g(g_), b(b_), a(a_) {}
    };

    inline constexpr float to_radians = pi * div_180;
    inline constexpr float to_degrees = 180.0f * div_pi;

    inline float normalizeAngle(float degrees) {
        // reduce the angle  
        degrees = fmod(degrees, 360.f);

        // force it to be the positive remainder, so that 0 <= angle < 360  
        degrees = fmod(degrees + 360.0f, 360.0f);

        // force into the minimum absolute value residue class, so that -180 < angle <= 180  
        if (degrees > 180)
            degrees -= 360;
    }

    inline bool nearly_equal(float a, float b, float epsilon = 128 * FLT_EPSILON, float relth = FLT_MIN) {
        assert(std::numeric_limits<float>::epsilon() <= epsilon);
        assert(epsilon < 1.f);

        if (a == b) return true;

        auto diff = std::abs(a - b);
        auto norm = std::min((std::abs(a) + std::abs(b)), std::numeric_limits<float>::max());
        // or even faster: std::min(std::abs(a + b), std::numeric_limits<float>::max());
        // keeping this commented out until I update figures below
        return diff < std::max(relth, epsilon* norm);
    }


    template<typename T>
    T constexpr sqrtNewtonRaphson(T x, T curr, T prev)
    {
        return curr == prev
            ? curr
            : sqrtNewtonRaphson<T>(x, 0.5f * (curr + x / curr), curr);
    }

    /*
    * Constexpr version of the square root
    * Return value:
    *   - For a finite and non-negative value of "x", returns an approximation for the square root of "x"
    *   - Otherwise, returns NaN
    */
    template<typename T>
    T constexpr sqrt(T x) {
        return x >= 0 && x < std::numeric_limits<T>::infinity()
            ? sqrtNewtonRaphson<T>(x, x, 0)
            : std::numeric_limits<T>::quiet_NaN();
    }

    struct vec3;
    struct vec4;
    struct vec2
    {
        float x, y;

        constexpr vec2() : x(0), y(0) {};
        constexpr vec2(float x_, float y_) : x(x_), y(y_) {};
        constexpr vec2(const vec2& v) : x(v.x), y(v.y) {}
        constexpr vec2(const vec3& v);
        constexpr vec2(const vec4& v);

        constexpr vec2& operator=(const vec2& v) {
            x = v.x; y = v.y;
            return *this;
        }
        constexpr vec2& operator+=(const vec2& v) {
            x += v.x; y += v.y;
            return *this;
        }
        constexpr vec2& operator-=(const vec2& v) {
            x -= v.x; y -= v.y;
            return *this;
        }
        constexpr vec2& operator*=(const float v) {
            x *= v; y *= v;
            return *this;
        }
        constexpr vec2& operator/=(const float v) {
            x /= v; y /= v;
            return *this;
        }
        constexpr vec2 operator+(const vec2& v) const {
            return vec2(x + v.x, y + v.y);
        } 
        constexpr vec2 operator-(const vec2& v) const {
            return vec2(x - v.x, y - v.y);
        }
        constexpr vec2 operator*(const float v) const {
            return vec2(x * v, y * v);
        }
        constexpr vec2 operator/(const float v) const {
            return vec2(x / v, y / v);
        }
        constexpr vec2 operator-() const {
            return vec2(-x, -y);
        }
        constexpr bool isZero() const {
            return x == 0.0f && y == 0.0f;
        }
        constexpr bool sOne() const {
            return x == 1.0f && y == 1.0f;
        }
        constexpr float magnitude() const {
            return sqrt(x * x + y * y);
        }
        constexpr float	magnitudeSquared() const {
            return x * x + y * y;
        }
        constexpr float distanceFrom(const vec2& ent) const {
            float dx = x - ent.x;
            float dy = y - ent.y;
            return sqrt(dx * dx + dy * dy);
        }
        constexpr float squaredDistanceFrom(const vec2& ent) const {
            float dx = x - ent.x;
            float dy = y - ent.y;
            return dx * dx + dy * dy;
        }
        constexpr vec2 normalized() const {
            if (x != 0.0f && y != 0.0f) {
                float mag = 1.0f / sqrt(x * x + y * y);
                return vec2(x * mag, y * mag);
            } else return vec2();
        }
        void normalize() {
            if (x != 0.0f && y != 0.0f) {
                float mag = 1.0f / sqrt(x * x + y * y);
                x *= mag; y *= mag;
            }
        }


        template<typename T>
        void serialize(T& archive) {
            archive(x, y);
        }
        friend stream& operator<<(stream& save, const vec2& vec) {
            save << vec.x << vec.y;
            return save;
        }
        friend stream& operator>>(stream& load, vec2& vec) {
            load >> vec.x >> vec.y;
            return load;
        }
        friend logger& operator<<(logger& log, const vec2& vec) {
            log << "( " << vec.x << ", " << vec.y << " )";
            return log;
        }
    };

    struct vec3
    {
        float x, y, z;

        constexpr vec3() : x(0), y(0), z(0) {};
        constexpr vec3(float common) : x(common), y(common), z(common) {};
        constexpr vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {};
        constexpr vec3(const vec3& v) : x(v.x), y(v.y), z(v.z) {}
        constexpr vec3(const vec2& v, float z_) : x(v.x), y(v.y), z(z_) {};
        constexpr vec3(const vec4& v);

        constexpr vec3& operator=(const vec3& v) {
            x = v.x; y = v.y; z = v.z;
            return *this;
        }
        constexpr vec3& operator+=(const vec3& v) {
            x += v.x; y += v.y; z += v.z;
            return *this;
        }
        constexpr vec3& operator-=(const vec3& v) {
            x -= v.x; y -= v.y; z -= v.z;
            return *this;
        }  
        constexpr vec3& operator*=(const float v) {
            x *= v; y *= v; z *= v;
            return *this;
        }
        constexpr vec3& operator*=(const vec3& v) {
            x *= v.x; y *= v.y; z *= v.z;
            return *this;
        }
        constexpr vec3& operator/=(const float v) {
            x /= v; y /= v; z /= v;
            return *this;
        }
        constexpr vec3 operator+(const vec3& v) const {
            return vec3(x + v.x, y + v.y, z + v.z);
        }
        constexpr vec3 operator-(const vec3& v) const {
            return vec3(x - v.x, y - v.y, z - v.z);
        }
        constexpr vec3 operator*(const float v) const {
            return vec3(x * v, y * v, z * v);
        }
        constexpr vec3 operator*(const vec3& v) const {
            return vec3(x * v.x, y * v.y, z * v.z);
        }
        constexpr vec3 operator^(const vec3& v) const {
            return vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
        }
        constexpr vec3 operator/(const float v) const {
            return vec3(x / v, y / v, z / v);
        }
        constexpr vec3 operator-() const {
            return vec3(-x, -y, -z);
        }
        constexpr bool isZero() const {
            return x == 0.0f && y == 0.0f && z == 0.0f;
        }
        constexpr bool sOne() const {
            return x == 1.0f && y == 1.0f && z == 1.0f;
        }
        constexpr float magnitude() const {
            return sqrt(x * x + y * y + z * z);
        }
        constexpr float	magnitudeSquared() const {
            return x * x + y * y + z * z;
        }
        constexpr float distanceFrom(const vec3& ent) const {
            float dx = x - ent.x;
            float dy = y - ent.y;
            float dz = z - ent.z;
            return sqrt(dx * dx + dy * dy + dz * dz);
        }
        constexpr float squaredDistanceFrom(const vec3& ent) const {
            float dx = x - ent.x;
            float dy = y - ent.y;
            float dz = z - ent.z;
            return dx * dx + dy * dy + dz * dz;
        }
        constexpr vec3 normalized() const {
            if (x != 0.0f || y != 0.0f || z != 0.0f) {
                float mag = 1.0f / sqrt(x * x + y * y + z * z);
                return vec3(x * mag, y * mag, z * mag);
            } else return vec3();
        }
        void normalize() {
            if (x != 0.0f || y != 0.0f || z != 0.0f) {
                float mag = 1.0f / sqrt(x * x + y * y + z * z);
                x *= mag; y *= mag; z *= mag;
            }
        }
        void preventZero() {
            if (x == 0.0f)
                x = 0.00001f;
            if (y == 0.0f)
                y = 0.00001f;
            if (z == 0.0f)
                z = 0.00001f;
        }

        vec4 quaternion() const;

        template<typename T>
        void serialize(T& archive) {
            archive(x, y, z);
        }
        friend stream& operator<<(stream& save, const vec3& vec) {
            save << vec.x << vec.y << vec.z;
            return save;
        }
        friend stream& operator>>(stream& load, vec3& vec) {
            load >> vec.x >> vec.y << vec.z;
            return load;
        }
        friend logger& operator<<(logger& log, const vec3& vec) {
            log << "( " << vec.x << ", " << vec.y << ", " << vec.z << " )";
            return log;
        }
    };

    struct vec4
    {
        float x, y, z, w;

        constexpr vec4() : x(0), y(0), z(0), w(0) {};
        constexpr vec4(float common) : x(common), y(common), z(common), w(common) {};
        constexpr vec4(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {};
        constexpr vec4(const vec4& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
        constexpr vec4(const vec2& v, float z_, float w_) : x(v.x), y(v.y), z(z_), w(w_) {};
        constexpr vec4(const vec3& v, float w_) : x(v.x), y(v.y), z(v.z), w(w_) {};

        constexpr vec4& operator=(const vec4& v) {
            x = v.x; y = v.y; z = v.z; w = v.w;
            return *this;
        }
        constexpr vec4& operator+=(const vec4& v) {
            x += v.x; y += v.y; z += v.z; w += v.w;
            return *this;
        }
        constexpr vec4& operator-=(const vec4& v) {
            x -= v.x; y -= v.y; z -= v.z; w -= v.w;
            return *this;
        }
        constexpr vec4& operator*=(const float v) {
            x *= v; y *= v; z *= v; w *= v;
            return *this;
        }
        constexpr vec4& operator/=(const float v) {
            x /= v; y /= v; z /= v; w /= v;
            return *this;
        }
        constexpr vec4 operator+(const vec4& v) const {
            return vec4(x + v.x, y + v.y, z + v.z, w + v.w);
        }
        constexpr vec4 operator-(const vec4& v) const {
            return vec4(x - v.x, y - v.y, z - v.z, w - v.w);
        }
        constexpr vec4 operator*(const float v) const {
            return vec4(x * v, y * v, z * v, w * v);
        }
        constexpr vec4 operator/(const float v) const {
            return vec4(x / v, y / v, z / v, w / v);
        }
        constexpr vec3 operator-() const {
            return vec4(-x, -y, -z, -w);
        }
        constexpr bool isZero() const {
            return x == 0.0f && y == 0.0f && z == 0.0f && w == 0.0f;
        }
        constexpr bool sOne() const {
            return x == 1.0f && y == 1.0f && z == 1.0f && w == 1.0f;
        }

        template<typename T>
        void serialize(T& archive) {
            archive(x, y, z, w);
        }

        friend stream& operator<<(stream& save, const vec4& vec) {
            save << vec.x << vec.y << vec.z << vec.w;
            return save;
        }
        friend stream& operator>>(stream& load, vec4& vec) {
            load >> vec.x >> vec.y << vec.z << vec.w;
            return load;
        }
        friend logger& operator<<(logger& log, const vec4& vec) {
            log << "( " << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << " )";
            return log;
        }
    };

    inline vec4 vec3::quaternion() const {
        // -radians(val * 0.5f)
        float rx = -x * pi * div_360;
        float ry = -y * pi * div_360;
        float rz = -z * pi * div_360;

        float cx = cos(rx);
        float sx = sin(rx);
        float cy = cos(ry);
        float sy = sin(ry);
        float cz = cos(rz);
        float sz = sin(rz);

        return vec4(
            sx * cy * cz - cx * sy * sz,
            cx * sy * cz + sx * cy * sz,
            cx * cy * sz - sx * sy * cz,
            cx * cy * cz + sx * sy * sz
        );
    }

    inline constexpr vec2::vec2(const vec3& v) : x(v.x), y(v.y) {};
    inline constexpr vec2::vec2(const vec4& v) : x(v.x), y(v.y) {};
    inline constexpr vec3::vec3(const vec4& v) : x(v.x), y(v.y), z(v.z) {};

    struct matrix4x4
    {
        float data[16];

        constexpr matrix4x4() : data{} {
        }
        constexpr matrix4x4(float f) : data{ f, 0.0f, 0.0f, 0.0f,  0.0f, f, 0.0f, 0.0f,  0.0f, 0.0f, f, 0.0f,  0.0f, 0.0f, 0.0f, f } {
        }
        constexpr matrix4x4(float c11, float c12, float c13, float c14, float c21, float c22, float c23, float c24, float c31, float c32, float c33, float c34, float c41, float c42, float c43, float c44) :
            data{ c11, c12, c13, c14, c21, c22, c23, c24, c31, c32, c33, c34, c41, c42, c43, c44 } {}

        void makeIndentity() {
            memcpy(data, s_identity, sizeof(float) * 16);
        }

        // I may change this later
        constexpr matrix4x4 operator*(const matrix4x4& m) const {
            matrix4x4 res;
            auto& r = res.data;
            sizet it = 0;
            for (sizet i = 0; i < 4; i++) {
                for (sizet j = 0; j < 4; j++) {
                    r[it] =
                        data[j     ] * m.data[i * 4    ] +
                        data[j + 4 ] * m.data[i * 4 + 1] +
                        data[j + 8 ] * m.data[i * 4 + 2] +
                        data[j + 12] * m.data[i * 4 + 3];
                    it++;
                }
            }
            return res;
        }

        template<typename T>
        void serialize(T& archive) {
            for (sizet i = 0; i < 16; i++) // optimize?..
                archive(data[i]);
        }
    private:
        static const constexpr float s_identity[16] = { 1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f, 1.0f };
    };
}