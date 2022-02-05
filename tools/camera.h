#pragma once

#include "../elang_library_builder.h"
#include "../common/transform.h"
#include "../common/container.h"
#include "registry.h"

namespace el
{
	constexpr int ortho_far = 100000000;

	struct ELANG_DLL Camera : Transform { using Transform::Transform; };
	enum class Projection
	{
		eOrtho, ePerspective
	};
	inline matrix4x4 gOrtho(1), gPerspective(0);

	extern ELANG_DLL void updateViewport(float left, float right, float bottom, float top);
	extern ELANG_DLL void updateViewport(matrix4x4& ortho, float left, float right, float bottom, float top);
}