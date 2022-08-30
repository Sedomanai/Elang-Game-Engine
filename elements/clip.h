#pragma once
#include "sprite.h"

namespace el
{
	struct ELANG_DLL ClipAnimation
	{
		ClipAnimation(asset<Clip>);
		
		void setClip_WithoutFrameReset(asset<Clip>);
		void setClip(asset<Clip>);
		asset<Clip> clip() { return mClip; }
		void update(Sprite& sprite);
		uint32 frame() { return mCurrentFrame; }
		void reset() { mFrame = 0.0f; mCurrentFrame = 0; }

	private:
		asset<Clip> mClip;
		float mFrame, mSpeed;
		uint32 mCurrentFrame;
	};
}