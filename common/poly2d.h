/*****************************************************************//**
 * @file   poly2d.h
 * @brief  2D polygons (convex only)
 * 
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
#include "define.h"
#include "vec2.h"

namespace el
{

#define EL_MAX_POLYGON_VERTS 8
     // Polygon bound #count of corners, with their vertex position and their normals
    struct poly2d
    {
        sizet count;
        vec2 verts[EL_MAX_POLYGON_VERTS];
        vec2 norms[EL_MAX_POLYGON_VERTS];
    };
}