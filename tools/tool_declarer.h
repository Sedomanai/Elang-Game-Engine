/*****************************************************************//**
 * @file   tool_declarer.h
 * @brief  General forward declaration for tools (to reduce compile time)
 * 
 * @author Sedomanai
 * @date   September 2022
 *********************************************************************/
#pragma once
#include "../common/string.h"
#include "../common/vector.h"
#include "../common/fileio.h"

namespace el
{
	struct vec2;
	struct vec3;
	struct vec4;
	struct matrix4x4;

	template<typename T>
	struct asset;
	template<typename T, typename U>
	struct hashmap;
}