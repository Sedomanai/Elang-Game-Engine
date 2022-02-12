#pragma once
#include "../tools/registry.h"
#include "../tools/material.h"
#include "../tools/painter.h"
#include "../elang_library_builder.h"

namespace el
{
	template<int M, int T, int C>
	struct ELANG_DLL Visage
	{
		Visage<M, T, C>() {}
		Visage<M, T, C>(asset<MaterialImpl<M, T>> material_, asset<PainterImpl<M, T, C>> painter_) : material(material_), painter(painter_) {}
		asset<MaterialImpl<M, T>> material;
		asset<PainterImpl<M, T, C>> painter;
	};

	template<typename V, int M, int T, int C>
	struct ELANG_DLL Quad : Visage<M, T, C>
	{
		Quad() : Visage<M, T, C>(), mDepth(0) {};
		Quad(asset<MaterialImpl<M, T>> material_, asset<PainterImpl<M, T, C>> painter_);

		void batch();
		void sync(aabb&);
		void sync(circle&);
		void sync(poly2d&);

		void setLT(const V& vertex) { mVertices[0] = vertex; }
		void setRT(const V& vertex) { mVertices[1] = vertex; }
		void setRB(const V& vertex) { mVertices[2] = vertex; }
		void setLB(const V& vertex) { mVertices[3] = vertex; }

	protected:
		V mVertices[4];
		float mDepth;
	};
}