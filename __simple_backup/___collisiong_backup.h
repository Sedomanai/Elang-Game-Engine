#pragma once

#include <cute/cute_c2.h>

#include "../elang_library_builder.h"
#include "../common/container.h"
#include "../common/math.h"


namespace el
{
    struct ShapeDebug;

    struct ELANG_DLL aabb
    {
        aabb() : l(0), b(0), r(0), t(0) {}
        aabb(float left, float bottom, float right, float top)
            : l(left), b(bottom), r(right), t(top) {}


        vec2 center() const {
            return vec2((l + r) / 2.0f, (b + t) / 2.0f);
        }
        float width() const {
            return r - l;
        }
        float height() const {
            return t - b;
        }
        float area() const {
            return (r - l) * (t - b);
        }
        void move(const vec2& value) {
            l += value.x;
            r += value.x;
            b += value.y;
            t += value.y;
        }
        void normalize() {
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
        bool contains(const vec2& point) const {
            return point.x >= l && point.x <= r && point.y >= b && point.y <= t;
        }
        bool intersects(const aabb& a) const {
            return (a.l <= r && l <= a.r && a.b <= t && b <= a.t);
        }
        aabb united(const aabb& a) const {
            return aabb(l<a.l ? l : a.l, b < a.b ? b : a.b, r>a.r ? r : a.r, t > a.t ? t : a.t);
        }
        aabb intersected(const aabb& a) const {
            return aabb(l > a.l ? l : a.l, b > a.b ? b : a.b, r < a.r ? r : a.r, t < a.t ? t : a.t);
        }
        void trap(vec2& point) {
            point.x = clamp(point.x, l, r);
            point.y = clamp(point.y, b, t);
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

        aabb copy(vec2 offset) {
            return aabb(l + offset.x, b + offset.y, r + offset.x, t + offset.y);
        }

        void debug(ShapeDebug* debug, const color& c, bool fill);

        template<typename T>
        void serialize(T& archive) {
            archive(l, b, r, t);
        }

        float l, b, r, t;
    };

    struct ELANG_DLL circle
    {
        circle() : x(0.0f), y(0.0f), r(0.0f) {}
        circle(float x_, float y_, float radius)
            : x(x_), y(y_), r(radius) {}

        bool contains(const vec2& point) const {
            auto dx = (x - point.x);
            auto dy = (y - point.y);
            return dx * dx + dy * dy < r* r;
        }
        void debug(ShapeDebug* debug, const color& c, bool fill);

        circle copy(vec2 offset) {
            return circle(x + offset.x, y + offset.y, r);
        }

        template<typename T>
        void serialize(T& archive) {
            archive(x, y, r);
        }

        float x, y, r;
    };

    #define IL_MAX_POLYGON_VERTS 8
    struct ELANG_DLL poly2d
    {
        int count;
        vec2 verts[IL_MAX_POLYGON_VERTS];
        vec2 norms[IL_MAX_POLYGON_VERTS];

        //bool contains(const vec2& point) const {
        //    auto circ = c2Circle{ c2v{ point.x, point.y }, 0.00001f };
        //    return c2CircletoPoly(circ, reinterpret_cast<const c2Poly*>(this), 0);
        //}
        aabb findRange();
    };

    extern ELANG_DLL void norms2d(vec2* verts, vec2* norms, int count);
    extern ELANG_DLL int hitPolyToPoly(const poly2d& a, const poly2d& b);

    // SFINAE
    /*template<typename T>
    class ELANG_DLL IsButtonShape
    {
        typedef char one;
        struct two { char x[2]; };

        template <typename C> static one test(decltype(&C::contains));
        template <typename C> static two test(...);

    public:
        enum { value = sizeof(test<T>(0)) == sizeof(char) };
    };*/
}


