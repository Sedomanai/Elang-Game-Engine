#pragma once

#include "../elang_library_builder.h"
#include "../common/fileio.h"
#include "../common/container.h"
#include "registry.h"


namespace el
{
	struct Texture;
	struct ELANG_DLL Cell
	{
		Cell() {};
		Cell(int x, int y, int w, int h, int oX, int oY, int aw, int ah);
		float left, down, right, up;
		float uvLeft, uvDown, uvRight, uvUp;

		template<typename T>
		void serialize(T& archive) {
			archive(left, down, right, up, uvLeft, uvDown, uvRight, uvUp);
		}
	};

	struct Clipframe
	{
		uint16 cell;
		uint16 duration;

		template<typename T>
		void serialize(T& archive) {
			archive(cell, duration);
		}
	};

	using Clip = vector<Clipframe>;

	struct ELANG_DLL Atlas
	{
		bihashmap<string, Entity> cells;
		bihashmap<string, Entity> clips;
		vector<asset<Texture>> textures;

		void makeFromAtlsFile(const string& filePath);
		void destroy();

		template<typename T>
		void serialize(T& archive) {
			archive(cells, clips, textures);
		}
	};

	enum class AtlasAction
	{
		NONE,
		SET_CELL,
		SET_CLIP,
		SET_USER,
		SET_WIDTH,
		SET_HEIGHT,
	};

	inline hashmap<string, AtlasAction> gAtlasActions = {
		{"\t@c", AtlasAction::SET_CELL },
		{"\t@a", AtlasAction::SET_CLIP },
		{"\t@u", AtlasAction::SET_USER },
		{"width", AtlasAction::SET_WIDTH },
		{"height", AtlasAction::SET_HEIGHT },
	};
}