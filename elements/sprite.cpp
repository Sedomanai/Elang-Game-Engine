#include "sprite.h"

#include "basic.h"

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


	Sprite::Sprite(asset<Material> material_, asset<Painter> painter_, const string& cell)
		: Quad(material_, painter_), mFlipped(false)
	{
		setCell(cell);
	}
	void Sprite::setCell(const string& value) {
		if (material && material->hasTexture()) {
			auto tex = material->textures[0];
			if (tex->atlas) {
				mCell = tex->atlas->cells.safe_data(value);
			}
		}
	}

	void Sprite::updatePositionSprite(ent<Position> position) {
		if (position) {
			auto& pos = *position;
			if (mCell) {
				auto& cell = *mCell;
				if (mFlipped) {
					mVertices[0].pos = vec2(cell.right, cell.up) + pos;
					mVertices[0].uv = vec2(cell.uvLeft, cell.uvUp);
					mVertices[1].pos = vec2(cell.left, cell.up) + pos;
					mVertices[1].uv = vec2(cell.uvRight, cell.uvUp);
					mVertices[2].pos = vec2(cell.left, cell.down) + pos;
					mVertices[2].uv = vec2(cell.uvRight, cell.uvDown);
					mVertices[3].pos = vec2(cell.right, cell.down) + pos;
					mVertices[3].uv = vec2(cell.uvLeft, cell.uvDown);
				} else {
					mVertices[0].pos = vec2(cell.left, cell.up) + pos;
					mVertices[0].uv = vec2(cell.uvLeft, cell.uvUp);
					mVertices[1].pos = vec2(cell.right, cell.up) + pos;
					mVertices[1].uv = vec2(cell.uvRight, cell.uvUp);
					mVertices[2].pos = vec2(cell.right, cell.down) + pos;
					mVertices[2].uv = vec2(cell.uvRight, cell.uvDown);
					mVertices[3].pos = vec2(cell.left, cell.down) + pos;
					mVertices[3].uv = vec2(cell.uvLeft, cell.uvDown);
				}
			}
			else {
				auto tex = material->textures[0];
				auto w = float(tex->width()) / 2.0f;
				auto h = float(tex->height()) / 2.0f;
				mVertices[0].pos = vec2(-w, -h) + pos;
				mVertices[0].uv = vec2(0.0f, 1.0f);
				mVertices[1].pos = vec2(w, -h) + pos;
				mVertices[1].uv = vec2(1.0f, 1.0f);
				mVertices[2].pos = vec2(w, h) + pos;
				mVertices[2].uv = vec2(1.0f, 0.0f);
				mVertices[3].pos = vec2(-w, h) + pos;
				mVertices[3].uv = vec2(0.0f, 0.0f);
			}
			return;
		}
	}

	void Sprite::updatePlanarSprite(ent<Planar> plane) {
		if (plane) {
			auto& tr = *plane;
			if (mCell) {
				auto& cell = *mCell;
				mVertices[0].pos = tr * vec2(cell.left, cell.up);
				mVertices[0].uv = vec2(cell.uvLeft, cell.uvUp);
				mVertices[1].pos = tr * vec2(cell.right, cell.up);
				mVertices[1].uv = vec2(cell.uvRight, cell.uvUp);
				mVertices[2].pos = tr * vec2(cell.right, cell.down);
				mVertices[2].uv = vec2(cell.uvRight, cell.uvDown);
				mVertices[3].pos = tr * vec2(cell.left, cell.down);
				mVertices[3].uv = vec2(cell.uvLeft, cell.uvDown);
			}
			else {
				auto tex = material->textures[0];
				auto w = float(tex->width()) / 2.0f;
				auto h = float(tex->height()) / 2.0f;
				mVertices[0].pos = tr * vec2(-w, -h);
				mVertices[0].uv = vec2(0.0f, 1.0f);
				mVertices[1].pos = tr * vec2(w, -h);
				mVertices[1].uv = vec2(1.0f, 1.0f);
				mVertices[2].pos = tr * vec2(w, h);
				mVertices[2].uv = vec2(1.0f, 0.0f);
				mVertices[3].pos = tr * vec2(-w, h);
				mVertices[3].uv = vec2(0.0f, 0.0f);
			}
			return;
		}
	}

	void Sprite::update(Entity e) {
		if (material && material->hasTexture()) {
			updatePositionSprite(e);
			updatePlanarSprite(e);
		}
	}
}