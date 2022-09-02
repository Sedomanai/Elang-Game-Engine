/*****************************************************************//**
 * @file   sprite.h
 * @brief  Sprites (with customizable vertex data)
 *
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
#include "../tools/tool_declarer.h"
#include "../common/aabb.h"
#include "../common/vec2.h"
#include "../tools/vertex.h"
#include "visage.h"

namespace el
{
	template<typename T>
	struct Quad;
	struct Material;
	struct Painter;
	struct SpriteVertex;
	struct Visage;
	struct Cell;
	struct Position;
	struct Planar;

	/**
	 * @brief Sprites are textured quads that is firmly dependent on an atlas Cell info
	 * @brief It is only resizable/rotatable via transform types and/or Quad::sync methods.
	 * @brief Its UV absolutely does not change until another Cell is set
	 * @brief When the cell is not set or is not valid, but the texture is, it will paint the entire texture from topleft corner.
	 */
	template<typename VertexType>
	struct ELANG_DLL SpriteType : Quad<VertexType>
	{
		SpriteType();
		SpriteType(asset<Material> material, asset<Painter> painter, const string& cell_key);

		/**
		 * Recalculate vertices. If the entity has either a Positon or Planar element, recalculates depending on its data.
		 * @brief When both are present, Planar is given precedence.
		 *
		 * @param Entity that holds this Sprite
		 */
		void recalc(Entity self);
		// Recalcuate vertices directly using a Position type
		void recalc(Position& position);
		// Recalcuate vertices using a Planar type
		void recalc(Planar& plane);
		// Set cell using Entity
		void setCell(asset<Cell> cell);
		// Set cell using the key of cell. The atlas data is recorded in the texture of the material
		void setCell(const string& cell_key);
		// Check whehter this sprite has a valid cell
		bool hasValidCell();
		// Get cell
		asset<Cell> cell();
		// Flip cell (currently not accurate, not important atm)
		void flip();
		// Flip cell (currently not accurate, not important atm)
		void flipside(bool invert);

		// Used by Cereal
		template<typename Arc>
		void serialize(Arc& archive) {
			archive(Visage::material, Visage::painter, mCell, mFlipped);
		}
	protected:

		asset<Cell> mCell;
		bool mFlipped;
	};

	using Sprite = SpriteType<SpriteVertex>;
}
