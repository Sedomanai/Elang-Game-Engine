#include <elpch.h>
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "math.h"

namespace el
{
    constexpr vec4::vec4() : x(0), y(0), z(0), w(0) {};
    constexpr vec4::vec4(float common) : x(common), y(common), z(common), w(common) {};
    constexpr vec4::vec4(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {};
    constexpr vec4::vec4(const vec4& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
    constexpr vec4::vec4(const vec2& v, float z_, float w_) : x(v.x), y(v.y), z(z_), w(w_) {};
    constexpr vec4::vec4(const vec3& v, float w_) : x(v.x), y(v.y), z(v.z), w(w_) {};

    constexpr vec4& vec4::operator=(const vec4& v) {
        x = v.x; y = v.y; z = v.z; w = v.w;
        return *this;
    }
    constexpr bool vec4::operator==(const vec4& v) {
        return x == v.x && y == v.y && z == v.z && w == v.w;
    }
    constexpr bool vec4::operator!=(const vec4& v) {
        return x != v.x || y != v.y || z != v.z || w == v.w;
    }
    constexpr vec4& vec4::operator+=(const vec4& v) {
        x += v.x; y += v.y; z += v.z; w += v.w;
        return *this;
    }
    constexpr vec4& vec4::operator-=(const vec4& v) {
        x -= v.x; y -= v.y; z -= v.z; w -= v.w;
        return *this;
    }
    constexpr vec4& vec4::operator*=(const float v) {
        x *= v; y *= v; z *= v; w *= v;
        return *this;
    }
    constexpr vec4& vec4::operator/=(const float v) {
        x /= v; y /= v; z /= v; w /= v;
        return *this;
    }
    constexpr vec4 vec4::operator+(const vec4& v) const {
        return vec4(x + v.x, y + v.y, z + v.z, w + v.w);
    }
    constexpr vec4 vec4::operator-(const vec4& v) const {
        return vec4(x - v.x, y - v.y, z - v.z, w - v.w);
    }
    constexpr vec4 vec4::operator*(const float v) const {
        return vec4(x * v, y * v, z * v, w * v);
    }
    constexpr vec4 vec4::operator/(const float v) const {
        return vec4(x / v, y / v, z / v, w / v);
    }
    constexpr vec3 vec4::operator-() const {
        return vec4(-x, -y, -z, -w);
    }
    constexpr bool vec4::isZero() const {
        return x == 0.0f && y == 0.0f && z == 0.0f && w == 0.0f;
    }
    constexpr bool vec4::isOne() const {
        return x == 1.0f && y == 1.0f && z == 1.0f && w == 1.0f;
    }
}