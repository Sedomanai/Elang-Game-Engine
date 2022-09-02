#include <elpch.h>
#include "aabb.h"
#include "vec2.h"
#include "algorithm.h"

namespace el
{
    constexpr aabb::aabb() : l(0), b(0), r(0), t(0) {}
    constexpr aabb::aabb(float left, float bottom, float right, float top)
        : l(left), b(bottom), r(right), t(top) {}

    constexpr vec2 aabb::center() const {
        return vec2((l + r) / 2.0f, (b + t) / 2.0f);
    }
    constexpr float aabb::width() const {
        return r - l;
    }
    constexpr float aabb::height() const {
        return t - b;
    }
    constexpr float aabb::area() const {
        return (r - l) * (t - b);
    }
    constexpr void aabb::move(const vec2& value) {
        l += value.x;
        r += value.x;
        b += value.y;
        t += value.y;
    }
    constexpr void aabb::normalize() {
        if (r < l) {
            float temp = l;
            l = r;
            r = temp;
        }
        if (t < b) {
            float temp = b;
            b = t;
            t = temp;
        }
    }
    constexpr void aabb::expand(float value) { l -= value; r += value; b -= value; t += value; }
    constexpr void aabb::expand(float dx, float dy) { l -= dx; r += dx; b -= dy; t += dy;}

    constexpr bool aabb::contains(const vec2& point) const {
        return point.x >= l && point.x <= r && point.y >= b && point.y <= t;
    }

    constexpr bool aabb::contains(const aabb& a) const {
        return a.l >= l && a.r <= r && a.b >= b && a.t <= t;
    }
    constexpr bool aabb::intersects(const aabb& a) const {
        return (a.l <= r && l <= a.r && a.b <= t && b <= a.t);
    }
    constexpr aabb aabb::intersected(const aabb& a) const {
        return aabb(l > a.l ? l : a.l, b > a.b ? b : a.b, r < a.r ? r : a.r, t < a.t ? t : a.t);
    }
    constexpr aabb aabb::united(const aabb& a) const {
        return aabb(l<a.l ? l : a.l, b < a.b ? b : a.b, r>a.r ? r : a.r, t > a.t ? t : a.t);
    }
        
    constexpr void aabb::trap(vec2& point) {
        point.x = clamp(point.x, l, r);
        point.y = clamp(point.y, b, t);
    }
    constexpr void aabb::trap(aabb& a) {
        vec2 val;
        if (a.width() < width()) {
            if (a.l < l)
            	val.x = l - a.l;
            if (a.r > r)
                val.x = r - a.r;
        } else {
            a.r = l + a.width();
            a.l = l;
        }

        if (a.height() < height()) {
            if (a.b < b)
                val.y = b - a.b;
            if (a.t > t)
                val.y = t - a.t;
        } else {
            a.b = t - a.height();
            a.t = t;
        }
        a.move(val);
    }

    void aabb::roundCorners() {
        l = round(l);
        r = round(r);
        b = round(b);
        t = round(t);
    }
    void aabb::halfRoundCorners() {
        l = round(l + 0.5f) - 0.5f;
        r = round(r + 0.5f) - 0.5f;
        t = round(t + 0.5f) - 0.5f;
        b = round(b + 0.5f) - 0.5f;
    } 

    void aabb::halfRoundHorizontalCorners() {
        l = round(l + 0.5f) - 0.5f;
        r = round(r + 0.5f) - 0.5f;
    }
    void aabb::halfRoundVerticalCorners() {
        t = round(t + 0.5f) - 0.5f;
        b = round(b + 0.5f) - 0.5f;
    }
    void aabb::roundHorizontalCorners() {
        l = round(l);
        r = round(r);
    }
    void aabb::roundVerticalCorners() {
        b = round(b);
        t = round(t);
    }
    aabb aabb::copy(const vec2& offset) {
        return aabb(l + offset.x, b + offset.y, r + offset.x, t + offset.y);
    }
}