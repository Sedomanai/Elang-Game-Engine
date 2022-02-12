#include "visage.h"

namespace el
{
	Quad::Quad(asset<Material> material_, asset<Painter> painter_) : Visage(material_, painter_), mDepth(0.0f) {}

	void Quad::batch() {
		if (painter && material) {
			painter->batch<SpriteVertex>(
				&mVertices[0], &gBox2dFillIndices[0],
				4, 6, material.index(), 0, mDepth
			);
		}
	}

	void Quad::sync(aabb& box) {
		box.l = mVertices[0].pos.x;
		box.b = mVertices[0].pos.y;
		box.r = mVertices[2].pos.x;
		box.t = mVertices[2].pos.y;
		box.normalize();
	}

	void Quad::sync(circle& circ) {
		auto& lb = mVertices[0].pos;
		auto& rt = mVertices[2].pos;

		circ.x = (lb.x + rt.x) / 2.0f;
		circ.y = (lb.y + rt.y) / 2.0f;
		circ.r = (rt.x - lb.x) / 2.0f;
	}

	void Quad::sync(poly2d& poly) {
		for (int i = 0; i < 4; i++)
			poly.verts[i] = mVertices[i].pos;
		poly.count = 4;
	}
}