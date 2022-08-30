#include "canvas.h"

namespace el
{
	template struct Canvas<SpriteVertex>;

	template<typename T>
	Canvas<T>::Canvas() : Quad<T>(), bounds(-50, -50, 50, 50), uvpos(0, 0), uvsize(0, 0) {}

	template<typename T>
	Canvas<T>::Canvas(asset<Material> material_, asset<Painter> painter_) 
		: Quad<T>(material_, painter_),
		bounds(-50, -50, 50, 50), uvpos(0, 0), uvsize(0, 0)
	{
		if (Visage::material && Visage::material->hasTexture()) {
			auto tex = Visage::material->textures[0];
			auto w = tex->width() / 2.0f;
			auto h = tex->height() / 2.0f;
			bounds = aabb(-w, -h, w, h);
			uvsize.x = float(tex->width());
			uvsize.y = float(tex->height());
		}
	}

	template<typename T>
	void Canvas<T>::update() {
		if (Visage::material && Visage::material->hasTexture()) {
			auto tex = Visage::material->textures[0];
			auto w = float(tex->width());
			auto h = float(tex->height());
			auto uvx = uvpos.x / w;
			auto uvy = uvpos.y / h;
			auto uvw = uvsize.x / w;
			auto uvh = uvsize.y / h;

			Quad<T>::mVertices[0].uv = vec2(uvx, uvy + uvh);
			Quad<T>::mVertices[1].uv = vec2(uvx + uvw, uvy + uvh);
			Quad<T>::mVertices[2].uv = vec2(uvx + uvw, uvy );
			Quad<T>::mVertices[3].uv = vec2(uvx, uvy);
		}

		Quad<T>::mVertices[0].pos = vec2(bounds.l, bounds.t);
		Quad<T>::mVertices[1].pos = vec2(bounds.r, bounds.t);
		Quad<T>::mVertices[2].pos = vec2(bounds.r, bounds.b);
		Quad<T>::mVertices[3].pos = vec2(bounds.l, bounds.b);
	}

	template<typename T>
	void Canvas<T>::update(asset<Cell> cell) {
		Quad<T>::mVertices[0].uv = vec2(cell->uvLeft, cell->uvUp);
		Quad<T>::mVertices[1].uv = vec2(cell->uvRight, cell->uvUp);
		Quad<T>::mVertices[2].uv = vec2(cell->uvRight, cell->uvDown);
		Quad<T>::mVertices[3].uv = vec2(cell->uvLeft, cell->uvDown);

		Quad<T>::mVertices[0].pos = vec2(bounds.l, bounds.t);
		Quad<T>::mVertices[1].pos = vec2(bounds.r, bounds.t);
		Quad<T>::mVertices[2].pos = vec2(bounds.r, bounds.b);
		Quad<T>::mVertices[3].pos = vec2(bounds.l, bounds.b);
	}
}