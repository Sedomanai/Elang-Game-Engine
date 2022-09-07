#include <elpch.h>
#include "define.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "math.h"

namespace el
{
    constexpr vec2::vec2() : x(0), y(0) {};
    constexpr vec2::vec2(float x_, float y_) : x(x_), y(y_) {};
    constexpr vec2::vec2(const vec2& v) : x(v.x), y(v.y) {}
    constexpr vec2::vec2(const vec3& v) : x(v.x), y(v.y) {};
    constexpr vec2::vec2(const vec4& v) : x(v.x), y(v.y) {};

    constexpr vec2& vec2::operator=(const vec2& v) {
        x = v.x; y = v.y;
        return *this;
    }
    constexpr vec2& vec2::operator+=(const vec2& v) {
        x += v.x; y += v.y;
        return *this;
    }
    constexpr vec2& vec2::operator-=(const vec2& v) {
        x -= v.x; y -= v.y;
        return *this;
    }
    constexpr bool vec2::operator==(const vec2& v) {
        return x == v.x && y == v.y;
    }
    constexpr bool vec2::operator!=(const vec2& v) {
        return x != v.x || y != v.y;
    }
    constexpr vec2& vec2::operator*=(const float v) {
        x *= v; y *= v;
        return *this;
    }
    constexpr vec2& vec2::operator/=(const float v) {
        x /= v; y /= v;
        return *this;
    }
    constexpr vec2 vec2::operator+(const vec2& v) const {
        return vec2(x + v.x, y + v.y);
    }
    constexpr vec2 vec2::operator-(const vec2& v) const {
        return vec2(x - v.x, y - v.y);
    }
    constexpr vec2 vec2::operator*(const float v) const {
        return vec2(x * v, y * v);
    }
    constexpr vec2 vec2::operator/(const float v) const {
        return vec2(x / v, y / v);
    }
    constexpr vec2 vec2::operator-() const {
        return vec2(-x, -y);
    }
    constexpr bool vec2::isZero() const {
        return x == 0.0f && y == 0.0f;
    }
    constexpr bool vec2::isOne() const {
        return x == 1.0f && y == 1.0f;
    }
    constexpr float vec2::magnitude() const {
        return el_math::sqrt(x * x + y * y);
    }
    constexpr float	vec2::magnitudeSquared() const {
        return x * x + y * y;
    }
    constexpr float vec2::distanceFrom(const vec2& ent) const {
        float dx = x - ent.x;
        float dy = y - ent.y;
        return el_math::sqrt(dx * dx + dy * dy);
    }

    /**
        * distanceFrom without square root. Great for distance comparisons without in need of actual distance value.
        *
        * @param ent- Distance from this vector point to this vector
        * @return Squared value of distance in float
        */
    constexpr float vec2::squaredDistanceFrom(const vec2& ent) const {
        float dx = x - ent.x;
        float dy = y - ent.y;
        return dx * dx + dy * dy;
    }

    /**
        * Creates new vector with normalized value.
        *
        * @return Normalized new vector
        */
    constexpr vec2 vec2::normalized() const {
        if (x != 0.0f && y != 0.0f) {
            float mag = 1.0f / el_math::sqrt(x * x + y * y);
            return vec2(x * mag, y * mag);
        } else return vec2();
    }

    /**
        * Normalizes this vector. Value before normalization is lost.
        */
    void vec2::normalize() {
        if (x != 0.0f && y != 0.0f) {
            float mag = 1.0f / el_math::sqrt(x * x + y * y);
            x *= mag; y *= mag;
        }
    }
}