#pragma once
#include "atlas.h"

namespace el
{
	template<int M, int T>
	struct MaterialImpl;


	template<int T>
	struct ELANG_DLL FontGlyphImpl
	{
		FontGlyphImpl() {};
		FontGlyphImpl(uint16 x, uint16 y, uint16 w, uint16 h, int16 oX, int16 oY, uint16 aw, uint16 ah, float adv_);

		template<typename Arc>
		void serialize(Arc& archive) {
			archive(left, down, right, up, uvLeft, uvDown, uvRight, uvUp, adv);
		}
		float left, down, right, up;
		float uvLeft, uvDown, uvRight, uvUp;
		float adv;
	};


	template<int M, int T>
	struct ELANG_DLL FontFaceImpl
	{
		void makeFromBfntFile(const string& filePath);
		bihashmap<uint32, Entity> glyphs;
		asset<MaterialImpl<M, T>> material;
		uint16 lineHeight;

		void destroy();

		template<typename Arc>
		void serialize(Arc& archive) {
			archive(glyphs, material, lineHeight);
		}
	};

	using FontGlyph = FontGlyphImpl<0>;
	using FontFace = FontFaceImpl<0, 0>;
	using EditorFontGlyph = FontGlyphImpl<1>;
	using EditorFontFace = FontFaceImpl<1, 1>;
}