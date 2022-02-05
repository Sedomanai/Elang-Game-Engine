#pragma once

#include <GL/glew.h>

#include "../common/container.h"
#include "../common/math.h"
#include "../common/string.h"
#include "../common/enums.h"


namespace el {
	inline sizet gLineIndices[2] = { 0, 1 };
	inline sizet gBox2dFillIndices[6] = { 0, 1, 3, 1, 3, 2 };
	inline sizet gBox2dLineIndices[8] = { 0, 1, 1, 2, 2, 3, 3, 0 };

	struct DefaultVertex
	{
		vec3 pos;
		vec2 uv;
	};

	struct PrimitiveVertex 
	{
		vec3 pos;
		color8 col;
	};

	struct Position2DVertex
	{
		vec2 pos;
	};

	struct Primitive2DVertex 
	{
		vec2 pos;
		color8 col;
	};

	struct SpriteVertex
	{
		vec2 pos;
		vec2 uv;

		SpriteVertex() {}
		SpriteVertex(const vec2& pos_, const vec2& uv_) : pos(pos_), uv(uv_) {}
	};

	struct ColoredSpriteVertex
	{
		vec2 pos;
		vec2 uv;
		color8 col;
	};
}