/*****************************************************************//**
 * @file   canvas.h
 * @brief  Canvas refers to a manually resizable textured rect that keeps its UV data
 * 
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
#include "visage.h"
#include "../tools/tool_declarer.h"
#include "../tools/vertex.h"
#include "../common/aabb.h"
#include "../common/vec2.h"

namespace el
{
	template<typename T>
	struct Quad;
	struct Material;
	struct Painter;
	struct SpriteVertex;
	struct Visage;
	struct Cell;
	/**
	 * @brief Canvases are manually resizable texture quad that keeps its UV data.
	 * @brief The size of the Canvas is recorded in bounds (type: aabb)
	 * @brief It is possible to indirectly reimagine UV via uvsize and uvpos
	 * @brief However it is not possible (nor is it recommended) to manually access UV data
	 */
	template<typename VertexType>
	struct ELANG_DLL Canvas : Quad<VertexType>
	{
		Canvas();
		Canvas(asset<Material> material, asset<Painter> painter);

		/**
		 * @brief Recalculates vertex data. Method to call whenever there's a change in UV, aabb, and/or positioning.
		 */
		void recalc();
		/**
		 * @brief Recalculates vertex data. Method to call whenever there's a change in UV, and/or aabb
		 * @brief Accept a cell to reassign its UV corners. Unlike sprites, canvases do not hold its cell data
		 * 
		 * @param Recalculate using only cell's UV data and not its size parameters. Makes this Canvas into a resiable Sprite
		 */
		void recalc(asset<Cell>);

		// The bounds of the Canvas 
		aabb bounds;
		// UV position. Its exact behavior depnds on texture wrap type. Usually used for scrolling backgrounds
		vec2 uvpos; 
		// UV size. The bigger, the smaller the contents of the texture. Overriden when recalcalculating with cell
		vec2 uvsize;

		// Used by Cereal
		template<typename Arc>
		void serialize(Arc& archive) {
			archive(Visage::material, Visage::painter, bounds, uvpos, uvsize);
		}
	};
}