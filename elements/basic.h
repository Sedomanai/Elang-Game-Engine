#pragma once

#include "../elang_library_builder.h"
#include "../common/transform2d.h"
#include "../common/transform.h"
#include "../common/shape2d.h"

namespace el
{
	struct Position : vec2 { using vec2::vec2; };
	struct Planar : Transform2d { using Transform2d::Transform2d; };
	struct Spatial : Transform { using Transform::Transform; };
	struct Box : aabb { using aabb::aabb; };
	struct Radius : circle { using circle::circle; };
}