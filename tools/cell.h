/*****************************************************************//**
 * @file   atlas.h
 * @brief  Cell of a sprite altas. May change the name in the future to reduce confusion
 * 
 * @author Sedomanai
 * @date   September 2022
 *********************************************************************/
#pragma once
#include "../common/define.h"
#include "../elang_builder.h"

namespace el
{
	struct stream;
	struct Cell;

	// Holds cell meta. Most probably erased in build
	struct ELANG_DLL CellMeta
	{
		CellMeta();

#pragma warning( push )
#pragma warning( disable : 26495 )
		CellMeta(sizet x_, sizet y_, sizet w_, sizet h_, sizet oX_, sizet oY_);
#pragma warning( pop )

		friend stream& operator<<(stream& save, const CellMeta& cc);
		friend stream& operator>>(stream& load, CellMeta& cc);
		int16 x, y, w, h, oX, oY;
	};

	// A cell data of an atlas. Has a left down right up data like an aabb, and corresponding uv
	struct ELANG_DLL Cell
	{
		Cell(const CellMeta& cc, sizet aw, sizet ah);
		void mold(const CellMeta& cc, sizet aw, sizet ah);
		float left, down, right, up;
		float uvLeft, uvDown, uvRight, uvUp;

		template<typename T>
		void serialize(T& archive) {
			archive(left, down, right, up, uvLeft, uvDown, uvRight, uvUp);
		}
	};
}