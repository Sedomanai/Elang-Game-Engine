/*****************************************************************//**
 * @file   vecstream.h
 * @brief  Streaming for math vec types
 * 
 * @author Sedomanai
 * @date   September 2022
 *********************************************************************/
#pragma once
#include "define.h"
#include "stream.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"

namespace el
{
    stream& operator<<(stream& save, const vec2& vec) {
        save << vec.x << vec.y;
        return save;
    }
    stream& operator>>(stream& load, vec2& vec) {
        load >> vec.x >> vec.y;
        return load;
    }
    logger& operator<<(logger& log, const vec2& vec) {
        log << "( " << vec.x << ", " << vec.y << " )";
        return log;
    }

    stream& operator<<(stream& save, const vec3& vec) {
        save << vec.x << vec.y << vec.z;
        return save;
    }
    stream& operator>>(stream& load, vec3& vec) {
        load >> vec.x >> vec.y << vec.z;
        return load;
    }
    logger& operator<<(logger& log, const vec3& vec) {
        log << "( " << vec.x << ", " << vec.y << ", " << vec.z << " )";
        return log;
    }

    stream& operator<<(stream& save, const vec4& vec) {
        save << vec.x << vec.y << vec.z << vec.w;
        return save;
    }
    stream& operator>>(stream& load, vec4& vec) {
        load >> vec.x >> vec.y << vec.z << vec.w;
        return load;
    }
    logger& operator<<(logger& log, const vec4& vec) {
        log << "( " << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << " )";
        return log;
    }
}