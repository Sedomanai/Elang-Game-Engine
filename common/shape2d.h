#pragma once
#include "math.h"

namespace el
{
    struct line
    {
        line() : p1x(0.0f), p1y(0.0f), p2x(0.0f), p2y(0.0f) {}
        line(float p1x_, float p1y_, float p2x_, float p2y_) 
            : p1x(p1x_), p1y(p1y_), p2x(p2x_), p2y(p2y_) {};

        vec2 p1() const { return vec2(p1x, p1y); }
        vec2 p2() const { return vec2(p2x, p2y); }

        template<typename T>
        void serialize(T& archive) {
            archive(p1x, p1y, p2x, p2y);
        }

        float p1x, p1y, p2x, p2y;
    };

    struct aabb
	{
        constexpr aabb() : l(0), b(0), r(0), t(0) {}
        constexpr aabb(float left, float bottom, float right, float top)
            : l(left), b(bottom), r(right), t(top) {}

        constexpr vec2 center() const {
            return vec2((l + r) / 2.0f, (b + t) / 2.0f);
        }
        constexpr float width() const {
            return r - l;
        }
        constexpr float height() const {
            return t - b;
        }
        constexpr  float area() const {
            return (r - l) * (t - b);
        }
        constexpr void move(const vec2& value) {
            l += value.x;
            r += value.x;
            b += value.y;
            t += value.y;
        }
        constexpr void normalize() {
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
        constexpr bool contains(const vec2& point) const {
            return point.x >= l && point.x <= r && point.y >= b && point.y <= t;
        }
        constexpr bool contains(const aabb& a) const {
            return a.l >= l && a.r <= r && a.b >= b && a.t <= t;
        }
        constexpr bool intersects(const aabb& a) const {
            return (a.l <= r && l <= a.r && a.b <= t && b <= a.t);
        }
        constexpr aabb intersected(const aabb& a) const {
            return aabb(l > a.l ? l : a.l, b > a.b ? b : a.b, r < a.r ? r : a.r, t < a.t ? t : a.t);
        }
        constexpr aabb united(const aabb& a) const {
            return aabb(l<a.l ? l : a.l, b < a.b ? b : a.b, r>a.r ? r : a.r, t > a.t ? t : a.t);
        }
        constexpr void trap(vec2& point) {
            point.x = clamp(point.x, l, r);
            point.y = clamp(point.y, b, t);
        }
        constexpr void trap(aabb& a) {
            vec2 val;
            bool lo = a.l < l;
            bool ro = a.r > r;
            bool bo = a.b < b;
            bool to = a.t > t;

            if (lo) {
                if (!ro)
                    val.x = l - a.l;
            } else if (ro) {
                val.x = r - a.r;
            }

            if (bo)
                if (!to)
                    val.y = b - a.b;
            else if (to)
                val.y = t - a.t;

            a.move(val);
        }

        void roundCorners() {
            l = round(l);
            r = round(r);
            b = round(b);
            t = round(t);
        }

        void halfRoundCorners(bool hori, bool verti) {
            if (hori) {
                l = round(l + 0.5f) - 0.5f;
                r = round(r + 0.5f) - 0.5f;
            } else {
                l = round(l);
                r = round(r);
            }

            if (verti) {
                t = round(t + 0.5f) - 0.5f;
                b = round(b + 0.5f) - 0.5f;
            } else {
                t = round(t);
                b = round(b);
            }
        } 

        aabb copy(const vec2& offset) {
            return aabb(l + offset.x, b + offset.y, r + offset.x, t + offset.y);
        }
		
        template<typename T>
        void serialize(T& archive) {
            archive(l, b, r, t);
        }

        friend logger& operator<<(logger& log, const aabb& a) {
            log << "( " << a.l << ", " << a.b << ", " << a.r << ", " <<  a.t << " )";
            return log;
        }

        float l, b, r, t;
    };

    struct circle
    {
        circle() : x(0.0f), y(0.0f), r(0.0f) {}
        circle(float x_, float y_, float radius)
            : x(x_), y(y_), r(radius) {}

        bool contains(const vec2& point) const {
            auto dx = (x - point.x);
            auto dy = (y - point.y);
            return dx * dx + dy * dy < r* r;
        }

        circle copy(const vec2& offset) {
            return circle(x + offset.x, y + offset.y, r);
        }

        template<typename T>
        void serialize(T& archive) {
            archive(x, y, r);
        }

        float x, y, r;
    };

#define EL_MAX_POLYGON_VERTS 8
    struct poly2d
    {
        sizet count;
        vec2 verts[EL_MAX_POLYGON_VERTS];
        vec2 norms[EL_MAX_POLYGON_VERTS];
    };

	
}