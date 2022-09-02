/*****************************************************************//**
 * @file   basic.h
 * @brief  Basic element types. They are all the equivalent children of existing data types
 *		   This is only done to let ENTT distinguish them as separate data types
 * 
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
#include "../common/transform2d.h"
#include "../common/transform.h"
#include "../common/circle.h"
#include "../common/aabb.h"
#include "../common/vec2.h"

namespace el
{
	struct Position : vec2 { EL_USING_BASE_CLASS(Position, vec2); };
	struct Planar : Transform2d { EL_USING_BASE_CLASS(Planar, Transform2d); };
	struct Spatial : Transform { EL_USING_BASE_CLASS(Spatial, Transform); };
	struct Box : aabb { EL_USING_BASE_CLASS(Box, aabb); };
	struct Radius : circle { EL_USING_BASE_CLASS(Radius, circle); };
}