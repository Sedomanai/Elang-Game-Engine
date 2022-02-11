#pragma once

#include "../elang_library_builder.h"
#include "../common/fileio.h"
#include "../common/container.h"
#include "registry.h"


namespace el
{
	inline uint32 gAtlasSortIndex() {
		uint32 atlasSortIndex = 0;
		return atlasSortIndex++;
	}

	struct Texture;
	struct ELANG_DLL Cell
	{
		Cell() : oX(0), oY(0), index(-1) {};
		Cell(int x, int y, int w, int h, int oX_, int oY_, int aw, int ah, int index);
		float left, down, right, up;
		float uvLeft, uvDown, uvRight, uvUp;
		int16 oX, oY; 
		uint32 index;

		template<typename T>
		void serialize(T& archive) {
			archive(left, down, right, up, uvLeft, uvDown, uvRight, uvUp, index);
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
		enum class eSortType
		{
			NONE,
			INDEX,
			ENTITY_HANDLE,
			UV_POSITION,
			NAME,
		};

		bihashmap<string, Entity> cells;
		bihashmap<string, Entity> clips;
		vector<asset<Texture>> textures;

		void makeFromAtlsFile(const string& filePath);
		void destroy();

		vector<asset<Cell>> linearCells(eSortType sortType = eSortType::NONE);

		/// <summary>
		/// Destroys and recreates atlas cells to the back of the Cell pool
		/// </summary>
		void packAndCacheCells();
		void recreateCells(const vector<asset<Cell>>&);

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