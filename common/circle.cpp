#include <elpch.h>
#include "circle.h"
#include "vec2.h"

namespace el
{
    circle::circle() : x(0.0f), y(0.0f), r(0.0f) {}
    circle::circle(float x_, float y_, float radius)
        : x(x_), y(y_), r(radius) {}

    // @return True only if aabb "a" is fully contained within this aabb
    bool circle::contains(const vec2& point) const {
        auto dx = (x - point.x);
        auto dy = (y - point.y);
        return dx * dx + dy * dy < r* r;
    }

    /**
        * Copies a new circle out of this one.
        *
        * @param offset : A new circle in a new position determined by offset value
        * @return New circle
        */
    circle circle::copy(const vec2& offset) {
        return circle(x + offset.x, y + offset.y, r);
    }
}