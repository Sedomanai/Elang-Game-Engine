#pragma once

#include "../elang_library_builder.h"
#include "../common/transform2d.h"
#include "../common/transform.h"
#include "../common/shape2d.h"
#include "../common/define.h"

namespace el
{
	struct Position : vec2 { EL_USING_BASE_CLASS(Position, vec2); };
	struct Planar : Transform2d { EL_USING_BASE_CLASS(Planar, Transform2d); };
	struct Spatial : Transform { EL_USING_BASE_CLASS(Spatial, Transform); };
	struct Box : aabb { EL_USING_BASE_CLASS(Box, aabb); };
	struct Radius : circle { EL_USING_BASE_CLASS(Radius, circle); };
}