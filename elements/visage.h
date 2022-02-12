#pragma once
#include "../tools/registry.h"
#include "../tools/material.h"
#include "../tools/painter.h"

namespace el
{
	template<typename U, typename V>
	struct Visage
	{
		Visage() {}
		Visage(asset<U> material_, asset<V> painter_) : material(material_), painter(painter_) {}

		asset<U> material;
		asset<V> painter;
	};

	template<typename T, typename U, typename V>
	struct Quad : Visage<U, V>
	{
		void batch();
		void sync(aabb&);
		void sync(circle&);
		void sync(poly2d&);

		void setLT(const T& vertex) { mVertices[0] = vertex; }
		void setRT(const T& vertex) { mVertices[1] = vertex; }
		void setRB(const T& vertex) { mVertices[2] = vertex; }
		void setLB(const T& vertex) { mVertices[3] = vertex; }

		Quad() : Visage<U, V>(), mDepth(0) {};
		Quad(asset<U> material_, asset<V> painter_);

	protected:
		T mVertices[4];
		float mDepth;
	};

	template<typename T, typename U, typename V>
	inline Quad<T, U, V>::Quad(asset<U> material_, asset<V> painter_) : Visage<U, V>(material_, painter_), mDepth(0.0f) {}

	template<typename T, typename U, typename V>
	inline void Quad<T, U, V>::batch() {
		if (Visage<U, V>::painter && Visage<U, V>::material) {
			cout << "on batch " << Visage<U, V>::material.index() << endl;
			Visage<U, V>::painter->batch<SpriteVertex>(
				&mVertices[0], &gBox2dFillIndices[0],
				4, 6, Visage<U, V>::material.index(), 0, mDepth
			);
		}
	}

	template<typename T, typename U, typename V>
	inline void Quad<T, U, V>::sync(aabb& box) {
		box.l = mVertices[0].pos.x;
		box.b = mVertices[0].pos.y;
		box.r = mVertices[2].pos.x;
		box.t = mVertices[2].pos.y;
		box.normalize();
	}

	template<typename T, typename U, typename V>
	inline void Quad<T, U, V>::sync(circle& circ) {
		auto& lb = mVertices[0].pos;
		auto& rt = mVertices[2].pos;

		circ.x = (lb.x + rt.x) / 2.0f;
		circ.y = (lb.y + rt.y) / 2.0f;
		circ.r = (rt.x - lb.x) / 2.0f;
	}

	template<typename T, typename U, typename V>
	inline void Quad<T, U, V>::sync(poly2d& poly) {
		for (int i = 0; i < 4; i++)
			poly.verts[i] = mVertices[i].pos;
		poly.count = 4;
	}
}