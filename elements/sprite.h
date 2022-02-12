#pragma once
#include "visage.h"
#include "basic.h"

namespace el
{
	
	struct ELANG_DLL Sprite : Quad
	{
		Sprite() : Quad() {};
		Sprite(asset<Material> material, asset<Painter> painter, const string& cell_key);

		void update(Entity e);
		void setCell(const string& value);
		void flip() { mFlipped = !mFlipped; }
		void flipside(bool invert) { mFlipped = invert; }

		template<typename T>
		void serialize(T& archive) {
			archive(material, painter, mCell, mFlipped);
		}
	protected:
		void updatePositionSprite(obj<Position> position);
		void updatePlanarSprite(obj<Planar> plane);

		asset<Cell> mCell;
		bool mFlipped;
	};
}
