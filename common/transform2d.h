#pragma once

#include "math.h"

namespace el
{
	//=======================================================================================
	// Transform2d is not a matrix3x3
	// Rather it uses abbreviated calculations for game related geometric transformations
	// This makes Transform2d a much faster alternative to matrix3x3 (at the expense of 1 more variable)
	//=======================================================================================
	struct Transform2d
	{
		Transform2d() :
			tx(0.0f), ty(0.0f), tz(0.0f),
			rx(1.0f), ry(0.0f),
			ux(0.0f), uy(1.0f),
			sx(1.0f), sy(1.0f),
			ez(0.0f) {};

		Transform2d(float tx_, float ty_, float tz_, float rx_, float ry_, float ux_, float uy_, float sx_, float sy_, float ez_) :
			tx(tx_), ty(ty_), tz(tz_),
			rx(rx_), ry(ry_),
			ux(ux_), uy(uy_),
			sx(sx_), sy(sy_), 
			ez(ez_) {};

		constexpr vec2 position() const { return vec2(tx, ty); }
		constexpr vec2 scale()	const { return vec2(sx, sy); }
		constexpr vec2 right()	const { return vec2(rx / sx, ry / sx); }
		constexpr vec2 up()		const { return vec2(ux / sy, uy / sy); }
		constexpr float depth() const { return tz; }
		constexpr float euler() const { return ez; }

		constexpr vec3 operator*(const vec3& v) const {
			return vec3(rx * v.x + ux * v.y + tx, ry * v.x + uy * v.y + ty, tz);
		}
		constexpr vec2 operator*(const vec2& v) const {
			return vec2(rx * v.x + ux * v.y + tx, ry * v.x + uy * v.y + ty);
		}
		Transform2d operator*(const Transform2d& v) const {
			return Transform2d(
				rx * v.tx + ux * v.ty + tx,
				ry * v.tx + uy * v.ty + ty,
				tz + v.tz,

				rx * v.rx + ux * v.ry,
				ry * v.rx + uy * v.ry,

				rx * v.ux + ux * v.uy,
				ry * v.ux + uy * v.uy,

				sx * v.sx, sy * v.sy,
				ez + v.ez
			);
		}

		void to(float x, float y) {
			tx = x; 
			ty = y;
		}
		void move(float x, float y) {
			tx += x;
			ty += y;
		}

		void scale(float x, float y) {
			if (x == 0.0f)
				x = 0.00001f;
			if (y == 0.0f)
				y = 0.00001f;

			rx *= x;
			ry *= x;

			ux *= y;
			uy *= y;

			sx *= x;
			sy *= y;
		}

		void resize(float x, float y) {
			if (x == 0.0f)
				x = 0.00001f;
			if (y == 0.0f)
				y = 0.00001f;

			x /= sx;
			y /= sy;

			rx *= x;
			ry *= x;

			ux *= y;
			uy *= y;

			sx *= x;
			sy *= y;
		}

		void rotate(float degrees) {
			ez += degrees;

			auto rad = degrees * to_radians;
			float s = sin(rad);
			float c = cos(rad);

			float tpx = rx;
			rx = tpx * c - ry * s;
			ry = tpx * s + ry * c;

			tpx = ux;
			ux = tpx * c - uy * s;
			uy = tpx * s + uy * c;
		}

		void setPosX(float value) { tx = value; }
		void setPosY(float value) { ty = value; }
		void setPosZ(float value) { tz = value; }
		void setSclX(float value) {
			value = (value == 0.0f) ? 0.0001f : value;
			value /= sx;
			rx *= value;
			ry *= value;
			sx *= value;
		}
		void setSclY(float value) {
			value = (value == 0.0f)? 0.0001f : value;
			value /= sy;
			ux *= value;
			uy *= value;
			sy *= value;
		}
		void setRot(float degrees) {
			rotate(degrees - ez);
			ez = degrees;
		}

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
