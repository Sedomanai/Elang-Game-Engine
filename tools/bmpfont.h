#pragma once
#include "atlas.h"

namespace el
{
	template<int N, int TextureType>
	struct MaterialImpl;


	template<int N>
	struct ELANG_DLL FontGlyphImpl
	{
		FontGlyphImpl() {};
		FontGlyphImpl(uint16 x, uint16 y, uint16 w, uint16 h, int16 oX, int16 oY, uint16 aw, uint16 ah, float adv_);

		template<typename T>
		void serialize(T& archive) {
			archive(left, down, right, up, uvLeft, uvDown, uvRight, uvUp, adv);
		}

		float left, down, right, up;
		float uvLeft, uvDown, uvRight, uvUp;
		float adv;
	};


	template<int N, int TextureIndex>
	struct ELANG_DLL FontFaceImpl
	{
		void makeFromBfntFile(const string& filePath);
		bihashmap<uint32, Entity> glyphs;
		asset<MaterialImpl<N, TextureIndex>> material;
		uint16 lineHeight;

		void destroy();

		template<typename T>
		void serialize(T& archive) {
			archive(glyphs, material, lineHeight);
		}
	};

	using FontGlyph = FontGlyphImpl<0>;
	using FontFace = FontFaceImpl<0, 0>;
}