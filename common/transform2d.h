/*****************************************************************//**
 * @file   transform2d.h
 * @brief  Transform2d. For more info check the Transform2d comment documentaiton
 *
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
#include "../elang_builder.h"

namespace el
{
	struct vec2;
	struct vec3;
	struct aabb;

	/**
	 * @brief Transform2d is not a matrix3x3. Rather it uses abbreviated calculations for game related geometric transformations.
	 * This makes Transform2d a somewhat faster alternative to matrix3x3 (at the expense of 4 more bytes of memory).
	 * @brief * Neither is optimized by SIMD at the moment
	 */
	struct ELANG_DLL Transform2d
	{
		Transform2d();
		Transform2d(float tx_, float ty_, float tz_, float rx_, float ry_, float ux_, float uy_, float sx_, float sy_, float ez_);

		constexpr vec2 position() const;
		constexpr vec2 scale()	const;
		constexpr vec2 right()	const;
		constexpr vec2 up()		const;
		constexpr float depth() const;
		constexpr float euler() const;

		constexpr vec3 operator*(const vec3& v) const;
		constexpr vec2 operator*(const vec2& v) const;
		constexpr aabb operator*(const aabb& a) const;
		Transform2d operator*(const Transform2d& v) const;

		void to(float x, float y);
		void move(float x, float y);
		void scale(float x, float y);
		void resize(float x, float y);
		void rotate(float degrees);

		void toX(float value);
		void toY(float value);
		void toZ(float value);
		void resizeX(float value);
		void resizeY(float value);
		void rotateTo(float degrees);

		/**
		 * Used by Cereal
		 * 
		 * @param archive : InArchive / OutArchive
		 */
		template<typename T>
		void serialize(T& archive) {
			archive(tx, ty, tz, rx, ry, ux, uy, sx, uy, ez);
		}

	private:
		float tx, ty, tz;
		float rx, ry;
		float ux, uy;
		float sx, sy;
		float ez;
	};
};
