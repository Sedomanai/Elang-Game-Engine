#include "visage.h"

namespace el
{
	template struct Visage<0, 0, 0>;
	template struct Visage<1, 1, 1>;
	template struct Visage<1, 0, 1>;
	template struct Visage<1, 1, 0>;
	template struct Visage<1, 0, 0>;

	template struct Quad<SpriteVertex, 0, 0, 0>;
	template struct Quad<SpriteVertex, 1, 1, 1>;
	template struct Quad<SpriteVertex, 1, 0, 1>;
	template struct Quad<SpriteVertex, 1, 1, 0>;
	template struct Quad<SpriteVertex, 1, 0, 0>;

	template<typename V, int M, int T, int C>
	Quad<V, M, T, C>::Quad(asset<MaterialImpl<M, T>> material_, asset<PainterImpl<M, T, C>> painter_)
		: Visage<M, T, C>(material_, painter_), mDepth(0.0f) {}

	template<typename V, int M, int T, int C>
	void Quad<V, M, T, C>::batch() {
		if (Visage<M, T, C>::painter && Visage<M, T, C>::material) {
				Visage<M, T, C>::painter->batch<SpriteVertex>(
				&mVertices[0], &gBox2dFillIndices[0],
				4, 6, Visage<M, T, C>::material.index(), 0, mDepth
			);
		}
	}

	template<typename V, int M, int T, int C>
	void Quad<V, M, T, C>::sync(aabb& box) {
		box.l = mVertices[0].pos.x;
		box.b = mVertices[0].pos.y;
		box.r = mVertices[2].pos.x;
		box.t = mVertices[2].pos.y;
		box.normalize();
	}

	template<typename V, int M, int T, int C>
	void Quad<V, M, T, C>::sync(circle& circ) {
		auto& lb = mVertices[0].pos;
		auto& rt = mVertices[2].pos;

		circ.x = (lb.x + rt.x) / 2.0f;
		circ.y = (lb.y + rt.y) / 2.0f;
		circ.r = (rt.x - lb.x) / 2.0f;
	}

	template<typename V, int M, int T, int C>
	void Quad<V, M, T, C>::sync(poly2d& poly) {
		for (int i = 0; i < 4; i++)
			poly.verts[i] = mVertices[i].pos;
		poly.count = 4;
	}
}