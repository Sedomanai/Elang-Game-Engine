#include <elpch.h>
#include "camera.h"

namespace el
{

	void updateViewport(float left, float right, float bottom, float top) {
		float width = right - left;
		float height = top - bottom;

		auto& dat = gOrtho.data;
		dat[0] = 2.0f / width;
		dat[5] = 2.0f / height;
		dat[10] = 2.0f / ortho_far;

		dat[12] = (left + right) / (left - right);
		dat[13] = (bottom + top) / (bottom - top);
		dat[14] = -1.0f;
		dat[15] = 1.0f;

		float focal = -1000; // 카메라가 -1000일 때
		float far_ = 0.0f;

		float n = -0.000001f;
		float zDelta = far_ - n;
		float zNear2 = 2 * n;

		/*float top_p = -width / 2.0f * n / focal;
		float bottom_p = -top_p;
		float left_p = bottom_p * width / height;
		float right_p = top_p * width / height;*/

		/*float zDelta = far_ -n;
		float width_p = right_p - left_p;
		float height_p = top_p - bottom_p;*/

		float height_p = -height * n / focal;
		float width_p = height_p * width / height;

		auto& dat2 = gPerspective.data;
		dat2[0] = zNear2 / width_p;
		dat2[5] = zNear2 / height_p;
		dat2[10] = (far_ + n) / zDelta;
		dat2[11] = 1.0f;
		dat2[14] = zNear2 * far_ / zDelta;
	}

	void updateViewport(matrix4x4& ortho, float left, float right, float bottom, float top) {
		float width = right - left;
		float height = top - bottom;

		auto& dat = ortho.data;
		dat[0] = 2.0f / width;
		dat[5] = 2.0f / height;
		dat[10] = 2.0f / ortho_far;

		dat[12] = (left + right) / (left - right);
		dat[13] = (bottom + top) / (bottom - top);
		dat[14] = -1.0f;
		dat[15] = 1.0f;
	};
}