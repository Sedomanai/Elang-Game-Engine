#pragma once
#include "sprite.h"

namespace el
{

	template<int M, int T, int C>
	struct ELANG_DLL ClipAnimationImpl
	{
		ClipAnimationImpl(asset<ClipImpl<T>>);
		
		void setClip_WithoutFrameReset(asset<ClipImpl<T>>);
		void setClip(asset<ClipImpl<T>>);
		void update(obj<SpriteImpl<SpriteVertex, M, T, C>> sprite);
		uint32 frame() { return mCurrentFrame; }
		void reset() { mFrame = 0.0f; mCurrentFrame = 0; }

	private:
		asset<ClipImpl<T>> mClip;
		float mFrame, mSpeed;
		uint32 mCurrentFrame;
	};

	using ClipAnimation = ClipAnimationImpl<0, 0, 0>;
	using EditorClipAnimation = ClipAnimationImpl<1, 1, 1>;
	using EditorProjectClipAnimation = ClipAnimationImpl<1, 0, 1>;
}