#include "canvas.h"

namespace el
{
	Canvas::Canvas(asset<Material> material_, asset<Painter> painter_) : Quad(material_, painter_),
		bounds(-50, -50, 50, 50), uvpos(0, 0), uvsize(0, 0)
	{
		if (material && material->hasTexture()) {
			auto tex = material->textures[0];
			auto w = tex->width() / 2.0f;
			auto h = tex->height() / 2.0f;
			bounds = aabb(-w, -h, w, h);
			uvsize.x = float(tex->width());
			uvsize.y = float(tex->height());
		}
	}

	void Canvas::update() {
		if (material && material->hasTexture()) {
			auto tex = material->textures[0];
			auto w = float(tex->width());
			auto h = float(tex->height());
			auto uvx = uvpos.x / w;
			auto uvy = uvpos.y / h;
			auto uvw = uvsize.x / w;
			auto uvh = uvsize.y / h;

			mVertices[0].uv = vec2(uvx, uvy + uvh);
			mVertices[1].uv = vec2(uvx + uvw, uvy + uvh);
			mVertices[2].uv = vec2(uvx + uvw, uvy);
			mVertices[3].uv = vec2(uvx, uvy);
		}

		mVertices[0].pos = vec2(bounds.l, bounds.b);
		mVertices[1].pos = vec2(bounds.r, bounds.b);
		mVertices[2].pos = vec2(bounds.r, bounds.t);
		mVertices[3].pos = vec2(bounds.l, bounds.t);
	}
}