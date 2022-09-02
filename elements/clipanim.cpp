#include <elpch.h>
#include "clipanim.h"
#include "sprite.h"
#include "visage.h"
#include "../tools/clip.h"

namespace el
{
	
	ClipAnimation::ClipAnimation(asset<Clip> clip) 
		: mPaused(false), mClip(clip), mFrame(0.0f), mSpeed(1.0f), mCurrentFrame(0) { 
		if (clip) {
			mSpeed = clip->speed;
		}
	};

	void ClipAnimation::step(Sprite& sprite) {
		if (!mPaused && mClip && mClip->cells.size() > 0.0f) {
			auto size = (float)mClip->cells.size();

			if (mClip->repeat) {
				while (mFrame >= size) {
					mFrame -= size;
				}
			}

			if (mFrame < size) {
				mCurrentFrame = (uint32)floor(mFrame);
				auto cell = mClip->cells.at(mCurrentFrame);
				if (sprite.cell() != cell) {
					sprite.setCell(cell);
				} mFrame += mSpeed * mClip->speed;
			}
		} else {
			sprite.setCell(asset<Cell>());
		}
	}

	asset<Clip> ClipAnimation::clip() { return mClip; }
	void ClipAnimation::setClip(asset<Clip> clip) { mClip = clip; mFrame = 0; }
	void ClipAnimation::setClipWithoutFrameReset(asset<Clip> clip) { mClip = clip; }

	uint32 ClipAnimation::frame() { return mCurrentFrame; }
	void ClipAnimation::setSpeed(float speed) { mSpeed = speed; }
	void ClipAnimation::reset() { mFrame = 0.0f; mCurrentFrame = 0; }
	void ClipAnimation::start() { mPaused = false; }
	void ClipAnimation::stop() { pause(); reset(); }
	void ClipAnimation::pause() { mPaused = true; }

}