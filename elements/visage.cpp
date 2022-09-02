#include <elpch.h>
#include "visage.h"
#include "../tools/vertex.h"
#include "../tools/painter.h"
#include "../tools/material.h"
#include "../common/aabb.h"
#include "../common/circle.h"
#include "../common/poly2d.h"

namespace el
{
	template struct Quad<SpriteVertex>;

	template<typename T>
	Quad<T>::Quad(asset<Material> material_, asset<Painter> painter_)
		: Visage(material_, painter_), mDepth(0.0f) {}

	template<typename T>
	void Quad<T>::batch() {
		if (Visage::painter && Visage::material) {
				Visage::painter->batch<T>(
				&mVertices[0], &gBox2dFillIndices[0],
				4, 6, Visage::material.index(), 0, mDepth
			);
		}
	}

	template<typename T>
	void Quad<T>::sync(aabb& box) {
		box.l = mVertices[0].pos.x;
		box.b = mVertices[0].pos.y;
		box.r = mVertices[2].pos.x;
		box.t = mVertices[2].pos.y;
		box.normalize();
	}

	template<typename T>
	void Quad<T>::sync(circle& circ) {
		auto& lb = mVertices[0].pos;
		auto& rt = mVertices[2].pos;

		circ.x = (lb.x + rt.x) / 2.0f;
		circ.y = (lb.y + rt.y) / 2.0f;
		circ.r = (rt.x - lb.x) / 2.0f;
	}

	template<typename T>
	void Quad<T>::sync(poly2d& poly) {
		for (int i = 0; i < 4; i++)
			poly.verts[i] = mVertices[i].pos;
		poly.count = 4;
	}
}