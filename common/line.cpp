#include <elpch.h>
#include "line.h"
#include "vec2.h"

namespace el
{
    line::line() : p1x(0.0f), p1y(0.0f), p2x(0.0f), p2y(0.0f) {}
    line::line(float p1x_, float p1y_, float p2x_, float p2y_)
        : p1x(p1x_), p1y(p1y_), p2x(p2x_), p2y(p2y_) {};

    vec2 line::p1() const { return vec2(p1x, p1y); }
    vec2 line::p2() const { return vec2(p2x, p2y); }
}