/*****************************************************************//**
 * @file   circle.h
 * @brief  2d circle
 *
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
#include "../elang_builder.h"

namespace el
{
	struct vec2;
	/**
	 * @brief Circular bound with position x, y, and radius values
	 */
	struct ELANG_DLL circle
	{
		circle();
		circle(float x_, float y_, float radius);

		// @return True only if aabb "a" is fully contained within this aabb
		bool contains(const vec2& point) const;

		/**
		 * Copies a new circle out of this one.
		 *
		 * @param offset : A new circle in a new position determined by offset value
		 * @return New circle
		 */
		circle copy(const vec2& offset);

		/**
		 * Used by Cereal
		 *
		 * @param InArchive/OutArchive
		 */
		template<typename T>
		void serialize(T& archive) {
			archive(x, y, r);
		}

		float x, y, r;
	};
}