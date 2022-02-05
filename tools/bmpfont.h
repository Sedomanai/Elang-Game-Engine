#pragma once
#include "atlas.h"

namespace el
{
	struct Material;
	struct ELANG_DLL FontGlyph
	{
		FontGlyph() {};
		FontGlyph(uint16 x, uint16 y, uint16 w, uint16 h, int16 oX, int16 oY, uint16 aw, uint16 ah, float adv_);

		template<typename T>
		void serialize(T& archive) {
			archive(left, down, right, up, uvLeft, uvDown, uvRight, uvUp, adv);
		}

		float left, down, right, up;
		float uvLeft, uvDown, uvRight, uvUp;
		float adv;
	};

	struct ELANG_DLL FontFace 
	{
		void makeFromBfntFile(const string& filePath);
		bihashmap<uint32, Entity> glyphs;
		asset<Material> material;
		uint16 lineHeight;

		void destroy();

		template<typename T>
		void serialize(T& archive) {
			archive(glyphs, material, lineHeight);
		}
	};
}