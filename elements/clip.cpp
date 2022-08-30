#include "clip.h"

namespace el
{
	
	ClipAnimation::ClipAnimation(asset<Clip> clip) 
		: mClip(clip), mFrame(0.0f), mSpeed(1.0f), mCurrentFrame(0) { 
		if (clip) {
			mSpeed = clip->speed;
		}
	};

	
	void ClipAnimation::update(Sprite& sprite) {
		if (mClip && mClip->size() > 0.0f) {
			auto size = (float)mClip->size();

			if (mClip->repeat) {
				while (mFrame >= size) {
					mFrame -= size;
				}
			}

			if (mFrame < size) {
				mCurrentFrame = (uint32)floor(mFrame);
				auto cell = mClip->at(mCurrentFrame);
				if (sprite.cell() != cell) {
					sprite.setCell(cell);
				}

				mFrame += mSpeed * mClip->speed;
			}
		} else {
			sprite.setCell(asset<Cell>());
		}
	}

	
	void ClipAnimation::setClip(asset<Clip> clip) {
		mClip = clip;
		mFrame = 0;
	}

	
	void ClipAnimation::setClip_WithoutFrameReset(asset<Clip> clip) {
		mClip = clip;
	}
}