#pragma once
#include "../tools/registry.h"
#include "../tools/material.h"
#include "../tools/painter.h"

namespace el
{
	struct ELANG_DLL Visage
	{
		Visage() {}
		Visage(asset<Material> material_, asset<Painter> painter_) : material(material_), painter(painter_) {}

		asset<Material> material;
		asset<Painter> painter;
	};

	template<typename T>
	struct Quad : Visage
	{
		void batch();
		void sync(aabb&);
		void sync(circle&);
		void sync(poly2d&);

		void setLT(const T& vertex) { mVertices[0] = vertex; }
		void setRT(const T& vertex) { mVertices[1] = vertex; }
		void setRB(const T& vertex) { mVertices[2] = vertex; }
		void setLB(const T& vertex) { mVertices[3] = vertex; }

		Quad() : Visage(), mDepth(0) {};
		Quad(asset<Material> material_, asset<Painter> painter_);
		T mVertices[4];
		float mDepth;
	};

	template<typename T>
	inline Quad<T>::Quad(asset<Material> material_, asset<Painter> painter_) : Visage(material_, painter_), mDepth(0.0f) {}

	template<typename T>
	inline void Quad<T>::batch() {
		if (painter && material) {
			painter->batch(
				SpriteVertexData::sVertexDataIndex,
				&mVertices[0], &gBox2dFillIndices[0],
				4, 6, (uint16)material.index(), 0, mDepth
			);
		}
	}

	template<typename T>
	inline void Quad<T>::sync(aabb& box) {
		box.l = mVertices[0].pos.x;
		box.b = mVertices[0].pos.y;
		box.r = mVertices[2].pos.x;
		box.t = mVertices[2].pos.y;
		box.normalize();
	}

	template<typename T>
	inline void Quad<T>::sync(circle& circ) {
		auto& lb = mVertices[0].pos;
		auto& rt = mVertices[2].pos;

		circ.x = (lb.x + rt.x) / 2.0f;
		circ.y = (lb.y + rt.y) / 2.0f;
		circ.r = (rt.x - lb.x) / 2.0f;
	}

	template<typename T>
	inline void Quad<T>::sync(poly2d& poly) {
		for (int i = 0; i < 4; i++)
			poly.verts[i] = mVertices[i].pos;
		poly.count = 4;
	}
}