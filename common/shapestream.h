/*****************************************************************//**
 * @file   shapestream.h
 * @brief  Streaming for 2d shape types
 * 
 * @author Sedomanai
 * @date   September 2022
 *********************************************************************/
#pragma once
#include "define.h"
#include "stream.h"
#include "line.h"
#include "aabb.h"
#include "circle.h"
#include "poly2d.h"

namespace el
{
    stream& operator<<(stream& save, const line& shape) {
        return save << shape.p1x << shape.p1y << shape.p2x << shape.p2y;
    }
    stream& operator>>(stream& load, line shape) {
        load >> shape.p1x >> shape.p1y >> shape.p2x >> shape.p2y;
        return load;
    }
    logger& operator<<(logger& log, const line& shape) {
        log << "Line( " << shape.p1x << ", " << shape.p1y << ")->" << "( " << shape.p2x << ", " << shape.p2y << " )";
        return log;
    }
    stream& operator<<(stream& save, const aabb& shape) {
        return save << shape.l << shape.b << shape.r << shape.t;
    }
    stream& operator>>(stream& load, aabb shape) {
        load >> shape.l >> shape.b >> shape.r >> shape.t;
        return load;
    }
    logger& operator<<(logger& log, const aabb& shape) {
        log << "AABB( L: " << shape.l << ", B: " << shape.b << ", R: " << shape.r << ", T: " << shape.t << " )";
        return log;
    }
    stream& operator<<(stream& save, const circle& shape) {
        return save << shape.x << shape.y << shape.r;
    }
    stream& operator>>(stream& load, circle shape) {
        load >> shape.x >> shape.y >> shape.r;
        return load;
    }
    logger& operator<<(logger& log, const circle& shape) {
        log << "Circle( Pos: ( " << shape.x << ", " << shape.y << " ), Radius: " << shape.r << " )";
        return log;
    }
    stream& operator<<(stream& save, const poly2d& shape) {
        save << shape.count;
        for (auto i = 0; i < shape.count; i++) {
            save << shape.norms[i].x << shape.norms[i].y << shape.verts[i].x << shape.verts[i].y;
        }
        return save;
    }
    stream& operator>>(stream& load, poly2d& shape) {
        load >> shape.count;
        for (auto i = 0; i < shape.count; i++) {
            load >> shape.norms[i].x >> shape.norms[i].y >> shape.verts[i].x >> shape.verts[i].y;
        }
    }
    logger& operator<<(logger& log, const poly2d& shape) {
        log << "A POLYGON. Please just debug with graphics.";
        return log;
    }
}