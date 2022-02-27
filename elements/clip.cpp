#include "clip.h"

namespace el
{
	template struct ClipAnimationImpl<0, 0, 0>;
	template struct ClipAnimationImpl<1, 1, 1>;
	template struct ClipAnimationImpl<1, 0, 1>;

	template<int M, int T, int C>
	ClipAnimationImpl<M, T, C>::ClipAnimationImpl(asset<ClipImpl<T>> clip) 
		: mClip(clip), mFrame(0.0f), mSpeed(1.0f), mCurrentFrame(0) {};

	template<int M, int T, int C>
	void ClipAnimationImpl<M, T, C>::update(obj<SpriteImpl<SpriteVertex, M, T, C>> sprite) {
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
				if (sprite->cell() != cell) {
					sprite.update().setCell(cell);
				}

				mFrame += mSpeed * mClip->speed;
			}
		} else {
			sprite->setCell(asset<CellImpl<T>>());
			sprite.update();
		}
	}

	template<int M, int T, int C>
	void ClipAnimationImpl<M, T, C>::setClip(asset<ClipImpl<T>> clip) {
		mClip = clip;
		mFrame = 0;
	}

	template<int M, int T, int C>
	void ClipAnimationImpl<M, T, C>::setClip_WithoutFrameReset(asset<ClipImpl<T>> clip) {
		mClip = clip;
	}
}