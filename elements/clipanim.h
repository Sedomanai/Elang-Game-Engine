/*****************************************************************//**
 * @file   clip.h
 * @brief  Elang has two animation types. This is Clip animation (or more clearly, Spritesheet animation)
 * 
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
#include "../tools/project.h"
#include "../tools/tool_declarer.h"

namespace el
{
	struct Clip;
	template<typename T>
	struct SpriteType;
	struct SpriteVertex;
	/**
	 * @brief Clip Animations are simple animations using the data parsed by the (sprite) Atlas.
	 * Since the atlas should have all the pivot points and clip cell sequence already assigned, there's no need for Clip animations to reposition or recalculate anything.
	 * @brief * Extremely efficient for old-school animation where there's no necessary position tweening involved.
	 * @brief ** This also opens up ways to combine/blend Clip animation with Bone animation
	 */
	struct ELANG_DLL ClipAnimation
	{
		ClipAnimation(asset<Clip>);

		// Entity of assigned clip
		asset<Clip> clip();
		// Assign clip via entity
		void setClip(asset<Clip>);
		// Assign clip via enttiy
		// Sets clip withour resetting current frame index; great for swapping similar clips without breaks
		void setClipWithoutFrameReset(asset<Clip>);

		// Assign clip via string and atlas
		//void setClip(string clipName, string atlasName) {};

		// Loop method; call this every frame (or animation loop)
		void step(SpriteType<SpriteVertex>& sprite);
		// Get the current frame index 
		uint32 frame();
		// Changes the speed of the animation
		// This will NOT change the native speed of the animation
		// The actual speed of the animation will be this speed value multiplied by native clip speed
		void setSpeed(float speed);
		// Completely resets the animation (without pausing)
		void reset();
		// Unpause or start the animation
		void start();
		// Stop is equvialent to pausing and and resetting
		void stop();
		// Pause the Clip animation
		void pause();

	private:
		asset<Clip> mClip;
		float mFrame, mSpeed;
		uint32 mCurrentFrame;
		bool mPaused;
	};
}