/*****************************************************************//**
 * @file   visage.h
 * @brief  Graphic batchables base
 *
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
#include "../tools/tool_declarer.h"
#include "../tools/project.h"

namespace el
{
	struct Material;
	struct Painter;
	struct aabb;
	struct circle;
	struct poly2d;
	/**
	 * @brief Any displayable graphic element that can be displayed must inherit from this class.
	 */
	struct ELANG_DLL Visage
	{
		Visage() {}
		Visage(asset<Material> material_, asset<Painter> painter_) : material(material_), painter(painter_) {}
		asset<Material> material;
		asset<Painter> painter;
	};

	/**
	 * @brief Any displayable graphic element that comes in a quad form. Has 4 vertices.
	 */
	template<typename VertexType>
	struct ELANG_DLL Quad : Visage
	{
		Quad() : Visage(), mDepth(0) {};
		Quad(asset<Material> material_, asset<Painter> painter_);

		// Batch displayable graphic element. Invoke every display loop
		// A valid material and painter must be populated for this to work.
		void batch();
		// Resize quad so that it syncs with any aabb. Works for ANY quad types. (Canvas, Sprite, etc)
		void sync(aabb&);
		// Resize quad so that it syncs with any inscribed circle. Works for ANY quad types. (Canvas, Sprite, etc)
		void sync(circle&);
		// Sync quad so that it syncs with any inscribed poly2d. Works for ANY quad types. (Canvas, Sprite, etc)
		void sync(poly2d&);

		// Set LeftTop vertex. Corresponds to mVertices[0].
		void setLT(const VertexType& vertex) { mVertices[0] = vertex; }
		// Set RightTop vertex. Corresponds to mVertices[1].
		void setRT(const VertexType& vertex) { mVertices[1] = vertex; }
		// Set RightBottom vertex. Corresponds to mVertices[2].
		void setRB(const VertexType& vertex) { mVertices[2] = vertex; }
		// Set LeftBottom vertex. Corresponds to mVertices[3].
		void setLB(const VertexType& vertex) { mVertices[3] = vertex; }

	protected:
		VertexType mVertices[4];
		float mDepth;
	};
}