/*****************************************************************//**
 * @file   line.h
 * @brief  2D line segment
 *
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
#include "../elang_builder.h"

namespace el
{
	struct stream;
	struct vec2;
	/**
	 * @brief Line between 2 points with x,y coordinates.
	 */
	struct ELANG_DLL line
	{
		line();
		line(float p1x_, float p1y_, float p2x_, float p2y_);

		vec2 p1() const;
		vec2 p2() const;

		/**
		 * Used by Cereal
		 *
		 * @param archive : InArchive/OutArchive
		 */
		template<typename T>
		void serialize(T& archive) {
			archive(p1x, p1y, p2x, p2y);
		}

		float p1x, p1y, p2x, p2y;
	};
}