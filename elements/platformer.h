/*****************************************************************//**
 * @file   platformer.h
 * @brief  Very simple platformer logic
 * @brief  The entire thing is currently on hold, re-establishing the GUI is the primary focus
 * 
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/

#pragma once
#include "../tools/all.h"
#include "basic.h"

namespace el
{
	ELANG_DLL extern float gGravity;

	/* struct Slope : aabb
	{
		enum class eType
		{
			LEFT_BOTTOM,
			RIGHT_BOTTOM
		};

		Slope(float left = 0.0f, float bottom = 0.0f, float right = 0.0f, float top = 0.0f,
			eType type_ = eType::LEFT_BOTTOM, bool footwise_ = true)
			: aabb(left, bottom, right, top), type(type_), footwise(footwise_) {};


		float inclination() {
			if (type == eType::LEFT_BOTTOM)
				return (t - b) / (r - l);
			else
				return (b - t) / (r - l);
		}

		float slopeXtoY(float value) {
			auto slope = inclination();
			if (type == eType::LEFT_BOTTOM)
				return clamp(b + slope * (value - l), b, t);
			else
				return clamp(t + slope * (value - l), b, t);
		}

		void debug(ShapeDebug* debug, const color& c, bool fill) {
			color cc = c;
			cc.a = 50;
			//debug->fill.batchAABB(*this, cc, E_DANGLING_VERTICES);

			switch (type) {
			case eType::LEFT_BOTTOM:
				debug->line.batchline(vec2(l, b), vec2(r, t), c, E_DANGLING_VERTICES);
				break;
			case eType::RIGHT_BOTTOM:
				debug->line.batchline(vec2(r, b), vec2(l, t), c, E_DANGLING_VERTICES);
				break;
			}
		}

		bool footwise;
		eType type;
	}; */

	struct ELANG_DLL Platform : aabb
	{
		Platform() : aabb(), mSides(0), footwise(true) {};
		Platform(float left, float bottom, float right, float top) : aabb(left, bottom, right, top), mSides(0), footwise(true) {};

		enum class eSide {
			Left = 1,
			BOTTOM = 2,
			Right = 4,
			TOP = 8,
			TOP_SLOPE = 16,
			BOTTOM_SLOPE = 32
		};
		enum class eSlope {
			FLAT = 0,
			LEFT_BOTTOM,
			RIGHT_BOTTOM
		};

		float inclination();
		float slopeXtoY(float value);

		Platform& makeBox() {
			mSides = (char)15;
			return *this;
		}

		Platform& makeJumpThrough(eSlope slope) {
			mSides = (char)8;
			mSlope = (char)slope;
			if (slope > eSlope::FLAT) {
				mSides *= 2;
			} return *this;
		}

		Platform& makeHardSlope(eSlope slope) {
			mSlope = (char)slope;
			mSides = (char)48;
			return *this;
		}

		char slope() { return mSlope; }

		void close(eSide side) { mSides |= (char)side; }
		void open(eSide side) { mSides &= ~(char)side; }
		void toggle(eSide side) { mSides ^= (char)side; }
		bool isClosed(eSide side) { return (mSides & (char)side) == (char)side; }
		void debug(ShapeDebug2d* debug, const color8& c, bool fill);

		template<typename T>
		void serialize(T& archive) {
			archive(cereal::base_class<aabb>(this), footwise, mSides, mSlope);
		}

		bool footwise;
	private:
		char mSides;
		char mSlope;
	};

	struct ELANG_DLL Platformer : aabb
	{
		enum eHitType
		{
			NO_HIT = 0,
			LOW_HIT,
			HIGH_HIT,
			THIS_SHOULDN_T_HAPPEN
		};

		obj<Platform> ground() {
			return mGround;
		}

		void dwindle(float force) {
			if (mAxis.x > force) {
				mAxis.x -= force;
				if (mAxis.x < 0.0f)
					mAxis.x = 0.0f;
			}
			else {
				mAxis.x += force;
				if (mAxis.x > 0.0f)
					mAxis.x = 0.0f;
			}
		}
		void capture() {
			mPrev = (*this);
		}
		void move(float force) {
			mAxis.x += force;
		}
		void jump(float force) { mAxis.y = force; }
		void setCap(vec2 cap) { mCap = cap; }

		void applyForce(Position& pos);

		void hitTest(obj<Platform> platform_, Position& pos);

		void finishingTouch() {
			if (mHitX != eHitType::NO_HIT && !mCrossedX)
				mAxis.x = 0.0f;
			if (mHitY != eHitType::NO_HIT && !mCrossedY)
				mAxis.y = 0.0f;
		}

		eHitType hitTypeX() { return mHitX; }
		eHitType hitTypeY() { return mHitY; }
		vec2 axis() { return mAxis; }

		template<typename T>
		void serialize(T& archive) {
			archive(cereal::base_class<aabb>(this), mGround, mCap, mAxis, mHitX, mHitY, mPrev, mCrossedX, mCrossedY);
		}

	private:
		obj<Platform> mGround;
		vec2 mCap;
		vec2 mAxis;
		eHitType mHitX, mHitY;
		aabb mPrev;
		bool mCrossedX, mCrossedY;
	};
}