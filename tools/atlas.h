#pragma once

#include "../elang_library_builder.h"
#include "../common/fileio.h"
#include "../common/container.h"
#include "registry.h"


namespace el
{
	template<int N>
	struct TextureImpl;

	template<int N>
	struct ELANG_DLL CellImpl
	{
		CellImpl() : oX(0), oY(0), index(-1) {};
		CellImpl(int x, int y, int w, int h, int oX_, int oY_, int aw, int ah, int index);
		float left, down, right, up;
		float uvLeft, uvDown, uvRight, uvUp;
		int16 oX, oY; 
		uint32 index;

		template<typename T>
		void serialize(T& archive) {
			archive(left, down, right, up, uvLeft, uvDown, uvRight, uvUp, oX, oY, index);
		}
	};

	template<int N>
	struct ClipImpl : vector<asset<CellImpl<N>>>
	{
		float speed;
		bool repeat;
		
		ClipImpl() : vector<asset<CellImpl<N>>>(), speed(1.0f), repeat(false) {}

		template<typename T>
		void serialize(T& archive) {
			archive(cereal::base_class<vector<asset<CellImpl<N>>>>(this), speed, repeat);
		}
	};

	// specializer: it apperas that vectors use non member serialization functions and clash with ClipImpl serialize()
	//				this resolves the conflict
	template <class Archive, int N>
	struct cereal::specialize<Archive, ClipImpl<N>, cereal::specialization::member_serialize> {};

	template<int N>
	struct ELANG_DLL AtlasImpl
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
		vector<asset<TextureImpl<N>>> textures;

		void makeFromAtlsFile(const string& filePath);
		void destroy();

		vector<asset<CellImpl<N>>> linearCells(eSortType sortType = eSortType::NONE);

		/// <summary>
		/// Destroys and recreates atlas cells to the back of the Cell pool
		/// </summary>
		vector<asset<CellImpl<N>>> packedAndCachedCells();
		void recreateCells(const vector<asset<CellImpl<N>>>&);

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

	using Cell = CellImpl<0>;
	using Clip = ClipImpl<0>;
	using Atlas = AtlasImpl<0>;
}