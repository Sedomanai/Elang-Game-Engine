#pragma once
#include "visage.h"
#include "basic.h"

namespace el
{
	
	template<typename V, int M, int T, int C>
	struct ELANG_DLL SpriteImpl : Quad<V, M, T, C>
	{
		SpriteImpl() : Quad<V, M, T, C>() {};
		SpriteImpl(asset<MaterialImpl<M, T>> material, asset<PainterImpl<M, T, C>> painter, const string& cell_key);

		void update(Entity e);
		void setCell(asset<CellImpl<T>> cell) { mCell = cell; }
		void setCell(const string& value);
		bool hasValidCell() { return mCell; }

		asset<CellImpl<T>> cell() { return mCell; }
		void flip() { mFlipped = !mFlipped; }
		void flipside(bool invert) { mFlipped = invert; }

		template<typename Arc>
		void serialize(Arc& archive) {
			archive(Visage<M, T, C>::material, Visage<M, T, C>::painter, mCell, mFlipped);
		}
	protected:
		void updatePositionSprite(obj<Position> position);
		void updatePlanarSprite(obj<Planar> plane);

		asset<CellImpl<T>> mCell;
		bool mFlipped;
	};

	using Sprite = SpriteImpl<SpriteVertex, 0, 0, 0>;
	using EditorSprite = SpriteImpl<SpriteVertex, 1, 1, 1>;
	using EditorProjectSprite = SpriteImpl<SpriteVertex, 1, 0, 1>;
	using EditorWorldSprite = SpriteImpl<SpriteVertex, 1, 1, 0>;
	using EditorWorldProjectSprite = SpriteImpl<SpriteVertex, 1, 0, 0>; // probably won't be used
}
