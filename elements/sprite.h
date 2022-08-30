#pragma once
#include "visage.h"
#include "basic.h"

namespace el
{
	
	template<typename VertexType>
	struct ELANG_DLL SpriteType : Quad<VertexType>
	{
		SpriteType();
		SpriteType(asset<Material> material, asset<Painter> painter, const string& cell_key);

		void update(Entity e);
		void setCell(asset<Cell> cell) { mCell = cell; }
		void setCell(const string& value);
		bool hasValidCell() { return mCell; }

		asset<Cell> cell() { return mCell; }
		void flip() { mFlipped = !mFlipped; }
		void flipside(bool invert) { mFlipped = invert; }

		template<typename Arc>
		void serialize(Arc& archive) {
			archive(Visage::material, Visage::painter, mCell, mFlipped);
		}
		void updatePositionSprite(Position& position);
		void updatePlanarSprite(Planar& plane);
	protected:

		asset<Cell> mCell;
		bool mFlipped;
	};

	using Sprite = SpriteType<SpriteVertex>;
}
