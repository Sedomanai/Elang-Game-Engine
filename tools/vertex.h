/*****************************************************************//**
 * @file   vertex.h
 * @brief  All Vertex types. Should fit any graphic library. (Centered around OpenGL)
 *
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
#include "../common/define.h"
#include "../common/vec2.h"
#include "../common/vec3.h"

namespace el
{
	inline sizet gLineIndices[2] = { 0, 1 };
	inline sizet gBox2dFillIndices[6] = { 0, 1, 3, 1, 3, 2 };
	inline sizet gBox2dLineIndices[8] = { 0, 1, 1, 2, 2, 3, 3, 0 };

	struct color8
	{
		uint8 r, g, b, a;
		color8(uint8 r_, uint8 g_, uint8 b_, uint8 a_)
			: r(r_), g(g_), b(b_), a(a_) {
		}
	};

	/**
	 * vec3 pos, vec2 uv
	 */
	struct DefaultVertex
	{
		vec3 pos;
		vec2 uv;
	};

	/**
	 * vec3 pos, color8 col
	 */
	struct PrimitiveVertex
	{
		vec3 pos;
		color8 col;
	};

	/**
	 * vec2 pos
	 */
	struct Position2DVertex
	{
		vec2 pos;
	};

	/**
	 * vec2 pos, color8 col
	 */
	struct Primitive2DVertex
	{
		vec2 pos;
		color8 col;
	};

	/**
	 * vec2 pos, vec2 uv
	 */
	struct SpriteVertex
	{
		vec2 pos;
		vec2 uv;

		SpriteVertex() {}
		SpriteVertex(const vec2& pos_, const vec2& uv_) : pos(pos_), uv(uv_) {}
	};

	/**
	 * vec2 pos, vec2 uv, color8 col
	 */
	struct ColoredSpriteVertex
	{
		vec2 pos;
		vec2 uv;
		color8 col;
	};
}