#pragma once
#include "../tools/registry.h"
#include "../tools/material.h"
#include "../tools/painter.h"
#include "../elang_library_builder.h"

namespace el
{
	struct ELANG_DLL Visage
	{
		Visage() {}
		Visage(asset<Material> material_, asset<Painter> painter_) : material(material_), painter(painter_) {}

		asset<Material> material;
		asset<Painter> painter;
	};

	struct ELANG_DLL Quad : Visage
	{
		void batch();
		void sync(aabb&);
		void sync(circle&);
		void sync(poly2d&);

		void setLT(const SpriteVertex& vertex) { mVertices[0] = vertex; }
		void setRT(const SpriteVertex& vertex) { mVertices[1] = vertex; }
		void setRB(const SpriteVertex& vertex) { mVertices[2] = vertex; }
		void setLB(const SpriteVertex& vertex) { mVertices[3] = vertex; }

		Quad() : Visage(), mDepth(0) {};
		Quad(asset<Material> material_, asset<Painter> painter_);

	protected:
		SpriteVertex mVertices[4];
		float mDepth;
	};
}