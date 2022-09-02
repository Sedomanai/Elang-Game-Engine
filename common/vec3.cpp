#include <elpch.h>
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "math.h"

namespace el
{
    constexpr vec3::vec3() : x(0), y(0), z(0) {};
    constexpr vec3::vec3(float common) : x(common), y(common), z(common) {};
    constexpr vec3::vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {};
    constexpr vec3::vec3(const vec3& v) : x(v.x), y(v.y), z(v.z) {}
    constexpr vec3::vec3(const vec2& v, float z_) : x(v.x), y(v.y), z(z_) {};
    constexpr vec3::vec3(const vec4& v) : x(v.x), y(v.y), z(v.z) {};;

    constexpr vec3& vec3::operator=(const vec3& v) {
        x = v.x; y = v.y; z = v.z;
        return *this;
    }
    constexpr vec3& vec3::operator+=(const vec3& v) {
        x += v.x; y += v.y; z += v.z;
        return *this;
    }
    constexpr vec3& vec3::operator-=(const vec3& v) {
        x -= v.x; y -= v.y; z -= v.z;
        return *this;
    }
    constexpr vec3& vec3::operator*=(const float v) {
        x *= v; y *= v; z *= v;
        return *this;
    }
    constexpr bool vec3::operator==(const vec3& v) {
        return x == v.x && y == v.y && z == v.z;
    }
    constexpr bool vec3::operator!=(const vec3& v) {
        return x != v.x || y != v.y || z != v.z;
    }
    constexpr vec3& vec3::operator*=(const vec3& v) {
        x *= v.x; y *= v.y; z *= v.z;
        return *this;
    }
    constexpr vec3& vec3::operator/=(const float v) {
        x /= v; y /= v; z /= v;
        return *this;
    }
    constexpr vec3 vec3::operator+(const vec3& v) const {
        return vec3(x + v.x, y + v.y, z + v.z);
    }
    constexpr vec3 vec3::operator-(const vec3& v) const {
        return vec3(x - v.x, y - v.y, z - v.z);
    }
    constexpr vec3 vec3::operator*(const float v) const {
        return vec3(x * v, y * v, z * v);
    }
    constexpr vec3 vec3::operator*(const vec3& v) const {
        return vec3(x * v.x, y * v.y, z * v.z);
    }
    constexpr vec3 vec3::operator^(const vec3& v) const {
        return vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }
    constexpr vec3 vec3::operator/(const float v) const {
        return vec3(x / v, y / v, z / v);
    }
    constexpr vec3 vec3::operator-() const {
        return vec3(-x, -y, -z);
    }
    constexpr bool vec3::isZero() const {
        return x == 0.0f && y == 0.0f && z == 0.0f;
    }
    constexpr bool vec3::isOne() const {
        return x == 1.0f && y == 1.0f && z == 1.0f;
    }
    constexpr float vec3::magnitude() const {
        return el_math::sqrt(x * x + y * y + z * z);
    }
    constexpr float	vec3::magnitudeSquared() const {
        return x * x + y * y + z * z;
    }

    constexpr float vec3::distanceFrom(const vec3& ent) const {
        float dx = x - ent.x;
        float dy = y - ent.y;
        float dz = z - ent.z;
        return el_math::sqrt(dx * dx + dy * dy + dz * dz);
    }

    /**
        * distanceFrom method without square root. Great for distance comparisons without in need of actual distance value.
        *
        * @param ent- Distance from this vector point to this vector
        * @return Squared value of distance in float
        */
    constexpr float vec3::squaredDistanceFrom(const vec3& ent) const {
        float dx = x - ent.x;
        float dy = y - ent.y;
        float dz = z - ent.z;
        return dx * dx + dy * dy + dz * dz;
    }

    /**
        * Creates new vector with normalized value.
        *
        * @return Normalized new vector
        */
    constexpr vec3 vec3::normalized() const {
        if (x != 0.0f || y != 0.0f || z != 0.0f) {
            float mag = 1.0f / sqrt(x * x + y * y + z * z);
            return vec3(x * mag, y * mag, z * mag);
        } else return vec3();
    }

    /**
        * Normalizes this vector. Value before normalization is lost.
        */
    void vec3::normalize() {
        if (x != 0.0f || y != 0.0f || z != 0.0f) {
            float mag = 1.0f / sqrt(x * x + y * y + z * z);
            x *= mag; y *= mag; z *= mag;
        }
    }
    void vec3::preventZero() {
        if (x == 0.0f)
            x = 0.00001f;
        if (y == 0.0f)
            y = 0.00001f;
        if (z == 0.0f)
            z = 0.00001f;
    }

    vec4 vec3::quaternion() const {
        // -radians(val * 0.5f)
        float rx = -x * el_math::pi * el_math::div_360;
        float ry = -y * el_math::pi * el_math::div_360;
        float rz = -z * el_math::pi * el_math::div_360;

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
}