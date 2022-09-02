/*****************************************************************//**
 * @file   aabb.h
 * @brief  AABB related
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
	 * @brief Rectanble bound with left, bottom, right, and top values.
	 */
	struct ELANG_DLL aabb
	{
		constexpr aabb();
		constexpr aabb(float left, float bottom, float right, float top);
		constexpr vec2 center() const;
		constexpr float width() const;
		constexpr float height() const;
		constexpr float area() const;
		constexpr void move(const vec2& value);
		constexpr void normalize();
		// @return Expands rect by value on each side (that is, not halved)
		constexpr void expand(float value);
		// @return Expands rect, dx horizontally and dy vertically, on each side (that is, not halved)
		constexpr void expand(float dx, float dy);

		// @return True only if point is fully contained within this aabb
		constexpr bool contains(const vec2& point) const;

		// @return True only if aabb "a" is fully contained within this aabb
		constexpr bool contains(const aabb& a) const;
		// @return True if aabb "a" intersects or even just completely touches this aabb
		constexpr bool intersects(const aabb& a) const;
		// @return Returns a new rect derived from the intersection betwwen "a" and this aabb
		constexpr aabb intersected(const aabb& a) const;
		// @return Returns a new rect derived from the unification of outermost bounds betwwen "a" and this aabb
		constexpr aabb united(const aabb& a) const;

		/**
		 * Traps a point within this aabb, uses clamp.
		 *
		 * @param point - The point to trap in vec2, overwrites value
		 */
		constexpr void trap(vec2& point);
		/**
		 * @brief Traps an aabb within this aabb, uses clamp.
		 * @brief If a dimension of the aabb to trap is smaller than the same dimension of this aabb,
		 * the aabb is snapped to either the the left or top (lt corner).
		 *
		 * @param a : The aabb to trap in vec2, overwrites value
		 */
		constexpr void trap(aabb& a);

		// Round all corners to the nearest integers. 
		void roundCorners();
		// Round all corners to the nearest multiples of 0.5 (including negatives)
		void halfRoundCorners();
		// Round all corners to the nearest multiples of 0.5 (including negatives), left and right only
		void halfRoundHorizontalCorners();
		// Round all corners to the nearest multiples of 0.5 (including negatives), bottom and top only
		void halfRoundVerticalCorners();
		// Round to the nearest integers, left and right only
		void roundHorizontalCorners();
		// Round to the nearest integers, bottom and top only
		void roundVerticalCorners();

		/**
		 * Copies a new aabb out of this one.
		 *
		 * @param offset : A new aabb in a new position determined by offset value
		 * @return New aabb
		 */
		aabb copy(const vec2& offset);

		/**
		 * Used by Cereal
		 *
		 * @param InArchive/OutArchive
		 */
		template<typename T>
		void serialize(T& archive) {
			archive(l, b, r, t);
		}

		//friend logger& operator<<(logger& log, const aabb& a) {
		//    log << "( " << a.l << ", " << a.b << ", " << a.r << ", " <<  a.t << " )";
		//    return log;
		//}

		float l, b, r, t;
	};
}