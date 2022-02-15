#include "canvas.h"

namespace el
{
	template struct CanvasImpl<SpriteVertex, 0, 0, 0>;
	template struct CanvasImpl<SpriteVertex, 1, 0, 1>;
	template struct CanvasImpl<SpriteVertex, 1, 1, 1>;

	template<typename V, int M, int T, int C>
	CanvasImpl<V, M, T, C>::CanvasImpl(asset<MaterialImpl<M, T>> material_, asset<PainterImpl<M, T, C>> painter_) 
		: Quad<V, M, T, C>(material_, painter_),
		bounds(-50, -50, 50, 50), uvpos(0, 0), uvsize(0, 0)
	{
		if (Visage<M, T, C>::material && Visage<M, T, C>::material->hasTexture()) {
			auto tex = Visage<M, T, C>::material->textures[0];
			auto w = tex->width() / 2.0f;
			auto h = tex->height() / 2.0f;
			bounds = aabb(-w, -h, w, h);
			uvsize.x = float(tex->width());
			uvsize.y = float(tex->height());
		}
	}

	template<typename V, int M, int T, int C>
	void CanvasImpl<V, M, T, C>::update() {
		if (Visage<M, T, C>::material && Visage<M, T, C>::material->hasTexture()) {
			auto tex = Visage<M, T, C>::material->textures[0];
			auto w = float(tex->width());
			auto h = float(tex->height());
			auto uvx = uvpos.x / w;
			auto uvy = uvpos.y / h;
			auto uvw = uvsize.x / w;
			auto uvh = uvsize.y / h;

			Quad<V, M, T, C>::mVertices[0].uv = vec2(uvx, uvy + uvh);
			Quad<V, M, T, C>::mVertices[1].uv = vec2(uvx + uvw, uvy + uvh);
			Quad<V, M, T, C>::mVertices[2].uv = vec2(uvx + uvw, uvy);
			Quad<V, M, T, C>::mVertices[3].uv = vec2(uvx, uvy);
		}

		Quad<V, M, T, C>::mVertices[0].pos = vec2(bounds.l, bounds.t);
		Quad<V, M, T, C>::mVertices[1].pos = vec2(bounds.r, bounds.t);
		Quad<V, M, T, C>::mVertices[2].pos = vec2(bounds.r, bounds.b);
		Quad<V, M, T, C>::mVertices[3].pos = vec2(bounds.l, bounds.b);
	}


	template<typename V, int M, int T, int C>
	void CanvasImpl<V, M, T, C>::update(asset<CellImpl<T>> cell) {
		Quad<V, M, T, C>::mVertices[0].uv = vec2(cell->uvLeft, cell->uvUp);
		Quad<V, M, T, C>::mVertices[1].uv = vec2(cell->uvRight, cell->uvUp);
		Quad<V, M, T, C>::mVertices[2].uv = vec2(cell->uvRight, cell->uvDown);
		Quad<V, M, T, C>::mVertices[3].uv = vec2(cell->uvLeft, cell->uvDown);

		Quad<V, M, T, C>::mVertices[0].pos = vec2(bounds.l, bounds.t);
		Quad<V, M, T, C>::mVertices[1].pos = vec2(bounds.r, bounds.t);
		Quad<V, M, T, C>::mVertices[2].pos = vec2(bounds.r, bounds.b);
		Quad<V, M, T, C>::mVertices[3].pos = vec2(bounds.l, bounds.b);
	}
}