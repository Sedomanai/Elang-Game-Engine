#include "platformer.h"

namespace el
{
	float gGravity = .3f;

	//Platform
	float Platform::inclination() {
		switch (mSlope) {
		case (char)eSlope::LEFT_BOTTOM:
			return (t - b) / (r - l);
		case (char)eSlope::RIGHT_BOTTOM:
			return (b - t) / (r - l);
		default:
			return 0.0f;
		}
	}
	float Platform::slopeXtoY(float value) {
		switch (mSlope) {
		case (char)eSlope::LEFT_BOTTOM:
			return clamp(b + inclination() * (value - l), b, t);
		case (char)eSlope::RIGHT_BOTTOM:
			return clamp(t + inclination() * (value - l), b, t);
		default:
			return t;
		}
	}

	void Platform::debug(ShapeDebug* debug, const color8& c, bool fill) {
		if ((mSides & 1) == 1) {
			debug->line.batchline(line(l, b, l, t), c, 0.0f);
		}
		if ((mSides & 2) == 2) {
			debug->line.batchline(line(l, b, r, b), c, 0.0f);
		}
		if ((mSides & 4) == 4) {
			debug->line.batchline(line(r, b, r, t), c, 0.0f);
		}
		if ((mSides & 8) == 8) {
			debug->line.batchline(line(l, t, r, t), c, 0.0f);
		}
		if (mSlope == (char)eSlope::LEFT_BOTTOM) {
			debug->line.batchline(line(l, b, r, t), c, 0.0f);
		}
		if (mSlope == (char)eSlope::RIGHT_BOTTOM) {
			debug->line.batchline(line(l, t, r, b), c, 0.0f);
		}
	}


	// Platformer
	void Platformer::applyForce(Position& pos) { // i.e. gravity
		mAxis.y -= gGravity;
		mAxis.y = clamp(mAxis.y, -mCap.y, mCap.y);
		mAxis.x = clamp(mAxis.x, -mCap.x, mCap.x);

		vec2 pass;
		if (mGround && mGround->slope() > 0) {
			//if ((mGround->inclination() > 0.0f && mAxis.x < 0.0f) || 
			//	(mGround->inclination() < 0.0f && mAxis.x > 0.0f))
			pass.y = mAxis.x * mGround->inclination();
			if (b + pass.y > mGround->t) {
				pass.y = mGround->t - b + gGravity + 0.02f;
			}
		}

		aabb::move(mAxis + pass);
		pos += mAxis + pass;
		mHitX = mHitY = eHitType::NO_HIT;
		mCrossedX = mCrossedY = false;
		mGround = ent<Platform>();
	}

	void Platformer::hitTest(ent<Platform> platform_, Position& pos) {
		auto& platform = *platform_;
		if (intersects(platform)) {
			vec2 offset;

			if (platform.isClosed(Platform::eSide::BOTTOM) && mPrev.t < platform.b) {
				offset.y = platform.b - t - 0.02f;
				mHitY = eHitType::HIGH_HIT;
			}
			else if (platform.isClosed(Platform::eSide::TOP) && mPrev.b > platform.t) {
				float foot = center().x;
				bool fixedFootwise = platform.footwise &&
					(
						(!platform.isClosed(Platform::eSide::LEFT) && foot < platform.l) ||
						(!platform.isClosed(Platform::eSide::RIGHT) && foot > platform.r)
						);

				if (!fixedFootwise) {
					offset.y = platform.t - b + 0.02f;
					mHitY = eHitType::LOW_HIT;
					mGround = platform_;
				}
			}

			if (platform.isClosed(Platform::eSide::LEFT) && mPrev.r < platform.l) {
				offset.x = platform.l - r - 0.02f;
				mHitX = eHitType::HIGH_HIT;
			}
			else if (platform.isClosed(Platform::eSide::RIGHT) && mPrev.l > platform.r) {
				offset.x = platform.r - l + 0.02f;
				mHitX = eHitType::LOW_HIT;
			}

			if (abs(offset.x) > 0.0f && abs(offset.y) > 0.0f) {
				if (mAxis.y == -gGravity) {
					offset.x = 0.0f;
					mCrossedX = true;
				}
				else {
					offset.y = 0.0f;
					mCrossedY = true;
				}

			}

			if (platform.slope() > (char)0) {
				auto target = platform.slopeXtoY((l + r) / 2.0f);
				auto ptarget = platform.slopeXtoY((mPrev.l + mPrev.r) / 2.0f);
				if (mPrev.b > ptarget) {
					if (platform.isClosed(Platform::eSide::TOP_SLOPE) && b <= target) {
						offset.y = target - b + 0.02f;
						mHitY = eHitType::LOW_HIT;
						mGround = platform_;
					}
				}
				else if (platform.isClosed(Platform::eSide::BOTTOM_SLOPE) && t > target && mAxis.y > 0.0f) {
					offset.y = target - t - 0.02f;
					mHitY = eHitType::HIGH_HIT;
				}
			}

			pos += offset;
			aabb::move(offset);
		}
	}
}

